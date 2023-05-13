#include "Native.h"
#include "Driver.h"
#include <iostream>
#include <map>

VOID PrintHexDump(PUCHAR buffer, ULONG length)
{
    DWORD i, count, index;
    CHAR rgbDigits[] = "0123456789abcdef";
    CHAR rgbLine[100];
    CHAR cbLine;

    for (index = 0; length;
        length -= count, buffer += count, index += count)
    {
        count = (length > 16) ? 16 : length;

        sprintf_s(rgbLine, 100, "%4.4x  ", index);
        cbLine = 6;

        for (i = 0; i < count; i++)
        {
            rgbLine[cbLine++] = rgbDigits[buffer[i] >> 4];
            rgbLine[cbLine++] = rgbDigits[buffer[i] & 0x0f];
            if (i == 7)
            {
                rgbLine[cbLine++] = ':';
            }
            else
            {
                rgbLine[cbLine++] = ' ';
            }
        }
        for (; i < 16; i++)
        {
            rgbLine[cbLine++] = ' ';
            rgbLine[cbLine++] = ' ';
            rgbLine[cbLine++] = ' ';
        }

        rgbLine[cbLine++] = ' ';

        for (i = 0; i < count; i++)
        {
            if (buffer[i] < 32 || buffer[i] > 126)
            {
                rgbLine[cbLine++] = '.';
            }
            else
            {
                rgbLine[cbLine++] = buffer[i];
            }
        }

        rgbLine[cbLine++] = 0;
        printf("%s\n", rgbLine);
    }
}

void DriverTest(CDriver* driver)
{
    CONST ULONG BufferSize = 0x1000;
    UCHAR Buffer[BufferSize] = { 0 };
    

    if (driver->ReadPhysical(0xC0000, Buffer, BufferSize))
    {
        PrintHexDump(Buffer, BufferSize);
    }

}

int main()
{
    SetConsoleTitleW(L"[Vulnerable 29.08.2006] Adv64Drv");

    CDriver driver;
    if (driver.Load())
    {
        DriverTest(&driver);
        driver.Unload();
    }

    printf("press [Enter] to close the console");
    getchar();
    return 0;
}