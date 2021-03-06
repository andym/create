/*Copyright 2017 Bang & Olufsen A/S

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "SigmaProtocolDataFormatter.h"
#include <cstring>

SigmaProtocolDataFormatter::SigmaProtocolDataFormatter() {

}
SigmaProtocolDataFormatter::~SigmaProtocolDataFormatter() {

}

void SigmaProtocolDataFormatter::SigmaProtocolDataFormatter::WriteRequest(uint16_t addr, uint16_t length, uint8_t* data, uint8_t* sigmaWriteRequest) 
{
	sigmaWriteRequest[0] = SigmaCommandWriteCode;
	sigmaWriteRequest[10] = (uint8_t)((length & 0xFF00) >> 8);
	sigmaWriteRequest[11] = (uint8_t) (length & 0x00FF);

	sigmaWriteRequest[12] = (uint8_t)((addr & 0xFF00) >> 8);
	sigmaWriteRequest[13] = (uint8_t)(addr & 0x00FF);

	memcpy(&sigmaWriteRequest[14], data, length);
}

void SigmaProtocolDataFormatter::ReadRequest(uint16_t addr, uint16_t length, uint8_t* sigmaReadRequest) 
{
	sigmaReadRequest[0] = SigmaCommandReadCode;
	sigmaReadRequest[8] = (uint8_t)((length & 0xFF00) >> 8);
	sigmaReadRequest[9] = (uint8_t)(length & 0x00FF);

	sigmaReadRequest[10] = (uint8_t)((addr & 0xFF00) >> 8);
	sigmaReadRequest[11] = (uint8_t)(addr & 0x00FF);
}

void SigmaProtocolDataFormatter::GetReadResponse(uint8_t* rawDataResponse, SigmaReadResponse* readRepsonse) 
{
	unsigned int len = (rawDataResponse[8] << 8) | rawDataResponse[9];
	unsigned int addr = (rawDataResponse[10] << 8) | rawDataResponse[11];

	readRepsonse->addr = (uint16_t) addr;
	readRepsonse->length = (uint16_t) len;

	if (len < MaxReadSize)
	{
		memcpy(readRepsonse->data, &rawDataResponse[14], len);
	}
}

void SigmaProtocolDataFormatter::EepromRequest(std::string pathToFile, uint16_t* requestLength, uint8_t* sigmaEepromRequest)
{
	unsigned int totalRequestSize = pathToFile.size() + 1 + SigmaCommandHeaderSize;
	*requestLength = totalRequestSize;

	sigmaEepromRequest[0] = SigmaCommandEepromCode;
	sigmaEepromRequest[1] = pathToFile.size();

	memcpy(&sigmaEepromRequest[SigmaCommandHeaderSize], pathToFile.c_str(), pathToFile.size());
}