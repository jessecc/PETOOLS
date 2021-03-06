#include"../PELib.h"
#include "VCommand.h"

//一些需要死写的伪指令


// 进入虚拟机函数
void _declspec(naked) VStartVM()
{
	_asm
	{
		// 将寄存器压入堆栈,由伪指令取出存放到VMReg中
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
		push ebp
		pushfd
		mov		esi,[esp+0x20]												;参数..伪代码开始的地址
		mov     ebp,esp														;ebp就是堆栈了
		sub		esp,0x200
		mov     edi,esp														;edi就是基地址
		sub		esp,0x40													;这时的esp就是vm用的堆栈了
//Jcc:																		;当流程改变时跳到这里
		mov		ebx,esi														;把伪代码base_addr做key
//Exec:																		;段内的伪代码从这里执行

		movzx	eax,byte ptr [esi]											;获得bytecode
		lea		esi,[esi+1]													;跳过这个字节
		//INT3																;解密al
		//jmp   dword ptr [eax*4+JUMPADDR]									;跳到Handler执行处
		VM_END																;VM结束标记
	}
}

//检测堆栈是否覆盖
void _declspec(naked) DCheckESP()
{
	_asm
	{
		lea     eax, dword ptr [edi+0x100]										;得到当前堆栈位置
		cmp     eax,ebp															;比较
		//jl goto...															;小于则继续执行
		nop
		nop																		;否则
		mov     edx, edi
		mov     ecx, esp
		sub     ecx, edx
		push	esi															;保存IP指针
		mov		esi,esp
		sub		esp, 0x60
		mov		edi,esp
		push	edi															;保存新的edi基地址
		sub		esp, 0x40
		cld
		rep movsb															;复制过去
		pop edi
		pop esi
		VM_END																;VM结束标记
	}
}

void _declspec(naked) DPushReg32()
{
	_asm
	{
		mov		eax,dword ptr [esi]											;得到reg偏移
		//INT3
		add		esi,4
		mov		eax,dword ptr [edi+eax]										;得到寄存器的值
		push	eax															;压入寄存器
		VM_END																;VM结束标记
	}
}
void _declspec(naked) DPushImm32()
{
	_asm
	{
		mov		eax,dword ptr [esi]
		//INT3
		add		esi,4
		push	eax
		VM_END																;VM结束标记
	}
}
void _declspec(naked) DPushMem32()
{
	//_asm
	//{
	//	mov		edx,0
	//	mov		ecx,0
	//	mov		eax,dword ptr [esp]				//第1个寄存器偏移
	//	test 	eax,eax
	//	cmovge	edx,dword ptr [edi+eax]			//如果不是负数则赋值
	//	mov		eax,dword ptr [esp+4]			//第2个寄存器偏移
	//	test 	eax,eax
	//	cmovge	ecx,dword ptr [edi+eax]			//如果不是负数则赋值
	//	imul	ecx,dword ptr [esp+8]			//第2个寄存器的乘积
	//	add     ecx,dword ptr [esp+0x0C]		//第三个为常量
	//	add		edx,ecx
	//	add		esp,0x10//释放参数
	//	push	edx//插入参数
	//	VM_END																;VM结束标记
	//}
	_asm
	{
		mov		edx,0
		mov		ecx,0
		mov		eax,dword ptr [esp]				//第1个寄存器偏移
		test 	eax,eax
		cmovge	edx,dword ptr [edi+eax]			//如果不是负数则赋值
		mov		eax,dword ptr [esp+4]			//第2个寄存器偏移
		test 	eax,eax
		cmovge	ecx,dword ptr [edi+eax]			//如果不是负数则赋值
		imul	ecx,dword ptr [esp+8]			//第2个寄存器的乘积
		mov     eax,dword ptr [esp+0x0C]		//第三个为常量
		mov		eax,dword ptr [eax]
		add		ecx,eax
		add		edx,ecx
		add		esp,0x10//释放参数
		push	edx//插入参数
		VM_END																;VM结束标记
	}
}
//弹回寄存器
void _declspec(naked) DPopReg32()
{
	_asm
	{
		mov		eax,dword ptr [esi]											;得到reg偏移
		add		esi,4
		//INT3
		pop		dword ptr [edi+eax]											;弹回寄存器
		VM_END																;VM结束标记
	}
}

//释放堆栈
void _declspec(naked) DFree()
{
	_asm
	{
		add esp,4
		VM_END																;VM结束标记
	}
}

////push cx
//void VPushReg16()
//{
//	_asm
//	{
//		mov		eax,dword ptr [esp]
//		sub		ebp,2
//		mov		word ptr [ebp],ax
//	}
//}

//void VAdd_Reg32_Imm32()
//{
//	_asm
//	{
//		mov eax,[esp]
//		mov ecx,[esp+4]
//		add eax,ecx
//		mov [esp],eax
//		mov [esp+4],ecx
//	}
//}
//void VAdd_Reg16_Imm16
//{
//	_asm
//	{
//		mov eax,[esp]
//		mov ecx,[esp+4]
//		add	ax,cx
//		mov [esp],eax
//		mov [esp+4],ecx
//	}
//};  