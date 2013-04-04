#include "StdErrSink.h"
#include "SmartPacket_m.h"

Define_Module(StdErrSink);

void StdErrSink::handleMessage(cMessage *msg)
{
    SmartPacket *sp=static_cast<SmartPacket *>(msg);
    if(sp->getBuffer()[0]==(char)0x81) fwrite(sp->getBuffer()+1,1,sp->getLength()-1,stderr);
    else for(int i=0;i<sp->getLength();i++) fprintf(stderr,"0x%02x ",(uint8_t)sp->getBuffer()[i]);
    delete[] sp->getBuffer();
    delete msg;
}
