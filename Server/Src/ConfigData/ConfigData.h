#ifndef __TYPE_DATA_PARSER_H__
#define __TYPE_DATA_PARSER_H__
#include "Sqlite\CppSQLite3.h"
#include "..\ConfigData\ConfigStruct.h"
class CConfigData;

typedef BOOL(CConfigData::*DataFunc)(CppSQLite3Query& QueryData);

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
	BOOL ReadConstantValue(CppSQLite3Query& QueryData);
	StConstantValue m_ConstantValue;

	//货币和体力
	///////////////////////////////////////////////
	std::vector<StMoneyInfo> m_vtMoneyList;
	std::vector<StActionInfo> m_vtActionList;
	BOOL ReadMoneyCfg(CppSQLite3Query& QueryData);
	BOOL ReadActionCfg(CppSQLite3Query& QueryData);
	INT64 GetMoneyMaxValue(UINT32 dwMoneyID);
	INT64 GetActoinMaxValue(UINT32 dwActionID);
	UINT32 GetActoinUnitTime(UINT32 dwActionID);

	//职业
	///////////////////////////////////////////////
	std::map<UINT32, StCarrerInfo> m_mapCarrer;
	BOOL ReadCarrer(CppSQLite3Query& QueryData);
	StCarrerInfo* GetCarrerInfo(UINT32 dwCarrerID);


	//职业等级信息
	///////////////////////////////////////////////
	StLevelInfo m_CarrerLevel[4][MAX_ROLE_LEVEL];
	BOOL ReadCarrerLevel(CppSQLite3Query& QueryData);
	StLevelInfo* GetCarrerLevelInfo(UINT32 dwCarrerID, UINT32 dwLevel);

	//角色
	///////////////////////////////////////////////
	std::map<UINT32, StActor> m_mapActor;
	BOOL ReadActor(CppSQLite3Query& QueryData);
	StActor* GetActorInfo(UINT32 dwActorID);

	//副本
	///////////////////////////////////////////////
	std::map<UINT32, StCopyInfo> m_mapCopyInfo;
	BOOL ReadCopyInfo(CppSQLite3Query& QueryData);
	StCopyInfo* GetCopyInfo(UINT32 dwCopyID);

	//语言配制表
	///////////////////////////////////////////////
	std::string m_strNull;
	std::map<UINT32, StLocalString> m_mapLocalString;
	BOOL ReadLanguage(CppSQLite3Query& QueryData);
	std::string& GetLanguageText(UINT32 dwID, UINT32 dwLang);

	//掉落表
	///////////////////////////////////////////////
	std::map<UINT32, std::vector<StAwardItem>> m_mapAwardItem;
	BOOL ReadAwardData(CppSQLite3Query& QueryData);
	BOOL ParseToDropItem(std::string strDrop, StDropItem& Item);
	BOOL GetAwardItem(INT32 nAwardID, INT32 nCarrer, StAwardItem& AwardItem);
	BOOL GetAwardItemByIndex(INT32 nAwardID, INT32 nCarrer, INT32 nIndex, StItemData& ItemData);
	BOOL GetItemsFromAwardID(INT32 nAwardID, INT32 nCarrer, std::vector<StItemData>& vtItemList);
	BOOL GetItemsAwardIDTimes(INT32 nAwardID, INT32 nCarrer, INT32 nTimes, std::vector<StItemData>& vtItemList);

	//物品表
	///////////////////////////////////////////////
	std::map<UINT32, StItemInfo> m_mapItem;
	BOOL ReadItemData(CppSQLite3Query& QueryData);
	StItemInfo* GetItemInfo(UINT32 dwItemID);

	//功能表
	std::map<UINT32, StFuncInfo> m_mapFuncInfo;
	BOOL ReadFuncInfo(CppSQLite3Query& QueryData);
	StFuncInfo* GetFuncInfo(UINT32 dwFuncID);
	BOOL IsFuncOpen(UINT32 dwFuncID, INT32 level, INT32 viplevel);

	//功能表VIP
	std::map<UINT32, StFuncVipInfo> m_mapFuncVipInfo;
	BOOL ReadFuncVipInfo(CppSQLite3Query& QueryData);
	StFuncVipInfo* GetFuncVipInfo(UINT32 dwFuncID);

	//功能表VIP花费
	std::map<UINT32, std::vector<INT32>> m_mapFuncCostInfo;
	BOOL ReadFuncCostInfo(CppSQLite3Query& QueryData);
	INT32 GetFuncCostInfo(UINT32 dwFuncID, INT32 Times);

	//装备
	std::map<UINT32, StEquipInfo> m_mapEquipInfo;
	BOOL ReadEquipInfo(CppSQLite3Query& QueryData);
	StEquipInfo* GetEquipInfo(UINT32 dwEquipID);

	//宠物
	std::map<UINT32, StPetInfo> m_mapPetInfo;
	BOOL ReadPetInfo(CppSQLite3Query& QueryData);
	StPetInfo* GetPetInfo(UINT32 dwPetID);

	//伙伴
	std::map<UINT32, StPartnerInfo> m_mapPartnerInfo;
	BOOL ReadPartnerInfo(CppSQLite3Query& QueryData);
	StPartnerInfo* GetPartnerInfo(UINT32 dwPartnerID);

	//任务
	std::map<UINT32, StTaskInfo> m_mapTaskInfo;
	BOOL ReadTaskInfo(CppSQLite3Query& QueryData);
	StTaskInfo* GetTaskInfo(UINT32 dwTaskID);

	//技能
	std::map<UINT32, StSkillInfo> m_mapSkillInfo;
	BOOL ReadSkillInfo(CppSQLite3Query& QueryData);
	StSkillInfo* GetSkillInfo(UINT32 dwSkillID, UINT32 dwLevel);

public:

	CppSQLite3DB	m_DBConnection;

	std::vector<DataFuncNode> m_vtDataFuncList;
};



#endif //__TYPE_DATA_PARSER_H__
