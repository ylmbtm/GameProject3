#ifndef __GLOBAL_DATA_MGR_H__
#define __GLOBAL_DATA_MGR_H__
#include "DBInterface/CppMysql.h"

struct GlobalDataObject;
class CGlobalDataManager
{
	CGlobalDataManager();
	~CGlobalDataManager();
public:
	static CGlobalDataManager* GetInstancePtr();

	BOOL LoadData(CppMySQL3DB& tDBConnection);

	UINT64 MakeNewGuid();

	VOID   SetMaxOnline(INT32 nNum);

	UINT32 GetMaxOnline();

	BOOL   SetDataChange();

public:
	GlobalDataObject*  m_pGlobalDataObject;
};
#endif //__GLOBAL_DATA_MGR_H__