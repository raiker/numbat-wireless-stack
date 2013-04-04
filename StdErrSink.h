#ifndef __SPR_STDERRSINK_H
#define __SPR_STDERRSINK_H

#include <omnetpp.h>

class StdErrSink : public cSimpleModule
{
protected:
    virtual void handleMessage(cMessage *msg);
};

#endif
