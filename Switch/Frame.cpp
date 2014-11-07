#include "stdafx.h"
#include "Frame.h"


Frame::Frame(void)
	: frame(NULL)
{
}


Frame::~Frame(void)
{
}


void Frame::AddFrame(const u_char *data_const)
{
	u_char *data = const_cast<u_char *>(data_const);
	
	send(buffer,data);
}


void Frame::GetFrame(void)
{
	if (frame) free(frame);
	frame = receive(buffer);
}


MACaddr Frame::GetSrcMAC(void)
{
	MACaddr src;
	int i;
	
	for (i=6;i < 12;i++) src.b[i-6] = frame[i];
	return src;
}


MACaddr Frame::GetDestMAC(void)
{
	MACaddr dest;
	int i;

	for (i=0;i < 6;i++) dest.b[i] = frame[i];
	return dest;
}


u_char * Frame::GetData(void)
{
	return frame;
}
