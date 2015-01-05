#include "file_define.h"
#include "packet.h"

void putByte(char* packet, unsigned char value, int& pos)
{
  *(unsigned char*)(pakcet + pos) = value;
  pos += sizeof(unsigned char);
}

unsigned char getByte(char* packet, int& pos)
{
  unsigned char value = *(unsigned char*)(packet + pos);
  pos += sizeof(unsigned char);
  return value;
}

void putWord(char* packet, unsigned short, int& pos)
{
  *(unsigned short*)(packet + pos) = value;
  pos += sizeof(unsigned short);
  return value;
}

unsigned short getWord(char* packet, int& pos)
{
  unsigned short value = *(unsigned short*)(packet + pos);
  pos += sizeof(unsigned short);
  return value;
}

void putDWord(char* packet, unsigned long value, int& pos)
{
  *(unsigned long*)(packet + pos) = value;
  pos += sizeof(unsigned long);
}

unsigned long getDWord(char* packet, int& pos)
{
  unsigned long value = *(unsigned long*)(packet + pos);
  pos += sizeof(unsigned long);
  return value;
}

void putInteger(char* packet, int value, int& pos)
{
  *(int*)(packet + pos) = value;
  pos += sizeof(int);
}

int getInteger(char* packet, int& pos)
{
  int value = *(int*)(packet + pos);
  pos += sizeof(int);
  return value;
}

void putShort(char* packet, short value, int& pos)
{
  *(short*)(packet + pos) = value;
  pos += sizeof(short);
}

short getShort(char* packet, int& pos)
{
  short value = *(short*)(packet + pos);
  pos += sizeof(short);
  return value;
}

void putString(char* packet, char* str, int& pos)
{
  *(unsigned short*)(packet + pos) = strlen(str);
  pos += sizeof(unsigned short);
  memcpy(packet + pos, str, strlen(str));
  pos += strlen(str);
}

void getString(char* packet, char* buffer, int& pos)
{
  unsigned short bufferLength;
  bufferLength = *(unsigned short*)(packet + pos);
  pos += sizeof(unsigned short);
  memcpy(buffer, packet + pos, bufferLength);
  *(buffer + bufferLength) = '\0';
  pos += bufferLength;
}

void setSize(char* packet, unsigned short pos)
{
  *(unsigned short*)packet = pos;
}
  
