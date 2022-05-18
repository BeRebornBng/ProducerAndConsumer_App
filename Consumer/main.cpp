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
		_T("ChildClassConsumer"),
		WndProc
	);

	if (!RegisterClass(&wc)) { return -1; }

	HWND hWnd = CreateWindow
	(
		_T("ChildClassConsumer"),
		_T("Consumer"),
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	TCHAR msg[256];
	int time = 0;
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
		hWindow = FindWindow(_T("MainhWndClass"), _T("Решение проблемы производителей и потребителей"));
		GetWindowThreadProcessId(hWnd, &ProcID);
		break;
	case WM_COPYDATA:
		_tcscpy_s(msg, 256, (TCHAR*)(((PCOPYDATASTRUCT)lParam)->lpData));
		time = _tstoi(msg);
		SetTimer(hWnd, IDC_TIMER, time, (TIMERPROC)NULL);
		break;
	case WM_TIMER:
		Consumer(time);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		CloseHandle(Full);
		CloseHandle(Mutex);
		CloseHandle(Empty);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


void ReadToFile(char *buf) {

	setlocale(LC_ALL, "ru");
	ifstream readFile{ Path };
	string str;
	char ch[5000];
	while (1) {
		readFile.getline(ch, 5000,'\r');
		if (readFile.eof())break;
		str += ch;
		str.push_back('\r');
	}
	if(!str.empty()) str.pop_back();
	readFile.close();
	ofstream writeFile{ Path };
	writeFile << str;
	writeFile.close();
}


void Consumer(int time) {
	char buf[256];
	COPYDATASTRUCT cds;
	TCHAR sendmsg[256], msg[256];
	WaitForSingleObject(Full, INFINITE);
	WaitForSingleObject(Mutex, INFINITE);
	ReadToFile(buf);
	_itot_s(ProcID, msg, 10);
	cds.dwData = NULL;
	cds.cbData = _tcsnlen(sendmsg, 256) + 1;
	_tcscpy_s(sendmsg, 256, msg);
	cds.lpData = sendmsg;
	SendMessageW(hWindow, WM_COPYDATA, (WPARAM)&cds, (LPARAM)&cds);
	ReleaseMutex(Mutex);
	ReleaseSemaphore(Empty, 1, NULL);
}