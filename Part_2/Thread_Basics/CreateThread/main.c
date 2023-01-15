#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#define MAX_THREADS		3
#define BUF_SIZE		255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void ErrorHandler(LPCTSTR lpszFunction);

typedef struct MyDATA {
	int val1;
	int val2;
} MYDATA, *PMYDATA;

int _tmain()
{
	PMYDATA pDataArray[MAX_THREADS];
	DWORD	dwThreadIdArray[MAX_THREADS];
	HANDLE	hThreadArray[MAX_THREADS];

	for (int i = 0; i < MAX_THREADS; ++i) {
		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
		if (pDataArray[i] == NULL)
		{
			// If allocation failed, system is out of memory
			ExitProcess(2);
		}

		pDataArray[i]->val1 = i;
		pDataArray[i]->val2 = i + 100;

		hThreadArray[i] = CreateThread(
			NULL,
			0,
			MyThreadFunction,
			pDataArray[i],
			0,
			&dwThreadIdArray[i]
		);

		// check create success
		if (hThreadArray[i] == NULL) {
			ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	}
	
	// Wait thread finish
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.
	for (int i = 0; i < MAX_THREADS; ++i) {
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;
		}
	}

	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	HANDLE hStdout;
	PMYDATA pDataArray;

	TCHAR msgBuff[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	pDataArray = (PMYDATA)lpParam;
	// Print the parameter values using thread-safe functions.
	StringCchPrintf(msgBuff, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
		pDataArray->val1, pDataArray->val2);
	StringCchLength(msgBuff, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuff, (DWORD)cchStringSize, &dwChars, NULL);
	return 0;
}

void ErrorHandler(LPCTSTR lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDispBuf;
	DWORD dw = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL );

	// Display error message
	lpDispBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDispBuf,
		LocalSize(lpDispBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDispBuf, TEXT("ERROR"), MB_OK);
	LocalFree(lpMsgBuf);
	LocalFree(lpDispBuf);
}