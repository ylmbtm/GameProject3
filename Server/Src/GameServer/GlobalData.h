#ifndef _GLOBAL_DATA_H_
#define _GLOBAL_DATA_H_

class GlobalData
{
public:
	GlobalData(void);
	~GlobalData(void);

	BOOL LoadGlobalData();

	BOOL LoadOneTable(std::string TbName);

	//BOOL ParseItemInfo(Recordset *pSet);
	//ItemInfo* GetItemInfo(int id);
};

#endif