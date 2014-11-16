#include "stdafx.h"
#include "MACtable.h"
#include "Switch.h"


MACtable::MACtable(void)
	: TimeOut(DEF_TIMEOUT_SEC)
{
}


MACtable::~MACtable(void)
{
}


void MACtable::Add(int port, MACaddr address)
{
	StoredMAC NewMAC;
	
	NewMAC.port = port;
	NewMAC.address = address;
	NewMAC.SecondsLeft = TimeOut;
	TableEntry.Add(NewMAC);
	theApp.GetSwitchDlg()->InstertToMACTab(NewMAC);
}


int MACtable::Find(int port_in, MACaddr src, MACaddr dest)
{
	int i, dest_port = -1;
	BOOL src_found = 0, dest_found = 0;

	for (i=0;i < TableEntry.GetCount();i++)
	{
		if ((!src_found) && (CompareMAC(src,TableEntry[i].address) == 0))
		{
			if (TableEntry[i].port != port_in) {
				TableEntry[i].port = port_in;
				theApp.GetSwitchDlg()->ModifyMACTab(i,TableEntry[i]);
			}
			TableEntry[i].SecondsLeft = TimeOut;
			theApp.GetSwitchDlg()->UpdateTimeout(i,TimeOut);
			src_found = 1;
		}
		if ((!dest_found) && (CompareMAC(dest,TableEntry[i].address) == 0))
		{
			dest_port = TableEntry[i].port;
			dest_found = 1;
		}
	}

	if (!src_found) Add(port_in,src);
	
	return dest_port;
}


int MACtable::CompareMAC(MACaddr& mac1, MACaddr& mac2)
{
	int i;

	for (i=0;i < 6;i++) if (mac1.b[i] != mac2.b[i]) return 1;
	
	return 0;
}


void MACtable::Maintain(void)
{
	int i=0;

	while (i < TableEntry.GetCount())
	{
		TableEntry[i].SecondsLeft -= 5;
		theApp.GetSwitchDlg()->UpdateTimeout(i,TableEntry[i].SecondsLeft);
		if (TableEntry[i].SecondsLeft == 0) {
			TableEntry.RemoveAt(i);
			theApp.GetSwitchDlg()->DeleteFromMACTab(i);
		}
		else i++;
	}
}


UINT MACtable::GetTimeOut(void)
{
	return TimeOut;
}


void MACtable::SetTimeOut(UINT seconds)
{
	TimeOut = seconds;
}


int MACtable::IsBroadcast(MACaddr& address)
{
	int i;

	for (i=0;i < 6;i++) if (address.b[i] != 0xFF) return 0;
	
	return 1;
}
