#pragma once
#include "afxtempl.h"
#include "Frame.h"

enum DIRECTION {In, Out};

struct Statistic {
	int port;
	DIRECTION d;
	FRAME_TYPE FrameType;
	WORD Lay3Type;
	BYTE Lay4Type;
	char HasFrameType, HasLay3Type, HasLay4Type;
	UINT count;
};

class Stats
{
public:
	Stats(void);
	~Stats(void);
private:
	CArray<Statistic> table;
public:
	void Add(int port, DIRECTION FDirection, Frame * f);
	void Reset(void);
};

