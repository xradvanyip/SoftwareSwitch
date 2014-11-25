#include "stdafx.h"
#include "Filter.h"


Filter::Filter(void)
{
}


Filter::~Filter(void)
{
}


int Filter::Check(int port, DIRECTION FDirection, Frame * f)
{
	int allowed = 1, i;
	
	for (i=0;i < rules.GetCount();i++)
	{
		if (!rules[i].enabled) continue;
		if ((rules[i].HasPort == 2) && (rules[i].SWport != port)) continue;
		if ((rules[i].HasDirection == 2) && (rules[i].d != FDirection)) continue;
		if ((rules[i].HasSrcMAC == 2) && (CompareMAC(rules[i].SrcMAC,f->GetSrcMAC()) == 1)) continue;
		if ((rules[i].HasDestMAC == 2) && (CompareMAC(rules[i].DestMAC,f->GetDestMAC()) == 1)) continue;
		if ((rules[i].HasLay3Type == 2) && (rules[i].Lay3Type != f->GetLay3Type())) continue;
		if ((rules[i].HasLay3Type == 2) && (rules[i].Lay3Type == 0x0800))
		{
			if ((rules[i].HasSrcIP == 2) && (CompareIP(rules[i].SrcIP,f->GetSrcIPaddr()) == 1)) continue;
			if ((rules[i].HasDestIP == 2) && (CompareIP(rules[i].DestIP,f->GetDestIPaddr()) == 1)) continue;
			if ((rules[i].HasLay4Type == 2) && (rules[i].Lay4Type != f->GetLay4Type())) continue;
			if ((rules[i].HasAppPort == 2) && (rules[i].AppPort != f->GetLay4SrcPort()) && (rules[i].AppPort != f->GetLay4DestPort())) continue;
		}
		if (rules[i].p == ALLOW) allowed = 1;
		else allowed = 0;
	}

	return allowed;
}


void Filter::Add(Rule &NewRule)
{
	rules.Add(NewRule);
}


Rule& Filter::Get(int index)
{
	return rules[index];
}


void Filter::Modify(int index, Rule &NewRule)
{
	rules[index] = NewRule;
}


void Filter::SetEnabled(int index, int IsEnabled)
{
	rules[index].enabled = IsEnabled;
}


void Filter::Remove(int index)
{
	rules.RemoveAt(index);
}


void Filter::RemoveAll(void)
{
	rules.RemoveAll();
}


int Filter::CompareMAC(MACaddr& mac1, MACaddr& mac2)
{
	int i;

	for (i=0;i < 6;i++) if (mac1.b[i] != mac2.b[i]) return 1;
	
	return 0;
}


int Filter::CompareIP(IPaddr& ip1, IPaddr& ip2)
{
	int i;

	for (i=0;i < 4;i++) if (ip1.b[i] != ip2.b[i]) return 1;
	
	return 0;
}
