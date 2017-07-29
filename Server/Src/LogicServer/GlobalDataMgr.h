#ifndef __GLOBAL_DATA_MGR_H__
#define __GLOBAL_DATA_MGR_H__

struct GlobalDataObject;
class CGlobalDataManager 
{
    CGlobalDataManager();
    ~CGlobalDataManager();
public:
    static CGlobalDataManager* GetInstancePtr();

    BOOL LoadGlobalData();

    UINT64 MakeNewGuid();

public:
   GlobalDataObject  *m_pGlobalDataObject;
};
#endif //__GLOBAL_DATA_MGR_H__