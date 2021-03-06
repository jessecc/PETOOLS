#pragma once
#include<Windows.h>
#include<string>
#include<map>
#include<vector>
#include<DelayImp.h>

#define PE_MODEL
//#define PE_PLUS_MODEL

#define	DosHeader	IMAGE_DOS_HEADER

#ifdef PE_MODEL
#define	NtHeader	IMAGE_NT_HEADERS32
#else
#define	NtHeader	IMAGE_NT_HEADERS64
#endif

#define	MagicValue			WORD
#define SectionHeader		IMAGE_SECTION_HEADER
#define ExportDirectory		IMAGE_EXPORT_DIRECTORY

#ifdef PE_MODEL
#define	ThunkData		IMAGE_THUNK_DATA32
#define	SnapByOrdinal	IMAGE_SNAP_BY_ORDINAL32
#else
#define	ThunkData		IMAGE_THUNK_DATA64
#define	SnapByOrdinal	IMAGE_SNAP_BY_ORDINAL64
#endif

#define	ImportByName	IMAGE_IMPORT_BY_NAME
#define	ImportDescriptor	IMAGE_IMPORT_DESCRIPTOR
#define BaseRelocation		IMAGE_BASE_RELOCATION

#define ResourceDirectory		IMAGE_RESOURCE_DIRECTORY
#define ResourceDirectoryEntry	IMAGE_RESOURCE_DIRECTORY_ENTRY
#define	ResourceDataEntry		IMAGE_RESOURCE_DATA_ENTRY

#ifdef PE_MODEL
#define	TlsDirectory	IMAGE_TLS_DIRECTORY32
#else
#define	TlsDirectory	IMAGE_TLS_DIRECTORY64
#endif

#ifdef PE_MODEL
#define	LoadConfigDirctory	IMAGE_LOAD_CONFIG_DIRECTORY32
#else
#define	LoadConfigDirctory	IMAGE_LOAD_CONFIG_DIRECTORY64
#endif

#define	BoundImportDescriptor	IMAGE_BOUND_IMPORT_DESCRIPTOR
#define	BoundForwarderRef		IMAGE_BOUND_FORWARDER_REF
#define	DebugDirectoryItem		IMAGE_DEBUG_DIRECTORY


//导出表定义
typedef struct _OrdinalFuncs
{
#ifdef PE_MODEL
	DWORD Func;						//RVAi(问题，函数地址永远是4字节?)
#else
	ULONGLONG Func;	
#endif
	std::string	strFuncName;
}OrdinalFuncs,*POrdinalFuncs;

typedef struct _Export
{
	std::vector<OrdinalFuncs>		mOrdinalFuncsVector;
	std::string						strDllName;
	ExportDirectory					mDirectorys;
}Export,*PExport;

//导入表定义
typedef struct _ThunkItem{
	DWORD	ThunkRVA;
	DWORD	ThunkFOA;
    std::string strFuncName;   
    WORD Ordinal;       //函数索引
#ifdef PE_MODEL
	DWORD AddrFunc;           //存放函数地址RVA
#else
	ULONGLONG AddrFunc;	
#endif
	
}ThunkItem,*PThunkItem;

typedef struct _ImportItem
{
	ImportDescriptor	mDescriptor;
	std::string			strDllName;
	std::vector<ThunkItem>	mThunksVector;
}ImportItem,*PImportItem;

//重定位表
typedef struct _ElementItem
{
	WORD	cType:4;
	WORD	wValue:12;
}ElementItem,*PElementItem;

typedef struct _RelocItem
{
	BaseRelocation				mBaseRelocation;
	std::vector<ElementItem>	mSnVector;
}RelocItem,* PRelocItem;

//资源表
typedef struct _EntryDataItem
{
	WORD				wId;       //标准ID
	std::wstring		wstrId;    //非标准的命名字符串
	WORD				wLua;     //标准语言类型码
	std::wstring		wstrLua;         //非标准的语言字符串
	ResourceDataEntry	mDataEntry;
}EntryDataItem,*PEntryDataItem;

typedef struct _ResourceItem
{
	WORD					dwType;			//标准资源类型，0代表非标准资源，非0代表标准资源标示符
	std::wstring			wstrType;		//非标准资源名称字符串
	std::vector<EntryDataItem>	mEntryDatasVector;
}ResourceItem,*PResourceItem;

//延迟输入表定义
typedef struct _DelayItem
{
	ImgDelayDescr mDescriptor;
	std::vector<ThunkItem>	mThunksVector;
	std::string			strDllName; 
}DelayItem,*PDelayItem;

//绑定导入表定义
typedef struct _BoundForwarderRefItem
{
	std::string			strDllName;
	BoundForwarderRef	mBoundForwarderRef;
}BoundForwarderRefItem,*PBoundForwarderRefItem;

typedef struct _BoundItem
{
	BoundImportDescriptor mBoundImportDescriptor;
	std::string	strDllName;
	std::vector<BoundForwarderRefItem>	mBoundForwarderRefsVector;
}BoundItem,*PBoundItem;

//附加数据结构定义
typedef struct _Overlay
{
	bool		isExist;    //是否存在附加数据
	ULONGLONG	dwOffset;		//附加数据偏移(FOA)
	size_t		dwSize;    //附加数据大小
}Overlay,*POverlay;

//PE头定义
typedef struct {
	DosHeader	mDosHeader;
	NtHeader	mNtHeader;
	MagicValue	wMagic;
	std::vector<SectionHeader> mSectionsVector;
	Export					mExport;     
	std::vector<ImportItem>	mImportsVector;
	std::vector<RelocItem>	mRelocsVector;
	std::vector<ResourceItem>	mResourcesVector;
	std::vector<DelayItem>		mDelaysVector;
	TlsDirectory		mTlsDirrctory;
	LoadConfigDirctory	mLoadConfigDirctory;
	std::vector<BoundItem>	mBoundsVector;
	std::vector<DebugDirectoryItem>	mDebugDirectorysVector;
	Overlay			mOverlay;
} pe_file_t;

typedef struct {
	std::string path;
	char *pVirMem;
	size_t size;
	pe_file_t pe;
} pe_ctx_t;