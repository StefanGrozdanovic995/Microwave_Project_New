#include "Wire.h" //<> for installed library, "" for created library

#define MCP_ADDR  0x21

#define IODIRA    0x00          //Register that holds in information that define what pins on port A are input or outputs. 1 - input; 0 - output
#define IODIRB    0x01          //Register that holds in information that define what pins on port B are input or outputs. 1 - input; 0 - output
#define IPOLA     0x02 
#define IPOLB     0x03 
#define GPINTENA  0x04 
#define GPINTENB  0x05 
#define GPPUA     0x0C 
#define GPPUB     0x0D 
#define GPIOA     0x12 
#define GPIOB     0x13 
#define OLATA     0x14 
#define OLATB     0x15 

void mcpWrite(char reg, int value)
{
  Wire.beginTransmission(MCP_ADDR); //begin transmission to MCP_ADDR
  Wire.write(reg);                  //Set register to write to - IO Direction Input/Output setting
  Wire.write(value);                //Write 1111 1100 to the port A to turn pins 0-1 as outputs and leave 2-7 as inputs 
  Wire.endTransmission();           //Complete transmission  
}

byte mcpRead(char reg)
{
  byte data;
  Wire.beginTransmission(MCP_ADDR); 
  Wire.write(reg);                  //Set register you wish to read from 
  Wire.endTransmission();           

  Wire.requestFrom(MCP_ADDR,1);     //begin transmission and ask for 1 byte
  data = Wire.read();               //read the byte 
  Wire.endTransmission();   
  return data;        
}

void MCPsetup()
{
  Wire.begin();

  mcpWrite(IODIRA, 0b11111111);
  mcpWrite(IODIRB, 0b01111111); //first bit is zero because we want to turn on led 7 on MCP regB
  mcpWrite(IPOLA , 0b11111111);
  mcpWrite(IPOLB , 0b11111111);
}
