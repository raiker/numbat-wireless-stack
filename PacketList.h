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

#ifndef PACKETLIST_H_
#define PACKETLIST_H_

#include <stdint.h>

// Stores last 1000 packets received from each node
class PacketList {
protected:
    uint32_t * packets;
    uint32_t count;

public:
    PacketList();
    virtual ~PacketList();
    virtual void addPacket(uint32_t packet);
    virtual bool findPacket(uint32_t packet);

};

#endif /* PACKETLIST_H_ */
