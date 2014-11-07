#pragma once

#include <agents.h>
#include "MACtable.h"

using namespace Concurrency;

class Frame
{
public:
	Frame(void);
	~Frame(void);
private:
	unbounded_buffer<u_char *> buffer;
	u_char *frame;
public:
	void AddFrame(const u_char *data);
	void GetFrame(void);
	MACaddr GetSrcMAC(void);
	MACaddr GetDestMAC(void);
	u_char * GetData(void);
};

