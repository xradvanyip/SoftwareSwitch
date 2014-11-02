#pragma once
#pragma comment(lib, "iphlpapi.lib")

#include <IPHlpApi.h>

typedef enum {ACCESS, TRUNK} PORT_MODE;

class SwitchPort
{
public:
	SwitchPort(int index);
	~SwitchPort(void);
private:
	int PortIndex;
	CStringA PortName;
	CString PortDescription;
	CString MACAddress;
	PORT_MODE Mode;
	static PIP_ADAPTER_ADDRESSES AdaptersInfoBuffer;
public:
	CStringA GetName(void);
	CString GetDescription(void);
	void SetName(CStringA name);
	void SetName(PSTR name);
	void SetDescription(CString desc);
	void SetDescription(PSTR desc);
	void SetDescription(PWSTR desc);
	CString GetMACAddress(void);
	int GetIndex(void);
	void SetMode(PORT_MODE mode);
	PORT_MODE GetMode(void);
};

