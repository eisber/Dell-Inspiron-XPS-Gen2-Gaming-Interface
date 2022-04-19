// $Id$
// eisber@eisber.net
// 
// Feel free to use the code, but I dont take any responsiblity if you damage your machine with it ;)
//
// If you create something nice, e.g. winamp plugin,  let me know.

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <conio.h>

enum Colors { Ruby=1, Citrine, Amber, Peridot, Emerald, Jade, Topaz, Tanzanite, Aquamarine, Sapphire, Iolite, Amythest, Kunzite, Rhodolite, Coral, Diamond};
// indexes in input array
enum LED { Fan=4, Speakers, PanelBack, Brightness };

int main(int argc, char* argv[])
{
	// Starting from 01
	// 0, 0, 0, 0 ... OFF
	// [4] == Fans             1 ... 0x10
	// [5] == Speakers
	// [6] == Panel Back Color
	// [7] == Brightness       0 ... 7
	unsigned char input[44] =
	{04, 00, 06, 00, 01, 01, 01, 07, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00};
	
	unsigned char output[44];
	memset(output, 0, sizeof(output));

	HANDLE hDevice;               // handle to the drive to be examined 
	BOOL bResult;                 // results flag
	DWORD junk;                   // discard results

	hDevice = CreateFile("\\\\.\\APPDRV",  // driver to open
						GENERIC_READ|GENERIC_WRITE,                // no 
						0, // share mode
						NULL,             // default security attributes
						OPEN_EXISTING,    // disposition
						0,                // file attributes
						NULL);            // do not copy file attributes

	if (hDevice == INVALID_HANDLE_VALUE) // cannot open device
	{
		printf("Could not load device driver\n");
		getch();
		return -1;
	}

	int color = 1;
	while(1)
	{
		input[Speakers] = color++%0x10;
		input[Brightness] = color%8;
		bResult = DeviceIoControl(hDevice,  // device to be queried
			0x22209C,  // operation to perform
			input, 44, // no input buffer
			output, 44,     // output buffer
			&junk,                 // # bytes returned
			(LPOVERLAPPED) NULL);  // synchronous I/O
		if(bResult)
		{
			printf(".\n");
		}
		else
		{
			printf ("Error %ld.\n", GetLastError ());
		}
		Sleep(100);
	}
		
	CloseHandle(hDevice);

	getch();
	return 0;
}
