#pragma once

#include <agents.h>
#include "MACtable.h"

using namespace Concurrency;

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
};

