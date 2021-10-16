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

    BOOL   LoadData(CppMySQL3DB& tDBConnection);

    UINT64 MakeNewGuid();

    VOID   SetMaxOnline(INT32 nNum);

    INT32  GetMaxOnline();

    BOOL   SetExData(INT32 nIndex, INT32 dwData);

    INT32  GetExData(INT32 nIndex);

public:
    GlobalDataObject*  m_pGlobalDataObject;
};
#endif //__GLOBAL_DATA_MGR_H__