#pragma once
#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <filesystem>
using std::ofstream;


#define OnMenuHideAndShowClicked		1//скрыть/показать
#define OnMenuExit						3//завершение работы софта


#define OnButtonClickClear				4//очистить дочерние окна от текста
#define OnButtonClickStart				5//запустить процесс софта
#define OnButtonClickStop				6//остановить процесс софта


#define BUFFSIZE						9//размер буфера

TCHAR buffer[BUFFSIZE];//хранит количество элементов в буфере



const int N = 5, M = 3;//количество запускаемых процессов Producer и Consumer
bool FlagShowWindow = false;//дл€ отображени€ окна(скрыть\показать)
const double K = 0.8, L = 1.0;//коэффициент дл€ передачи времени вызываемым процессам
int numOfElementBuffer = 0;//количество элементов в буфере



struct MSG_Prod_Cons
{
	TCHAR** ConsumerMSG = nullptr;
	TCHAR** ProducerMSG = nullptr;
	TCHAR msgProducer[256];
	TCHAR msgConsumer[256];
	int ProducerNumber[N];
	int ConsumerNumber[M];
	MSG_Prod_Cons() {
		for (int i = 0; i < N; ++i) {
			ProducerNumber[i] = 0;
		}
		for (int i = 0; i < M; ++i) {
			ConsumerNumber[i] = 0;
		}
		ProducerMSG = new TCHAR * [N + 1];
		for (int i = 0; i < N + 1; ++i) {
			ProducerMSG[i] = new TCHAR[256];
		}
		ConsumerMSG = new TCHAR * [M + 1];
		for (int i = 0; i < M + 1; ++i) {
			ConsumerMSG[i] = new TCHAR[256];
		}
		_tcscpy_s(ProducerMSG[0], 256, _T("Producers"));
		_tcscpy_s(ConsumerMSG[0], 256, _T("Consumers"));
		for (int i = 1; i < N + 1; ++i) {
			_tcscpy_s(ProducerMSG[i], 256, _T(""));
		}
		for (int i = 1; i < M + 1; ++i) {
			_tcscpy_s(ConsumerMSG[i], 256, _T(""));
		}
	}
	~MSG_Prod_Cons() {
		for (int i = 0; i < N + 1; ++i) {
			delete[]ProducerMSG[i];
		}
		delete ProducerMSG;
		for (int i = 0; i < M + 1; ++i) {
			delete[]ConsumerMSG[i];
		}
		delete ConsumerMSG;
	}
	TCHAR* ProducerWindowMsg(int index, DWORD pId, TCHAR* msg) {
		++ProducerNumber[index];
		TCHAR number[256];
		TCHAR Producernumber[256];
		_itot_s(index + 1, number, 256, 10);
		_itot_s(ProducerNumber[index], Producernumber, 256, 10);
		if (ProducerMSG[index + 1] == _T("")) {
			_tcscat_s(ProducerMSG[index + 1], 256, number);
			_tcscat_s(ProducerMSG[index + 1], 256, (TCHAR*)("-"));
			_tcscat_s(ProducerMSG[index + 1], 256, msg);
			_tcscat_s(ProducerMSG[index + 1], 256, _T("  "));
			_tcscat_s(ProducerMSG[index + 1], 256, Producernumber);
		}
		else {
			_tcscpy_s(ProducerMSG[index + 1], 256, _T(""));
			_tcscat_s(ProducerMSG[index + 1], 256, number);
			_tcscat_s(ProducerMSG[index + 1], 256, (TCHAR*)("-"));
			_tcscat_s(ProducerMSG[index + 1], 256, msg);
			_tcscat_s(ProducerMSG[index + 1], 256, _T("  "));
			_tcscat_s(ProducerMSG[index + 1], 256, Producernumber);
		}
		return ProducerMSG[index + 1];
	}
	TCHAR* ConsumerWindowMsg(int index, DWORD pId, TCHAR* msg) {
		++ConsumerNumber[index];
		TCHAR number[256];
		TCHAR Consumernumber[256];
		_itot_s(ConsumerNumber[index], Consumernumber, 256, 10);
		_itot_s(index + 1, number, 256, 10);
		if (ConsumerMSG[index + 1] == _T("")) {
			_tcscat_s(ConsumerMSG[index + 1], 256, number);
			_tcscat_s(ConsumerMSG[index + 1], 256, (TCHAR*)("-"));
			_tcscat_s(ConsumerMSG[index + 1], 256, msg);
			_tcscat_s(ConsumerMSG[index + 1], 256, _T("  "));
			_tcscat_s(ConsumerMSG[index + 1], 256, Consumernumber);
		}
		else {
			_tcscpy_s(ConsumerMSG[index + 1], 256, _T(""));
			_tcscat_s(ConsumerMSG[index + 1], 256, number);
			_tcscat_s(ConsumerMSG[index + 1], 256, (TCHAR*)("-"));
			_tcscat_s(ConsumerMSG[index + 1], 256, msg);
			_tcscat_s(ConsumerMSG[index + 1], 256, _T("  "));
			_tcscat_s(ConsumerMSG[index + 1], 256, Consumernumber);
		}
		return ConsumerMSG[index + 1];
	}
	void CreateStrProducer() {
		_tcscpy_s(msgProducer, 256, _T(""));
		for (int i = 0; i < N + 1; ++i) {
			_tcscat_s(msgProducer, 256, ProducerMSG[i]);
			_tcscat_s(msgProducer, 256, _T("\r\n"));
		}
	}
	void CreateStrConsumer() {
		_tcscpy_s(msgConsumer, 256, _T(""));
		for (int i = 0; i < M + 1; ++i) {
			_tcscat_s(msgConsumer, 256, ConsumerMSG[i]);
			_tcscat_s(msgConsumer, 256, _T("\r\n"));
		}
	}
	void Clear() {
		for (int i = 0; i < N; ++i) {
			ProducerNumber[i] = 0;
		}
		for (int i = 0; i < M; ++i) {
			ConsumerNumber[i] = 0;
		}
		for (int i = 1; i < N + 1; ++i) {
			_tcscpy_s(ProducerMSG[i], 256, _T(""));
		}
		for (int i = 1; i < M + 1; ++i) {
			_tcscpy_s(ConsumerMSG[i], 256, _T(""));
		}
	}
};
MSG_Prod_Cons MSGPC;//храним текст обоих дочерних окон



HINSTANCE hInst;//хранит обработчик экземпл€ра или точку вхождени€
				//ќперационна€ система использует это значение дл€
				//задани€ исполн€емого файла (EXE) при его загрузке в пам€ть
HWND hEditControlOne;//дочернее окно Producers
HWND hEditControlTwo;//дочернее окно Consumers
HWND hEditControlThree;//дочернее окно NumberOfElementBuffer
HWND hWindow;//дл€ хранени€ хэндла окон



DWORD ProcID;//хранит ID процесса
PROCESS_INFORMATION Producers[N], Consumers[M];//процессы



const TCHAR* Path = _T("..\\Buffer.txt");//относительный путь
const TCHAR* PathProducer = _T("..\\Producer\\x64\\Debug\\Producer.exe");//относительный путь
const TCHAR* PathConsumer = _T("..\\Consumer\\x64\\Debug\\Consumer.exe");//относительный путь



WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR NAME, WNDPROC Procedure);//регистрируем экземпл€р
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//обратна€ функци€ или процедура дл€ обработки сообщений системы
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);//обратна€ функци€ дл€ установлени€ хэндла вызываемого процесса



void MainWndAddMenu(HWND hWnd);//добавление меню
void MainWndAddWidgets(HWND hWnd);//добавление виджетов
void StartProcess();//запустить процесс
void DropProcess();//остановить процесс
void ClearFile();//чистим буфер
void CreateTrayIcon(HWND hWnd);//создание иконки дл€ сворачивани€ в трей
int OnTray(HWND hWnd, WPARAM wParam, LPARAM lParam);//обработка сообщений системы дл€ иконки в трее
void DropTrayIcon(HWND hWnd);//дроп трей иконки