#pragma once
#include "Stats.h"
#include "afxtempl.h"

enum POLICY {ALLOW, DENY};

struct Rule {
	POLICY p;
	int SWport;
	DIRECTION d;
	MACaddr SrcMAC;
	MACaddr DestMAC;
	IPaddr SrcIP;
	IPaddr DestIP;
	WORD Lay3Type;
	BYTE Lay4Type;
	WORD AppPort;
	char enabled, HasPort, HasDirection, HasSrcMAC, HasDestMAC, HasLay3Type, HasSrcIP, HasDestIP, HasLay4Type, HasAppPort;
};

class Filter
{
public:
	Filter(void);
	~Filter(void);
private:
	CArray<Rule> rules;
public:
	int Check(int port, DIRECTION FDirection, Frame * f);
	void Add(Rule &NewRule);
	Rule& Get(int index);
	void Modify(int index, Rule &NewRule);
	void SetEnabled(int index, int IsEnabled);
	void Remove(int index);
	void RemoveAll(void);
	int CompareMAC(MACaddr& mac1, MACaddr& mac2);
	int CompareIP(IPaddr& ip1, IPaddr& ip2);
};

