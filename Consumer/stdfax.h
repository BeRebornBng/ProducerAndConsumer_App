#pragma once
#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;

#define IDC_TIMER 123

HANDLE Full, Mutex, Empty;
const int N = 8;
DWORD ProcID;
HWND hWindow;

const TCHAR* Path = _T("..\\Buffer.txt");//относительный путь

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR NAME, WNDPROC Procedure);//регистрируем экземпл€р
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//обратна€ функци€ или процедура дл€ обработки сообщений

void ReadToFile(char *buf);


void Consumer(int time);