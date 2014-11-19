#include "stdafx.h"
#include "Stats.h"
#include "Switch.h"


Stats::Stats(void)
{
}


Stats::~Stats(void)
{
}


void Stats::Add(int port, DIRECTION FDirection, Frame * f)
{
	int i;
	BOOL direction_found = 0, frametype_found = 0, lay3_found = 0, lay4_found = 0;
	int direction_index = -1, frametype_index = -1, lay3_index = -1, lay4_index = -1;
	int offset = 0;
	Statistic newStat;

	for (i=0;i < table.GetCount();i++)
	{
		if ((!direction_found) && (table[i].HasFrameType < 2)) {
			direction_index = i;
			if ((table[i].port == port) && (table[i].d == FDirection)) direction_found = 1;
		}
		if ((!frametype_found) && (table[i].HasLay3Type < 2)) {
			frametype_index = i;
			if ((table[i].port == port) && (table[i].d == FDirection)
				&& (table[i].FrameType == f->GetType())) {
					frametype_found = 1;
					if (table[i].FrameType != ETH2) break;
			}
		}
		if ((!lay3_found) && (table[i].HasLay4Type < 2)) {
			lay3_index = i;
			if ((table[i].port == port) && (table[i].d == FDirection)
				&& (table[i].FrameType == f->GetType()) && (table[i].Lay3Type == f->GetLay3Type())) {
					lay3_found = 1;
					if (table[i].Lay3Type != 0x0800) break;
			}
		}
		if (!lay4_found) {
			lay4_index = i;
			if ((table[i].port == port) && (table[i].d == FDirection)
				&& (table[i].FrameType == f->GetType()) && (table[i].Lay3Type == f->GetLay3Type())
				&& (table[i].Lay4Type == f->GetLay4Type())) {
					lay4_found = 1;
					break;
			}
		}
	}

	if (direction_found) {
		newStat = table[direction_index];
		table[direction_index].count++;
		theApp.GetSwitchDlg()->UpdateStat(direction_index,table[direction_index].count);
	}
	else {
		newStat.port = port;
		newStat.d = FDirection;
		newStat.HasFrameType = 1;
		newStat.HasLay3Type = 1;
		newStat.HasLay4Type = 1;
		newStat.count = 1;
		direction_index++;
		table.InsertAt(direction_index,newStat);
		theApp.GetSwitchDlg()->InsertStat(direction_index,newStat);
		offset++;
	}

	frametype_index += offset;
	if (frametype_found) {
		newStat = table[frametype_index];
		table[frametype_index].count++;
		theApp.GetSwitchDlg()->UpdateStat(frametype_index,table[frametype_index].count);
	}
	else {
		newStat.FrameType = f->GetType();
		newStat.HasFrameType = 2;
		if (newStat.FrameType == ETH2) {
			newStat.HasLay3Type = 1;
			newStat.HasLay4Type = 1;
		}
		else {
			newStat.HasLay3Type = 0;
			newStat.HasLay4Type = 0;
		}
		newStat.count = 1;
		frametype_index++;
		table.InsertAt(frametype_index,newStat);
		theApp.GetSwitchDlg()->InsertStat(frametype_index,newStat);
		offset++;
	}
	if (table[frametype_index].FrameType != ETH2) return;

	lay3_index += offset;
	if (lay3_found) {
		newStat = table[lay3_index];
		table[lay3_index].count++;
		theApp.GetSwitchDlg()->UpdateStat(lay3_index,table[lay3_index].count);
	}
	else {
		newStat.Lay3Type = f->GetLay3Type();
		newStat.HasLay3Type = 2;
		if (newStat.Lay3Type != 0x0800) newStat.HasLay4Type = 0;
		newStat.count = 1;
		lay3_index++;
		table.InsertAt(lay3_index,newStat);
		theApp.GetSwitchDlg()->InsertStat(lay3_index,newStat);
		offset++;
	}
	if (table[lay3_index].Lay3Type != 0x0800) return;

	lay4_index += offset;
	if (lay4_found) {
		table[lay4_index].count++;
		theApp.GetSwitchDlg()->UpdateStat(lay4_index,table[lay4_index].count);
	}
	else {
		newStat.Lay4Type = f->GetLay4Type();
		newStat.HasLay4Type = 2;
		newStat.count = 1;
		lay4_index++;
		table.InsertAt(lay4_index,newStat);
		theApp.GetSwitchDlg()->InsertStat(lay4_index,newStat);
	}
}


void Stats::Reset(void)
{
	table.RemoveAll();
}
