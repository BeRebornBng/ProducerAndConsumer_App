#pragma once
#include <windows.h>
#include <tchar.h>
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::wifstream;
using std::wofstream;

#define IDC_TIMER 123

bool EncondingFlag = false;
const int N = 8;

HANDLE Mutex, Full, Empty;
HWND hWindow;
DWORD ProcID;

const TCHAR* Path = _T("..\\Buffer.txt");//относительный путь


WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR NAME, WNDPROC Procedure);//регистрируем экземпл€р
LRESULT CALLBACK WIN32PROC(HWND, UINT, WPARAM, LPARAM);//обратна€ функци€ или процедура дл€ обработки сообщений


void GenerateText(char* textbuff);
void AddTextToFile(char* textbuff);
void ProcessProducer(HWND hWnd,int time);