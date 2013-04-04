#ifndef __PACKET_RADIO_SMARTPACKETRADIO_H_
#define __PACKET_RADIO_SMARTPACKETRADIO_H_

#include <csimplemodule.h>
#include <cmessage.h>
#include <stdint.h>

class SmartPacketRadio : public cSimpleModule
{

private:
    struct packet
    {
        uint8_t control;
        uint8_t dest;
        uint8_t length;
        uint8_t payload[255];
    } packet;

    int fd;
    uint8_t address;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

public:
    virtual inline uint8_t getAddress() {return address;}
};

#endif
