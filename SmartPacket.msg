cplusplus {{typedef char * charp;}}
class noncobject charp;
message SmartPacket
{
	unsigned char dest;
    charp buffer;
    unsigned short length;
}