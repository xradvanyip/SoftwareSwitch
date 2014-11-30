#include "stdafx.h"
#include "VLANList.h"


VLANList::VLANList(void)
{
}


VLANList::~VLANList(void)
{
}


int VLANList::Add(int vlan_id, int trunk1, int trunk2)
{
	int i;
	VLAN newVLAN;

	for (i=0;i < VIDtable.GetCount();i++) if (VIDtable[i].VID == vlan_id) return 1;
	
	newVLAN.VID = vlan_id;
	newVLAN.AllowedForTrunk1 = trunk1;
	newVLAN.AllowedForTrunk2 = trunk2;
	VIDtable.Add(newVLAN);
	return 0;
}


void VLANList::UpdateAllowedVLANs(int index, int TrunkPortID, int IsAllowed)
{
	if (TrunkPortID == 1) VIDtable[index].AllowedForTrunk1 = IsAllowed;
	else VIDtable[index].AllowedForTrunk2 = IsAllowed;
}


int VLANList::IsAllowedForTrunk(int vlan_id, int TrunkPortID)
{
	int i;

	for (i=0;i < VIDtable.GetCount();i++) if (VIDtable[i].VID == vlan_id)
	{
		if ((TrunkPortID == 1) && (VIDtable[i].AllowedForTrunk1 == 1)) return 1;
		if ((TrunkPortID == 2) && (VIDtable[i].AllowedForTrunk2 == 1)) return 1;
		break;
	}
	
	return 0;
}


int VLANList::IsAllowedForTrunks(int vlan_id)
{
	int i;

	for (i=0;i < VIDtable.GetCount();i++) if (VIDtable[i].VID == vlan_id)
	{
		if ((VIDtable[i].AllowedForTrunk1 == 1) && (VIDtable[i].AllowedForTrunk2 == 1)) return 1;
		break;
	}
	
	return 0;
}


int VLANList::GetVID(int index)
{
	return VIDtable[index].VID;
}


void VLANList::Remove(int index)
{
	VIDtable.RemoveAt(index);
}
