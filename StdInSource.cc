#include "StdInSource.h"
#include "PollScheduler.h"
#include "SmartPacket_m.h"
#include "fcntl.h"

Define_Module(StdInSource);

void StdInSource::initialize()
{
    static_cast<PollScheduler *>(cSimulation::getActiveSimulation()->getScheduler())->registerFD(this,0);
    n=0;
    buffer=new char[100];
    fcntl(0,F_SETFL,fcntl(0,F_GETFL)|O_NONBLOCK);
    dest=par("dest");
    scheduleAt(simTime(),new cMessage("flush buffer"));
}

void StdInSource::handleMessage(cMessage *msg)
{
    if(msg->isName("ready to read")) n+=read(0,buffer+n,20-n);
    else if(msg->isName("flush buffer")) scheduleAt(simTime()+0.3,msg);
    else delete msg;

    if(n>=20 or (n>0 and msg->isName("flush buffer")))
    {
        SmartPacket *smartPacket=new SmartPacket();
        smartPacket->setDest(dest);
        smartPacket->setBuffer(buffer);
        smartPacket->setLength(n);
        send(smartPacket,"out");
        n=0;
        buffer=new char[100];
    }
}
