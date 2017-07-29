#ifndef __TYPE_DATA_PARSER_H__
#define __TYPE_DATA_PARSER_H__
#include "Sqlite\CppSQLite3.h"
#include "..\ConfigData\ConfigStruct.h"
class CConfigData;

typedef BOOL(CConfigData::*DataFunc)(CppSQLite3Query &QueryData);

struct DataFuncNode 
{
    std::string m_strTbName;
    DataFunc m_pDataFunc;

    DataFuncNode(std::string sValue, DataFunc func)
    {
        m_strTbName = sValue;
        m_pDataFunc = func;
    }
};

class CConfigData
{
    CConfigData();
	~CConfigData();
public:
	static CConfigData* GetInstancePtr();
	BOOL InitDataReader();
	BOOL ReadConfigData(std::string strDbFile); 

public:
	//基本配制变量
	///////////////////////////////////////////////
	BOOL ReadConstantValue(CppSQLite3Query &QueryData);
	StConstantValue m_ConstantValue;

	//货币和体力
	///////////////////////////////////////////////
	std::vector<StMoneyInfo> m_vtMoneyList;
	std::vector<StActionInfo> m_vtActionList;
	INT64 GetMoneyMaxValue(UINT32 dwMoneyID);
	INT64 GetActoinMaxValue(UINT32 dwActionID);
	UINT32 GetActoinUnitTime(UINT32 dwActionID);

	//角色
	///////////////////////////////////////////////
	std::map<UINT32, StActor> m_mapActor;
    BOOL ReadActor(CppSQLite3Query &QueryData);
	StActor* GetActorInfo(UINT32 dwActorID);
	
	//副本
	///////////////////////////////////////////////
    std::map<UINT32, StCopyInfo> m_mapCopyInfo; 
	BOOL ReadCopyInfo(CppSQLite3Query &QueryData);
	StCopyInfo* GetCopyInfo(UINT32 dwCopyID);

	//语言配制表
	///////////////////////////////////////////////
    std::string m_strNull;
    std::map<UINT32, StLocalString> m_mapLocalString;
	BOOL ReadLanguage(CppSQLite3Query &QueryData);
    std::string& GetLanguageText(UINT32 dwID, UINT32 dwLang);

	//掉落物品表
	///////////////////////////////////////////////
	std::map<UINT32, StAwardItem> m_mapAwardItem;
	BOOL ReadAwardData(CppSQLite3Query &QueryData);
	BOOL ParseToDropItem(std::string strDrop, StDropItem &Item);
	BOOL GetAwardItemByIndex(INT32 nAwardID, INT32 nIndex, StItemData &ItemData);
	BOOL GetItemsFromAwardID(INT32 nAwardID, std::vector<StItemData> &vtItemList);
	BOOL GetItemsAwardIDTimes(INT32 nAwardID, INT32 nTimes, std::vector<StItemData> &vtItemList);
	
	
	
	
public:

	CppSQLite3DB	m_DBConnection;

	std::vector<DataFuncNode> m_vtDataFuncList;
};



#endif //__TYPE_DATA_PARSER_H__
