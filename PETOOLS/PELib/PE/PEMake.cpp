#include "PEMake.h"
#include<Windows.h>
#include"PeProtect.h"
#include"../Patch.h"
#include"../mem.h"


PEMake::PEMake()
{
	ptrPeProtect = new PeProtect(mPeCtx);
}

PEMake::~PEMake()
{
	delete ptrPeProtect;
	ptrPeProtect = NULL;
}

bool PEMake::isAnalysised()
{
	return ptrPeProtect->isAnalysised;
}

bool PEMake::PELoadFile(const char *lpPath,const char *lpModel)
{
	return ptrPeProtect->PeLoadFile(lpPath,lpModel);
}

void PEMake::PEUnload()
{
	if (ptrPeProtect)
	{
		ptrPeProtect->PeUnload();
	}
}

bool PEMake::CheckPESig()
{
	return ptrPeProtect->CheckPeSig();
}

bool PEMake::Analysis()
{
	return ptrPeProtect->Analysis();
}

void PEMake::SetAnalysised(bool isAnalysised)
{
	ptrPeProtect->isAnalysised=isAnalysised;
}

unsigned int PEMake::RvaToFoa(unsigned int rva)
{
	return ptrPeProtect->RvaToFoa(rva);
}

unsigned int PEMake::FoaToRva(unsigned int foa)
{
	return ptrPeProtect->FoaToRva(foa);
}

bool PEMake::ClsRelocDataDirectory()
{
	return ptrPeProtect->ClsRelocDataDirectory();
}

bool PEMake::AddSectionToEnd(const char *data, unsigned int size)
{
	return ptrPeProtect->AddSectionToEnd(data,size);
}

bool PEMake::EncryptImportTable()
{
	return ptrPeProtect->EncryptImportTable();
}

bool PEMake::AddPatch(const char *pName, const void *pPatch, const unsigned int dwSize, unsigned int mOffset)
{
	return ptrPeProtect->AddPatch(pName, pPatch, dwSize, mOffset);
}

bool PEMake::Protect1A()
{
	unsigned int mShell_nSize =(DWORD) (&_patch1_ShellCodeEnd_) - (DWORD)(&_patch1_ShellCodeBegin_);
	char *pData=new char[mShell_nSize];
	memset(pData,0,mShell_nSize);
	memcpy(pData,&_patch1_ShellCodeBegin_,mShell_nSize);
	bool bret=ptrPeProtect->AddPatch(".PEMake",pData,mShell_nSize,(DWORD)&_patch1_offset_entry_jump-(DWORD)&_patch1_ShellCodeBegin_);
	delete []pData;
	return bret;
}

bool PEMake::Protect1B()
{
	return true;
}

bool PEMake::Protect2A()
{
	unsigned int mShell_nSize =(DWORD) (&_patch2_ShellCodeEnd_) - (DWORD)(&_patch2_ShellCodeBegin_);
	char *pData=new char[mShell_nSize];
	memset(pData,0,mShell_nSize);
	memcpy(pData,&_patch2_ShellCodeBegin_,mShell_nSize);
	bool bret=ptrPeProtect->EncryptOne(".PEMake",pData,mShell_nSize,(DWORD)&_patch2_offset_entry_jump-(DWORD)&_patch2_ShellCodeBegin_);
	delete []pData;
	return bret;
}

bool PEMake::Protect3A()
{
	//��3
	unsigned int mShell_nSize = (DWORD)(&_patch3_ShellCodeEnd_) - (DWORD)(&_patch3_ShellCodeBegin_);
	char *pData = new char[mShell_nSize];
	memset(pData, 0, mShell_nSize);
	memcpy(pData, &_patch3_ShellCodeBegin_, mShell_nSize);
	return ptrPeProtect->EncryptTwo(".peMake", pData, mShell_nSize, (DWORD)&_patch3_offset_entry_jump - (DWORD)&_patch3_ShellCodeBegin_);
}

bool PEMake::Protect4A()
{
	//��4
	return ptrPeProtect->EncryptThree(".peMake");
}