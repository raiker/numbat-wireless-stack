//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Sink.h"
#include "SmartPacket_m.h"
#include "SmartPacketRadio.h"
#include <sstream>
#include <string>
#include "PacketList.h"
#include <cstringtokenizer.h>

Define_Module(Sink)
;

void Sink::handleMessage(cMessage *msg) {
	SmartPacket *sp = static_cast<SmartPacket *>(msg);
	int source, dest;
	uint32_t packetNumber;
	int thisRadio = par("address");
	int broadcastAddress = par("broadcast");
	int address;
	char *incomingPacket = sp->getBuffer() + 1;
	char packetType;
	char * buffer;
	SmartPacket *job;
	static int n = 0;
	PacketList *packetList = new PacketList();

	// Packet format: Source Destination #Number <-- Payload -->
	cStringTokenizer tokenizer(incomingPacket);
	if (tokenizer.hasMoreTokens())
		source = strtoul(tokenizer.nextToken(), nullptr, 10);
	if (tokenizer.hasMoreTokens())
		dest = strtoul(tokenizer.nextToken(), nullptr, 10);
	if (tokenizer.hasMoreTokens())
		address = strtoul(tokenizer.nextToken(), nullptr, 10);
	if (tokenizer.hasMoreTokens())
		packetNumber = strtoul(tokenizer.nextToken(), nullptr, 10);
	if (tokenizer.hasMoreTokens())
		packetType = *tokenizer.nextToken();

	// Prints each incoming packet
	EV << "Source: " << source
			<< " dest: " << dest
			<< " #" << packetNumber;
	if (packetType == 'a')
		EV << " (ack)";
	if (packetType == 'h')
		EV << " (hello)";
	EV << "\n";

	   if (packetList->findPacket(packetNumber) || ((packetNumber/1000000)==thisRadio)) {
	        // Discard if already received packet. Ignore packets sent by this radio and acks.
	    }
	    else {
	        packetList->addPacket(packetNumber);
	        if (packetType == 'a') {

	        }
				else {
				if (address == thisRadio) {
					// ACK
					job=new SmartPacket();
					buffer=new char[100];
					sprintf(buffer,"%d %d %d %d a",thisRadio,broadcastAddress,source,(thisRadio*1000000)+100000+n++);
					job->setBuffer(buffer);
					job->setLength(strlen(buffer)+1);
					job->setDest(broadcastAddress);
					send(job,"out");
				}
				else if (address == broadcastAddress) {
					// Add to neighbour table
				}
				// Pass on
				else {
					// Pass on packet
					job=new SmartPacket();
					buffer=new char[100];
					sprintf(buffer,"%d %d %d %d h",source,broadcastAddress,address,packetNumber);
					job->setBuffer(buffer);
					job->setLength(strlen(buffer)+1);
					job->setDest(dest);
					send(job,"out");
				}
			}
	    }

	delete[] sp->getBuffer();
	delete msg;
}
