#ifndef __SPR_STDINSOURCE_H
#define __SPR_STDINSOURCE_H

#include <omnetpp.h>
class StdInSource : public cSimpleModule
{
  private:
    char *buffer;
    int n;
    unsigned char dest;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
