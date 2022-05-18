#include "stdfax.h"
#include "resource.h"

int APIENTRY _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode)
{

	hInst = This;

	WNDCLASS wc = NewWindowClass
	(
		(HBRUSH)COLOR_WINDOW,
		LoadCursor(NULL, IDC_ARROW),
		This,
		LoadIcon(This, MAKEINTRESOURCE(IDI_ICON1)),
		_T("MainhWndClass"),
		WndProc
	);

	if (!RegisterClass(&wc)) { return -1; }

	HWND hWnd = CreateWindow
	(
		_T("MainhWndClass"),
		_T("Решение проблемы производителей и потребителей"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		300, 100, 350, 340,
		NULL, NULL, This, NULL
	);
	

	hWindow = hWnd;//запоминаем дескриптор главного окна
	ShowWindow(hWnd, mode); // Показать окно
   // Цикл обработки сообщений
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{ // Обработчик сообщений
	TCHAR msg[256];
	DWORD procID;
	int i = 0;
	TCHAR bufmsg[256] = _T("Элементов в буфере: ");
	switch (message)
	{
	case WM_CREATE:
		MainWndAddMenu(hWnd);
		MainWndAddWidgets(hWnd);
		CreateTrayIcon(hWnd);
		break;
	case WM_COMMAND:
		switch (wParam) {
			//меню
		case OnMenuHideAndShowClicked:
			if (FlagShowWindow) {
				ShowWindow(hWnd, SW_SHOW);
				ShowWindow(hWnd, SW_RESTORE);
				FlagShowWindow = false;
			}
			else {
				ShowWindow(hWnd, SW_MINIMIZE);
				ShowWindow(hWnd, SW_HIDE);
				FlagShowWindow = true;
			}
			SetForegroundWindow(hWnd);
			break;
		case OnMenuExit:
			DropProcess();
			DestroyWindow(hWnd);
			break;
			//кнопки
		case OnButtonClickClear:
			SetWindowText(hEditControlOne, MSGPC.ProducerMSG[0]);
			SetWindowText(hEditControlTwo, MSGPC.ConsumerMSG[0]);
			SetWindowText(hEditControlThree, _T("Элементов в буфере: 0"));
			numOfElementBuffer = 0;
			MSGPC.Clear();
			ClearFile();
			break;
		case OnButtonClickStart:
			DropProcess();
			numOfElementBuffer = 0;
			MSGPC.Clear();
			StartProcess();
			break;
		case OnButtonClickStop:
			DropProcess();
			numOfElementBuffer = 0;
			MSGPC.Clear();
			break;
		default:
			break;
		}
		break;
	case WM_COPYDATA:
		_tcscpy_s(msg, 256, (TCHAR*)(((PCOPYDATASTRUCT)lParam)->lpData));
		procID = _tstoi(msg);
		for (i = 0; i < N; ++i) {
			if ((Producers[i].dwProcessId) == procID) {
				numOfElementBuffer++;
				MSGPC.ProducerWindowMsg(i, Producers[i].dwProcessId, msg);
				MSGPC.CreateStrProducer();
				SetWindowTextW(hEditControlOne, MSGPC.msgProducer);
				UpdateWindow(hEditControlOne);
				break;
			}
		}
		if (i == N) {
			for (i = 0; i < M; ++i) {
				if ((Consumers[i].dwProcessId) == procID) {
					numOfElementBuffer--;
					MSGPC.ConsumerWindowMsg(i, Consumers[i].dwProcessId, msg);
					MSGPC.CreateStrConsumer();
					SetWindowTextW(hEditControlTwo, MSGPC.msgConsumer);
					UpdateWindow(hEditControlTwo);
					break;
				}
			}
		}
		_itot_s(numOfElementBuffer, buffer, BUFFSIZE, 10);
		_tcscat_s(bufmsg, 256, buffer);
		SetWindowTextW(hEditControlThree, bufmsg);
		UpdateWindow(hEditControlThree);
		break;
	case WM_USER:
		OnTray(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		UpdateWindow(hEditControlOne);
		UpdateWindow(hEditControlTwo);
		UpdateWindow(hEditControlThree);
		UpdateWindow(hWnd);
		break;
	case WM_DESTROY:
		DropProcess();
		DestroyWindow(hWnd);
		DropTrayIcon(hWnd);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



void StartProcess() {
	TCHAR message[256];
	COPYDATASTRUCT cds;

	STARTUPINFO cif[M];

	for (int i = 0; i < M; ++i) {
		ZeroMemory(&cif[i], sizeof(STARTUPINFO));
		CreateProcess(PathConsumer, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &cif[i], &Consumers[i]);

		ProcID = Consumers[i].dwProcessId;
		Sleep(1000);
		EnumWindows(EnumWindowsProc, NULL);

		cds.dwData = 0;
		_itot_s(((i + 1) * L * 1000), message, 256, 10);
		cds.cbData = _tcscnlen(message, 256) + 1;
		cds.lpData = message;
		SendMessage(hWindow, WM_COPYDATA, 0, (LPARAM)&cds);

	}

	STARTUPINFO cif2[N];

	for (int i = 0; i < N; ++i) {

		ZeroMemory(&cif2[i], sizeof(STARTUPINFO));
		CreateProcess(PathProducer, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &cif2[i], &Producers[i]);

		ProcID = Producers[i].dwProcessId;
		Sleep(1000);
		EnumWindows(EnumWindowsProc, NULL);

		cds.dwData = 0;
		_itot_s(((i + 1) * K * 1500), message, 256, 10);
		cds.cbData = _tcscnlen(message, 256) + 1;
		cds.lpData = message;
		SendMessage(hWindow, WM_COPYDATA, 0, (LPARAM)&cds);

	}

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


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{

	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == ProcID)
	{
		hWindow = hwnd;
		return false;
	}
	return true;

}


void DropProcess() {//дропаем процессы
	for (int i = 0; i < N; ++i) {
		TerminateProcess(Producers[i].hProcess, NO_ERROR);
	}
	for (int i = 0; i < M; ++i) {
		TerminateProcess(Consumers[i].hProcess, NO_ERROR);
	}
}


void ClearFile() {
	ofstream file;
	file.open(Path, std::ofstream::out | std::ofstream::trunc);
	file.close();
}


void MainWndAddMenu(HWND hWnd) {

	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();

	AppendMenu(SubMenu, MF_STRING, OnMenuHideAndShowClicked, _T("Скрыть/Показать"));
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnMenuExit, _T("Выход"));
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, _T("Файл"));
	SetMenu(hWnd, RootMenu);

}


void MainWndAddWidgets(HWND hWnd) {

	hEditControlOne = CreateWindow(_T("edit"), (MSGPC.ProducerMSG[0]),
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
		5, 2, 150, 180, hWnd, NULL, NULL, NULL);

	hEditControlTwo = CreateWindow(_T("edit"), (MSGPC.ConsumerMSG[0]),
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL,
		175, 2, 150, 180, hWnd, NULL, NULL, NULL);

	hEditControlThree = CreateWindow(_T("edit"), _T("Элементов в буфере: 0"),
		WS_VISIBLE | WS_CHILD | ES_MULTILINE,
		70, 200, 175, 30, hWnd, NULL, NULL, NULL);

	CreateWindow(_T("button"), _T("Clear"),
		WS_VISIBLE | WS_CHILD | ES_CENTER,
		30, 245, 80, 30, hWnd,
		(HMENU)OnButtonClickClear,
		NULL, NULL);

	CreateWindow(_T("button"), _T("Start"),
		WS_VISIBLE | WS_CHILD | ES_CENTER,
		120, 245, 80, 30, hWnd,
		(HMENU)OnButtonClickStart,
		NULL, NULL);

	CreateWindow(_T("button"), _T("Stop"),
		WS_VISIBLE | WS_CHILD | ES_CENTER,
		210, 245, 80, 30, hWnd,
		(HMENU)OnButtonClickStop,
		NULL, NULL);

}


void CreateTrayIcon(HWND hWnd) {
	NOTIFYICONDATA NID;

	NID.cbSize = sizeof(NOTIFYICONDATA);
	NID.hWnd = hWnd;
	NID.uID = 1;
	NID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	NID.uCallbackMessage = WM_USER;
	NID.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	_tcscpy_s((NID.szTip), _T("Решение задачи производителя и потребителя"));
	Shell_NotifyIcon(NIM_ADD, &NID);
}


int OnTray(HWND hWnd, WPARAM wParam, LPARAM lParam)

{

	switch (lParam)

	{

	case WM_LBUTTONDBLCLK:
		ShowWindow(hWnd, SW_SHOWNORMAL);
		SetForegroundWindow(hWnd);
		if (FlagShowWindow == true)
			FlagShowWindow = false;
		break;
	case WM_RBUTTONDOWN:

		HMENU hMenu = CreatePopupMenu();

		AppendMenu(hMenu, MFT_STRING, OnMenuHideAndShowClicked, _T("Скрыть/Показать"));
		AppendMenu(hMenu, MFT_STRING, OnMenuExit, _T("Выход"));
		SetForegroundWindow(hWnd);
		POINT pt;
		GetCursorPos(&pt);
		TrackPopupMenu(hMenu, TPM_LEFTALIGN,
			pt.x, pt.y, 0, hWnd, NULL);
		break;

	}
	return 0;

}


void DropTrayIcon(HWND hWnd)

{

	NOTIFYICONDATA NID;

	NID.cbSize = sizeof(NOTIFYICONDATA);
	NID.hWnd = hWnd;
	NID.uID = 1;
	Shell_NotifyIcon(NIM_DELETE, &NID);

}