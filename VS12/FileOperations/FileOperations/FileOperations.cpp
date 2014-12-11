// FileOperations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BUFFERSIZE 512
#define FILENAME_W L"myfile.bin"
#define FILENAME_A "myfile.bin"

void DisplayError(LPTSTR lpszFunction);

void createFile(void) {
	FILE * pFile;
	char buffer [BUFFERSIZE];
	pFile = fopen (FILENAME_A, "wb");
	if (pFile == NULL) perror ("Error opening file");
	else
	{
		fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
		fclose (pFile);
	}
}

int main(int argc, _TCHAR* argv[])
{
	HANDLE hFile;
	BOOL bResult   = FALSE;                 // results flag
	DWORD junk     = 0;                     // discard results
	DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
	TCHAR fileName[] = FILENAME_W;
	char   ReadBuffer[BUFFERSIZE] = {0};
	DWORD  bytesRead;
	
	createFile();
	//SleepEx(5000, TRUE);

    hFile = CreateFile(fileName,               // file to open
                       GENERIC_READ,          // open for reading
                       0,       // do not share
                       NULL,                  // default security
                       OPEN_EXISTING,         // existing file only
                       FILE_ATTRIBUTE_NORMAL, // normal file
                       NULL);                 // no attr. template
 
    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        DisplayError(TEXT("CreateFile"));
        _tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), argv[1]);
        return 1; 
    }

	bResult = DeviceIoControl(hFile,                       // device to be queried
                            IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, // operation to perform
                            NULL, 0,                       // no input buffer
                            &pdg, sizeof(pdg),            // output buffer
                            &junk,                         // # bytes returned
                            (LPOVERLAPPED) NULL);          // synchronous I/O

	if( FALSE == ReadFile(hFile, ReadBuffer, BUFFERSIZE, &bytesRead, NULL) )
    {
        DisplayError(TEXT("ReadFile"));
        printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
        CloseHandle(hFile);
        return 1;
    }


   
	return 0;
}

void DisplayError(LPTSTR lpszFunction) 
// Routine Description:
// Retrieve and output the system error message for the last-error code
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, 
        NULL );

    lpDisplayBuf = 
        (LPVOID)LocalAlloc( LMEM_ZEROINIT, 
                            ( lstrlen((LPCTSTR)lpMsgBuf)
                              + lstrlen((LPCTSTR)lpszFunction)
                              + 40) // account for format string
                            * sizeof(TCHAR) );
    
    if (FAILED( StringCchPrintf((LPTSTR)lpDisplayBuf, 
                     LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                     TEXT("%s failed with error code %d as follows:\n%s"), 
                     lpszFunction, 
                     dw, 
                     lpMsgBuf)))
    {
        printf("FATAL ERROR: Unable to output error code.\n");
    }
    
    _tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

