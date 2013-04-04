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

#include "PacketList.h"

PacketList::PacketList() {
	// TODO Auto-generated constructor stub
	packets = new uint32_t[1000];
	count = 0;
}

void PacketList::addPacket(uint32_t packet) {
	if (count >= 1000) {
		count = 0;
	}
	packets[count] = packet;
	count++;
}

bool PacketList::findPacket(uint32_t packet) {
	bool found = false;
	for (uint32_t i = 0; (i <= count); i++) {
		if (packets[i] == packet) {
			found = true;
		}
	}
	return found;
}
PacketList::~PacketList() {
	// TODO Auto-generated destructor stub
}

