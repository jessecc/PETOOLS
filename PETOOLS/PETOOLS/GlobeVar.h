//�˴����ڶ���ȫ�ֵĺ궨��
#pragma once
#include "stdafx.h"
#include<Windows.h>
#include "..\Dump\Dump.h"
#include"..\Synchro\Synchro.h"
#include"..\MemoryPool\MemoryPool.h"
#include"../OtherLib/16Edit/16EditDll.h"

#pragma comment(lib,"Disassembler.lib")
#pragma comment(lib,"Dump.lib")
#pragma comment(lib,"Synchro.lib")
#pragma comment(lib,"MemoryPool.lib")
#pragma comment(lib,"PELib.lib")
#pragma comment(lib,"..\\OtherLib\\16Edit\\16Edit.lib")

#ifdef UNICODE

#define MAX_LENGTH 128
#define MAX_FUNLENGTH  128
#define MAX_HEADLENGTH	128
#else
#define MAX_LENGTH 128
#define MAX_FUNLENGTH  128
#define MAX_HEADLENGTH	128
#endif


extern const char *UNUSE;
#define xLength	4
extern TCHAR *_TUNUSE;
#define xxLength	8

extern HANDLE g_DumpThread;


void Create16EditWindow(unsigned char *buffer, unsigned int size, unsigned int startoff, unsigned int endoff);

