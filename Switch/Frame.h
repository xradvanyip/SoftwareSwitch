#pragma once

#include <agents.h>
#include "MACtable.h"

#define ETH2_HDR_LEN 14

using namespace Concurrency;

enum FRAME_TYPE {ETH2, RAW, SNAP, LLC};

struct IPaddr {
	BYTE b[4];
};

struct BufferedFrame {
	u_int FrameLength;
	u_char *FrameData;
};

class Frame
{
public:
	Frame(void);
	~Frame(void);
private:
	unbounded_buffer<BufferedFrame *> buffer;
	u_char *frame;
	u_int length;
public:
	void AddFrame(u_int length, const u_char *data);
	void GetFrame(void);
	MACaddr GetSrcMAC(void);
	MACaddr GetDestMAC(void);
	u_char * GetData(void);
	u_int GetLength(void);
	WORD MergeBytes(BYTE upper, BYTE lower);
	BYTE GetUpperByte(WORD number);
	BYTE GetLowerByte(WORD number);
	FRAME_TYPE GetType(void);
	WORD GetLay3Type(void);
	BYTE GetLay4Type(void);
	WORD GetLay4SrcPort(void);
	WORD GetLay4DestPort(void);
	IPaddr GetSrcIPaddr(void);
	IPaddr GetDestIPaddr(void);
};

