#include "stdafx.h"
#include "SwitchPort.h"


PIP_ADAPTER_ADDRESSES SwitchPort::AdaptersInfoBuffer = NULL;


SwitchPort::SwitchPort(int index)
	: MACAddress(_T("unknown"))
	, PortIndex(index)
{
	int i;

	Mode = ACCESS;
	VLAN_ID = 1;
	for (i=0;i < 6;i++) MACAddrStruct.b[i] = 0x00;
	FrameBuffer = new Frame();
}


SwitchPort::~SwitchPort(void)
{
}


CStringA SwitchPort::GetName(void)
{
	return PortName;
}


CString SwitchPort::GetDescription(void)
{
	return PortDescription;
}


void SwitchPort::SetName(CStringA name)
{
	PortName = name;
}


void SwitchPort::SetName(PSTR name)
{
	PortName.SetString(name);
}


void SwitchPort::SetDescription(CString desc)
{
	PortDescription = desc;
}


void SwitchPort::SetDescription(PSTR desc)
{
	PortDescription = CString(desc);
}


void SwitchPort::SetDescription(PWSTR desc)
{
	PortDescription.SetString(desc);
}


CString SwitchPort::GetMACAddress(void)
{
	DWORD dwRetVal = NO_ERROR;
	int i = 0;
	ULONG InfoBufLength = 10000;
	ULONG Iterations = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_SKIP_UNICAST | GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;
	
	PIP_ADAPTER_ADDRESSES CurrentAdapter;
	
	if (!AdaptersInfoBuffer) do {
		AdaptersInfoBuffer = (IP_ADAPTER_ADDRESSES *) malloc(InfoBufLength);
		
		dwRetVal = GetAdaptersAddresses(AF_INET, flags, NULL, AdaptersInfoBuffer, &InfoBufLength);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			free(AdaptersInfoBuffer);
			AdaptersInfoBuffer = NULL;
		} else {
			break;
		}

		Iterations++;
	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 3));

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		CurrentAdapter = AdaptersInfoBuffer;
		MACAddress.SetString(_T("unknown"));
		while (CurrentAdapter) {
			if (PortName.Find(CurrentAdapter->AdapterName) != -1) {
				SetDescription(CurrentAdapter->Description);
				MACAddress.Empty();
				for (i=0;i < (int)CurrentAdapter->PhysicalAddressLength;i++) {
					if (i == (CurrentAdapter->PhysicalAddressLength - 1)) MACAddress.AppendFormat(_T("%.2X"),(int)CurrentAdapter->PhysicalAddress[i]);
					else MACAddress.AppendFormat(_T("%.2X-"),(int)CurrentAdapter->PhysicalAddress[i]);
					if (i < 6) MACAddrStruct.b[i] = (int)CurrentAdapter->PhysicalAddress[i];
				}
				break;
			}
			CurrentAdapter = CurrentAdapter->Next;
		}
	} else {
		if (AdaptersInfoBuffer) free(AdaptersInfoBuffer);
		AdaptersInfoBuffer = NULL;
	}
	
	return MACAddress;
}


int SwitchPort::GetIndex(void)
{
	return PortIndex;
}


void SwitchPort::SetMode(PORT_MODE mode)
{
	Mode = mode;
}


PORT_MODE SwitchPort::GetMode(void)
{
	return Mode;
}


WORD SwitchPort::GetVID(void)
{
	return (WORD) VLAN_ID;
}


void SwitchPort::SetVID(int vid)
{
	VLAN_ID = vid;
}


MACaddr SwitchPort::GetMACAddrStruct(void)
{
	return MACAddrStruct;
}


Frame * SwitchPort::GetBuffer(void)
{
	return FrameBuffer;
}
