#include "stdafx.h"
#include "Frame.h"


Frame::Frame(void)
	: frame(NULL)
	, length(0)
	, VLAN_ID(-1)
{
}


Frame::~Frame(void)
{
}


void Frame::AddFrame(u_int length, const u_char *data_const)
{
	BufferedFrame *FrameToAdd = (BufferedFrame *) malloc(sizeof(BufferedFrame));

	FrameToAdd->FrameLength = length;
	FrameToAdd->FrameData = (u_char *) malloc(length);
	memcpy(FrameToAdd->FrameData,data_const,length);
	
	send(buffer,FrameToAdd);
}


void Frame::GetFrame(void)
{
	BufferedFrame *FrameToExtract = receive(buffer);

	if (frame) free(frame);
	length = FrameToExtract->FrameLength;
	frame = FrameToExtract->FrameData;

	free(FrameToExtract);

	if ((length >= 18) && (MergeBytes(frame[12],frame[13]) == 0x8100)) VLAN_ID = (int) MergeBytes(frame[14] & 0x0F,frame[15]);
	else VLAN_ID = -1;
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


u_int Frame::GetLength(void)
{
	return length;
}


WORD Frame::MergeBytes(BYTE upper, BYTE lower)
{
	WORD num = upper << 8;
	num |= lower;
	return num;
}


BYTE Frame::GetUpperByte(WORD number)
{
	return number >> 8;
}


BYTE Frame::GetLowerByte(WORD number)
{
	return number & 0xFF;
}


FRAME_TYPE Frame::GetType(void)
{
	if (frame[12] >= 0x06) return ETH2;
	else if ((frame[14] == 0xFF) && (frame[15] == 0xFF)) return RAW;
	else if ((frame[14] == 0xAA) && (frame[15] == 0xAA) && (frame[16] == 0x03)) return SNAP;
	else return LLC;
}


WORD Frame::GetLay3Type(void)
{
	return MergeBytes(frame[12],frame[13]);
}


BYTE Frame::GetLay4Type(void)
{
	return frame[23];
}


WORD Frame::GetLay4SrcPort(void)
{
	int IP_header_length = (frame[14] & 0x0F) * 4;

	return MergeBytes(frame[ETH2_HDR_LEN+IP_header_length],frame[ETH2_HDR_LEN+IP_header_length+1]);
}


WORD Frame::GetLay4DestPort(void)
{
	int IP_header_length = (frame[14] & 0x0F) * 4;
	
	return MergeBytes(frame[ETH2_HDR_LEN+IP_header_length+2],frame[ETH2_HDR_LEN+IP_header_length+3]);
}


IPaddr Frame::GetSrcIPaddr(void)
{
	IPaddr src;
	int i;
	
	for (i=26;i < 30;i++) src.b[i-26] = frame[i];
	return src;
}


IPaddr Frame::GetDestIPaddr(void)
{
	IPaddr dest;
	int i;
	
	for (i=30;i < 34;i++) dest.b[i-30] = frame[i];
	return dest;
}


int Frame::IsTagged(void)
{
	if (VLAN_ID == -1) return 0;
	return 1;
}


WORD Frame::GetVID(void)
{
	return (WORD) VLAN_ID;
}


void Frame::Tag(WORD vid)
{
	u_char *TaggedFrame;
	
	if ((!frame) || (length < 14)) return;
	TaggedFrame = (u_char *) malloc(length+4);
	memcpy(TaggedFrame,frame,12);
	TaggedFrame[12] = 0x81;
	TaggedFrame[13] = 0x00;
	TaggedFrame[14] = GetUpperByte(vid);
	TaggedFrame[15] = GetLowerByte(vid);
	memcpy(TaggedFrame+16,frame+12,length-12);

	length += 4;
	VLAN_ID = (int) vid;
	free(frame);
	frame = TaggedFrame;
}


void Frame::UnTag(void)
{
	u_char *UnTaggedFrame;
	
	if ((!frame) || (length < 18)) return;
	UnTaggedFrame = (u_char *) malloc(length-4);
	memcpy(UnTaggedFrame,frame,12);
	memcpy(UnTaggedFrame+12,frame+16,length-16);

	length -= 4;
	VLAN_ID = -1;
	free(frame);
	frame = UnTaggedFrame;
}
