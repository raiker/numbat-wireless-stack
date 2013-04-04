#include "SmartPacketRadio.h"
#include "SmartPacket_m.h"
#include "PollScheduler.h"
#include "PacketList.h"
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

Define_Module(SmartPacketRadio)
;

void SmartPacketRadio::initialize() {
	termios t;
	memset(&t, '\0', sizeof(t));
	t.c_cflag = B230400 | CRTSCTS | CS8 | CLOCAL | CREAD;
	t.c_iflag = IGNPAR;
	t.c_oflag = 0;
	t.c_lflag = 0;
	t.c_cc[VTIME] = 0;
	t.c_cc[VMIN] = 0;

	address = par("address");
	if (not address)
		address = 0; //TODO find any /dev/radio*

	char device[20];
	sprintf(device, "/dev/radio%u", address);
	fd = open(device, O_RDWR);
	if (fd < 0)
		throw cRuntimeError(this, "Couldn't open radio %d.", address);

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &t);

	uint8_t reset = 0x01;
	uint8_t setAddress[] = { 0x08, address };
	uint8_t mac[] = { 0x02, 0x00 }; // for setting the MAC algorithm

	if (write(fd, &reset, 1) < 0 or write(fd, setAddress, sizeof(setAddress)) < 0 or write(fd, mac, sizeof(mac)) < 0)
		throw cRuntimeError(this, "Couldn't initialise radio %d.", address);

	check_and_cast<PollScheduler *>(cSimulation::getActiveSimulation()->getScheduler())->registerFD(this, fd);
	packet.control = 0x10;

}

void SmartPacketRadio::finish() {
	close(fd);
}

void SmartPacketRadio::handleMessage(cMessage *msg) {
	if (not strcmp(msg->getName(), "ready to read")) {
		SmartPacket *smartPacket = new SmartPacket();
		smartPacket->setDest(0);
		char *buffer = new char[256];
		smartPacket->setBuffer(buffer);
		smartPacket->setLength(read(fd, buffer, 256));
		send(smartPacket, "out");
	} else {
		SmartPacket *smartPacket = static_cast<SmartPacket *>(msg);
		packet.dest = smartPacket->getDest();
		packet.length = smartPacket->getLength();
		memcpy(packet.payload, smartPacket->getBuffer(), packet.length);
		if (write(fd, &packet, packet.length + 3) < 0)
			throw cRuntimeError(this, "Write to radio %d failed.", address);
		delete[] smartPacket->getBuffer();
		delete msg;
	}
}
