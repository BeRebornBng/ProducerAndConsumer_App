#pragma once
#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <filesystem>
using std::ofstream;


#define OnMenuHideAndShowClicked		1//������/��������
#define OnMenuExit						3//���������� ������ �����


#define OnButtonClickClear				4//�������� �������� ���� �� ������
#define OnButtonClickStart				5//��������� ������� �����
#define OnButtonClickStop				6//���������� ������� �����


#define BUFFSIZE						9//������ ������

TCHAR buffer[BUFFSIZE];//������ ���������� ��������� � ������



const int N = 5, M = 3;//���������� ����������� ��������� Producer � Consumer
bool FlagShowWindow = false;//��� ����������� ����(������\��������)
const double K = 0.8, L = 1.0;//����������� ��� �������� ������� ���������� ���������
int numOfElementBuffer = 0;//���������� ��������� � ������



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
MSG_Prod_Cons MSGPC;//������ ����� ����� �������� ����



HINSTANCE hInst;//������ ���������� ���������� ��� ����� ���������
				//������������ ������� ���������� ��� �������� ���
				//������� ������������ ����� (EXE) ��� ��� �������� � ������
HWND hEditControlOne;//�������� ���� Producers
HWND hEditControlTwo;//�������� ���� Consumers
HWND hEditControlThree;//�������� ���� NumberOfElementBuffer
HWND hWindow;//��� �������� ������ ����



DWORD ProcID;//������ ID ��������
PROCESS_INFORMATION Producers[N], Consumers[M];//��������



const TCHAR* Path = _T("..\\Buffer.txt");//������������� ����
const TCHAR* PathProducer = _T("..\\Producer\\x64\\Debug\\Producer.exe");//������������� ����
const TCHAR* PathConsumer = _T("..\\Consumer\\x64\\Debug\\Consumer.exe");//������������� ����



WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR NAME, WNDPROC Procedure);//������������ ���������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//�������� ������� ��� ��������� ��� ��������� ��������� �������
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);//�������� ������� ��� ������������ ������ ����������� ��������



void MainWndAddMenu(HWND hWnd);//���������� ����
void MainWndAddWidgets(HWND hWnd);//���������� ��������
void StartProcess();//��������� �������
void DropProcess();//���������� �������
void ClearFile();//������ �����
void CreateTrayIcon(HWND hWnd);//�������� ������ ��� ������������ � ����
int OnTray(HWND hWnd, WPARAM wParam, LPARAM lParam);//��������� ��������� ������� ��� ������ � ����
void DropTrayIcon(HWND hWnd);//���� ���� ������