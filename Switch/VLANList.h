#pragma once
#include "afxtempl.h"

struct VLAN {
	int VID;
	char AllowedForTrunk1;
	char AllowedForTrunk2;
};

class VLANList
{
public:
	VLANList(void);
	~VLANList(void);
private:
	CArray<VLAN> VIDtable;
public:
	int Add(int vlan_id, int trunk1, int trunk2);
	void UpdateAllowedVLANs(int index, int TrunkPortID, int IsAllowed);
	int IsAllowedForTrunk(int vlan_id, int TrunkPortID);
	int IsAllowedForTrunks(int vlan_id);
	int GetVID(int index);
	void Remove(int index);
};

