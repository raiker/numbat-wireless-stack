#ifndef __PACKET_RADIO_SMARTPACKETRADIO_H_
#define __PACKET_RADIO_SMARTPACKETRADIO_H_

#include <csimplemodule.h>
#include <cmessage.h>

class SmartPacketRadio : public cSimpleModule
{

private:
    struct packet
    {
        char control;
        char dest;
        unsigned char length;
        char payload[255];
    } packet;

    int fd;
    unsigned char address;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

public:
    virtual inline unsigned char getAddress() {return address;}
};

#endif
