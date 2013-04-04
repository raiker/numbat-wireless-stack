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

#include "Source.h"
#include "SmartPacket_m.h"

Define_Module(Source);

Source::Source()
{
    timerMessage = NULL;
}

Source::~Source()
{
    cancelAndDelete(timerMessage);
}

void Source::initialize()
{
    timerMessage = new cMessage("timer");
    scheduleAt(simTime(), timerMessage);
    EV<<"Broadcasting to all radios.. \n";
}

void Source::handleMessage(cMessage *msg)
{
    ASSERT(msg==timerMessage);
    static int n=0;
    SmartPacket *job=new SmartPacket();
    char *buffer=new char[100];
    int src=par("address");
    int dest=par("broadcast");
    // Outgoing packet format: Source Dest Address #packetnumber Type (h=hello,a=ack)
    sprintf(buffer,"%d %d %d %d h",src,dest,dest,(src*1000000)+n++);
    job->setBuffer(buffer);
    job->setLength(strlen(buffer)+1);
    job->setDest(dest);
    send(job,"out");
    scheduleAt(simTime()+par("sendInterval").doubleValue(), timerMessage);
}
