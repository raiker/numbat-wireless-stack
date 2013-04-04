package spr;

simple SmartPacketRadio
{
    parameters:
        int address=default(0);
        int broadcast=default(255);
      
    gates:
        input in[2];
        output out;
}
