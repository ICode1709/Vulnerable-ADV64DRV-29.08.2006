#pragma once

class CDriver
{
public:
	CDriver();
	VIRTUAL ~CDriver();

	VIRTUAL BOOLEAN Load();
	VIRTUAL VOID Unload();

	VIRTUAL BOOLEAN ReadPhysical(ULONGLONG PhysicalAddress, PVOID Buffer, ULONG BufferSize);
private:
	HANDLE DeviceHandle;
};