#pragma once

//#include "SwitchPort.h"
#include "afxtempl.h"

#define DEF_TIMEOUT_SEC 300

struct MACaddr {
	BYTE b[6];
};

struct StoredMAC {
	int port;
	MACaddr address;
	UINT SecondsLeft;
};

class MACtable
{
public:
	MACtable(void);
	~MACtable(void);
private:
	CArray<StoredMAC> TableEntry;
	UINT TimeOut;
public:
	void Add(int port, MACaddr address);
	int Find(int port, MACaddr src, MACaddr dest);
	int CompareMAC(MACaddr& mac1, MACaddr& mac2);
	void Maintain(void);
	UINT GetTimeOut(void);
	void SetTimeOut(UINT seconds);
};

