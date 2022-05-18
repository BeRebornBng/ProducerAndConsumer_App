#include "stdfax.h"


int APIENTRY _tWinMain(HINSTANCE hInst,
	HINSTANCE Prev,
	LPTSTR cmd,
	int mode)
{
	WNDCLASS wc = NewWindowClass
	(
		(HBRUSH)COLOR_WINDOW,
		LoadCursor(NULL, IDC_HAND),
		hInst,
		LoadIcon(hInst, MAKEINTRESOURCE(IDC_ICON)),
		_T("ChildClass"),
		WIN32PROC
	);

	if (!RegisterClass(&wc)) { return -1; }

	HWND hWnd = CreateWindow
	(
		_T("ChildClass"),
		_T("Producer"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		300, 100, 350, 340,
		NULL, NULL, hInst, NULL
	);
	ShowWindow(hWnd, mode); // Показать окно
	UpdateWindow(hWnd);
	MSG msg;
	while (1)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}
	}
	return 0;
}


WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR NAME, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };
	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = NAME;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}


void GenerateText(char *textbuff) {
	strcpy_s(textbuff, 7, ("данные"));
}


void AddTextToFile(char *textbuff) {
	char buffer[5000];
	buffer[0] = ('\0');
	ifstream readFile(Path);
	readFile.getline(buffer, 5000);
	readFile.close();
	ofstream file(Path);
	if (buffer[0] == ('\0')) {
		file << textbuff;
	}
	else {
		file << buffer;
		file << "\r";
		file << textbuff;
	}
	file.close();
}


void ProcessProducer(HWND hWnd,int time) {
	COPYDATASTRUCT cds;
	TCHAR sendmsg[256], msg[256];
	char buf[10];
	GenerateText(buf);
	WaitForSingleObject(Empty, INFINITE);
	WaitForSingleObject(Mutex, INFINITE);
	AddTextToFile(buf);
	_itot_s(ProcID, msg, 10);
	cds.dwData = NULL;
	cds.cbData = _tcsnlen(sendmsg, 256) + 1;
	_tcscpy_s(sendmsg, 256, msg);
	cds.lpData = sendmsg;
	//SetWindowText(hWnd, sendmsg);
	hWindow = FindWindow(_T("MainhWndClass"), _T("Решение проблемы производителей и потребителей"));
	SendMessageW(hWindow, WM_COPYDATA, (WPARAM)&cds, (LPARAM)&cds);
	ReleaseMutex(Mutex);
	ReleaseSemaphore(Full, 1, NULL);
}


LRESULT CALLBACK WIN32PROC(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	//COPYDATASTRUCT cds;
	TCHAR msg[256];
	int time=0;
	switch (message)
	{
	case WM_CREATE:
		Full = CreateSemaphore(NULL, 0, N, _T("SemFull"));
		Mutex = CreateMutex(NULL, false, _T("MutCrit"));
		Empty = CreateSemaphore(NULL, N, N, _T("SemEmpty"));
		break;
	case WM_SIZE:
		ShowWindow(hWnd, SW_HIDE);
		SetForegroundWindow(hWnd);
		GetWindowThreadProcessId(hWnd, &ProcID);
		break;
	case WM_COPYDATA:
		_tcscpy_s(msg, 256, (TCHAR*)(((PCOPYDATASTRUCT)lParam)->lpData));
		time = _tstoi(msg);
		TCHAR sendmsg[256], msg[256];
		SetTimer(hWnd, IDC_TIMER, time, (TIMERPROC)NULL);
		break;
	case WM_TIMER:
		ProcessProducer(hWnd,time);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, IDC_TIMER);
		CloseHandle(Full);
		CloseHandle(Mutex);
		CloseHandle(Empty);
		PostQuitMessage(0);
		DestroyWindow(hWnd);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}