#ifndef __PACKET_H__
#define __PACKET_H__

// set value of a packet at pos.
void putByte(char* packet, unsigned char value, int& pos);

// get value of a packet at pos.
unsigned char getByte(char* packet, int& pos);

void putWord(char* packet, unsigned short value, int& pos);

unsigned short getWord(char* packet, int& pos);

void putDWord(char* packet, unsigned long value, int& pos);

unsigned long getDWord(char* packet, int& pos);

void putInteger(char* packet, int value, int& pos);

int getInteger(char* packet, int& pos);

void putShort(char* packet, short value, int& pos);

short getShort(char* packet, int& pos);

void putString(char* packet, char* str, int& pos);

void getString(char* packet, char* buffer, int& pos);

void setSize(char* packet, unsigned short pos);

#endif

