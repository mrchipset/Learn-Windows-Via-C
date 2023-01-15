#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	LPWSTR cCurrentDir;
	const size_t STR_LEN = 512;
	cCurrentDir = (LPWSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(LPSTR) * STR_LEN);
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	//LPWSTR cmd = TEXT("DemoProcess1.exe arg1 arg2=3");
	LPWSTR cmd = TEXT("cmd.exe");
	GetCurrentDirectory(MAX_PATH, cCurrentDir);
	errno_t errn = wcscat_s(cCurrentDir, STR_LEN, TEXT("\\..\\..\\..\\x64\\Debug\\DemoProcess1.exe a b c d"));
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = INVALID_HANDLE_VALUE;
	if ( !CreateProcess(NULL,
			cCurrentDir,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi)
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return -1;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	// Get exit code.
	DWORD exitCode;
	if (GetExitCodeProcess(pi.hProcess, &exitCode))
	{
		printf("Process exit code: %d\n", exitCode);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}