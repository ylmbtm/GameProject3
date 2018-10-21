#include "stdafx.h"
#include "ConfigData.h"

CConfigData::CConfigData()
{
	InitDataReader();
}

CConfigData::~CConfigData()
{

}

CConfigData* CConfigData::GetInstancePtr()
{
	static CConfigData _StaticMgr;

	return &_StaticMgr;
}

BOOL CConfigData::InitDataReader()
{
	m_vtDataFuncList.push_back(DataFuncNode("Data_Constant",    &CConfigData::ReadConstantData));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Role",        &CConfigData::ReadCarrer));
	m_vtDataFuncList.push_back(DataFuncNode("Data_RoleLevel",   &CConfigData::ReadCarrerLevel));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Actor",       &CConfigData::ReadActor));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Copy",        &CConfigData::ReadCopyInfo));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Item",        &CConfigData::ReadItemData));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Action",      &CConfigData::ReadActionCfg));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Language",    &CConfigData::ReadLanguage));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Award",       &CConfigData::ReadAwardData));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Func",        &CConfigData::ReadFuncInfo));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Func_Vip",    &CConfigData::ReadFuncVipInfo));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Func_Cost",   &CConfigData::ReadFuncCostInfo));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Equip",       &CConfigData::ReadEquipInfo));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Gem",			&CConfigData::ReadGemInfo));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Pet",         &CConfigData::ReadPetInfo));
 	m_vtDataFuncList.push_back(DataFuncNode("Data_Partner",     &CConfigData::ReadPartnerInfo));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Task",        &CConfigData::ReadTaskInfo));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Skill",       &CConfigData::ReadSkillInfo));
// 	m_vtDataFuncList.push_back(DataFuncNode("Data_Buff",        &CConfigData::ReadBuffInfo));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Store",       &CConfigData::ReadStoreInfo));
	return TRUE;
}

BOOL CConfigData::LoadConfigData(std::string strDbFile)
{
	try
	{
		m_DBConnection.open(strDbFile.c_str());
	}
	catch(CppSQLite3Exception& e)
	{
		CLog::GetInstancePtr()->LogError("CConfigData::LoadConfigData Failed!!!, Reason:%s", e.errorMessage());
		return FALSE;
	}

	char szSql[SQL_BUFF_LEN]  = {0};
	for(std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
	{
		DataFuncNode dataNode = (*itor);
		snprintf(szSql, SQL_BUFF_LEN, "select * from %s;", dataNode.m_strTbName.c_str());
		CppSQLite3Query Tabledatas = m_DBConnection.execQuery(szSql);
		(this->*dataNode.m_pDataFunc)(Tabledatas);
	}

	m_DBConnection.close();

	return TRUE;
}

BOOL CConfigData::ReloadConfigData( std::string strTbName )
{
	try
	{
		m_DBConnection.open("Config.db");
	}
	catch(CppSQLite3Exception& e)
	{
		CLog::GetInstancePtr()->LogError("CConfigData::ReloadConfigData Failed!!!, Reason:%s", e.errorMessage());
		return FALSE;
	}

	char szSql[SQL_BUFF_LEN]  = {0};
	for(std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
	{
		DataFuncNode dataNode = (*itor);
		if(dataNode.m_strTbName != strTbName)
		{
			continue;
		}

		snprintf(szSql, SQL_BUFF_LEN, "select * from %s;", dataNode.m_strTbName.c_str());
		CppSQLite3Query Tabledatas = m_DBConnection.execQuery(szSql);
		(this->*dataNode.m_pDataFunc)(Tabledatas);
	}

	m_DBConnection.close();

	return TRUE;
}

BOOL CConfigData::ReadConstantData(CppSQLite3Query& QueryData)
{
	m_mapConstantValue.clear();
	while(!QueryData.eof())
	{
		std::string strName = QueryData.getStringField("name");
		std::string strValue = QueryData.getStringField("value");

		m_mapConstantValue.insert(std::make_pair(strName, CommonConvert::StringToInt(strValue.c_str())));

		QueryData.nextRow();
	}

	return TRUE;
}

INT32 CConfigData::GetConstantIntValue(std::string& strName)
{
	std::map<std::string, INT32>::iterator itor = m_mapConstantValue.find(strName);
	if (itor != m_mapConstantValue.end())
	{
		return itor->second;
	}

	return 0;
}


INT64 CConfigData::GetMoneyMaxValue(UINT32 dwMoneyID)
{
	if((dwMoneyID <= 0) || (dwMoneyID >= m_vtMoneyList.size()))
	{
		ASSERT_FAIELD;
		return 1;
	}

	return m_vtActionList.at(dwMoneyID - 1).dwMax;
}

INT64 CConfigData::GetActoinMaxValue(UINT32 dwActionID)
{
	if((dwActionID <= 0) || (dwActionID >= m_vtActionList.size()))
	{
		ASSERT_FAIELD;
		return 1;
	}

	return m_vtActionList.at(dwActionID - 1).dwMax;
}

UINT32 CConfigData::GetActoinUnitTime(UINT32 dwActionID)
{
	if((dwActionID <= 0) || (dwActionID >= m_vtActionList.size()))
	{
		ASSERT_FAIELD;
		return 1;
	}

	return m_vtActionList.at(dwActionID - 1).UnitTime;
}

BOOL CConfigData::ReadCarrer(CppSQLite3Query& QueryData)
{
	m_mapCarrer.clear();

	while(!QueryData.eof())
	{
		StCarrerInfo stValue;
		stValue.dwID = QueryData.getIntField("Carrer");
		stValue.dwActorID = QueryData.getIntField("ActorID");
		stValue.dwBornCity = QueryData.getIntField("BornCity");
		m_mapCarrer.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StCarrerInfo* CConfigData::GetCarrerInfo(UINT32 dwCarrerID)
{
	std::map<UINT32, StCarrerInfo>::iterator itor = m_mapCarrer.find(dwCarrerID);

	ERROR_RETURN_NULL(itor != m_mapCarrer.end());

	return &itor->second;
}

BOOL CConfigData::ReadMoneyCfg(CppSQLite3Query& QueryData)
{
	return TRUE;
}

BOOL CConfigData::ReadActionCfg(CppSQLite3Query& QueryData)
{
	m_vtActionList.clear();
	while(!QueryData.eof())
	{
		StActionInfo stValue;
		stValue.dwActionID = QueryData.getIntField("Id");
		stValue.dwMax = QueryData.getIntField("Max");
		stValue.UnitTime = QueryData.getIntField("UnitTime");
		m_vtActionList.push_back(stValue);
		QueryData.nextRow();
	}

	return TRUE;
}

BOOL CConfigData::ReadCarrerLevel(CppSQLite3Query& QueryData)
{
	while(!QueryData.eof())
	{
		UINT32 dwCarrerID = QueryData.getIntField("Carrer");
		UINT32 dwLevel = QueryData.getIntField("Level");
		m_CarrerLevel[dwCarrerID - 1][dwLevel - 1].dwLevel = dwLevel;
		m_CarrerLevel[dwCarrerID - 1][dwLevel - 1].dwFightValue = QueryData.getIntField("FightValue");
		m_CarrerLevel[dwCarrerID - 1][dwLevel - 1].dwNeedExp = QueryData.getIntField("RequireExp");

		int nIndex  = QueryData.fieldIndex("P1");
		for(int i = 0; i < 20; i++)
		{
			m_CarrerLevel[dwCarrerID - 1][dwLevel - 1].Propertys[i] = QueryData.getIntField(i + nIndex, 0);
		}

		QueryData.nextRow();
	}

	return TRUE;
}

StLevelInfo* CConfigData::GetCarrerLevelInfo(UINT32 dwCarrerID, UINT32 dwLevel)
{
	return &m_CarrerLevel[dwCarrerID - 1][dwLevel - 1];
}

BOOL CConfigData::ReadActor(CppSQLite3Query& QueryData)
{
	m_mapActor.clear();

	while(!QueryData.eof())
	{
		StActorInfo stValue;
		stValue.dwID = QueryData.getIntField("Id");
		int nIndex  = QueryData.fieldIndex("P1");
		for(int i = 0; i < PROPERTY_NUM; i++)
		{
			stValue.Propertys[i] = QueryData.getIntField(i + nIndex, 0);
		}

		m_mapActor.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StActorInfo* CConfigData::GetActorInfo(UINT32 dwActorID)
{
	ERROR_RETURN_NULL(dwActorID != 0);
	std::map<UINT32, StActorInfo>::iterator itor = m_mapActor.find(dwActorID);
	if(itor != m_mapActor.end())
	{
		return &itor->second;
	}
	ASSERT_FAIELD;
	return NULL;
}

BOOL CConfigData::ReadCopyInfo(CppSQLite3Query& QueryData)
{
	m_mapCopyInfo.clear();

	while(!QueryData.eof())
	{
		StCopyInfo stValue;
		stValue.dwCopyID = QueryData.getIntField("Id");
		stValue.dwCopyType = QueryData.getIntField("CopyType");
		stValue.dwCostActID = QueryData.getIntField("CostActionId");
		stValue.dwCostActNum = QueryData.getIntField("CostActionNum");
		stValue.dwGetMoneyID = QueryData.getIntField("GetMoneyId");
		stValue.dwGetMoneyRatio = QueryData.getIntField("GetMoneyRatio");
		m_mapCopyInfo.insert(std::make_pair(stValue.dwCopyID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StCopyInfo* CConfigData::GetCopyInfo(UINT32 dwCopyID)
{
	ERROR_RETURN_NULL(dwCopyID != 0);
	std::map<UINT32, StCopyInfo>::iterator itor = m_mapCopyInfo.find(dwCopyID);
	if(itor != m_mapCopyInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadLanguage(CppSQLite3Query& QueryData)
{
	m_mapLanguage.clear();

	while(!QueryData.eof())
	{
		StLocalString stValue;
		stValue.dwID = QueryData.getIntField("id");
		stValue.Language[0] = QueryData.getIntField("lang0");
		stValue.Language[1] = QueryData.getIntField("lang1");
		stValue.Language[2] = QueryData.getIntField("lang2");
		stValue.Language[3] = QueryData.getIntField("lang3");
		stValue.Language[4] = QueryData.getIntField("lang4");
		stValue.Language[5] = QueryData.getIntField("lang5");
		stValue.Language[6] = QueryData.getIntField("lang6");
		stValue.Language[7] = QueryData.getIntField("lang7");
		stValue.Language[8] = QueryData.getIntField("lang8");
		stValue.Language[9] = QueryData.getIntField("lang9");
		stValue.Language[10] = QueryData.getIntField("lang10");
		stValue.Language[11] = QueryData.getIntField("lang11");
		stValue.Language[12] = QueryData.getIntField("lang12");
		stValue.Language[13] = QueryData.getIntField("lang13");
		stValue.Language[14] = QueryData.getIntField("lang14");
		m_mapLanguage.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

std::string& CConfigData::GetLanguageText( UINT32 dwID, UINT32 dwLang )
{
	static std::string strNull = "null";

	if((dwLang < 0) || (dwLang > 14))
	{
		return strNull;
	}

	std::map<UINT32, StLocalString>::iterator itor = m_mapLanguage.find(dwID);
	if(itor != m_mapLanguage.end())
	{
		return itor->second.Language[dwLang];
	}

	return strNull;
}

std::string& CConfigData::GetLanguageText(std::string strID, UINT32 dwLang)
{
	static std::string strNull = "null";
	return strNull;
}

BOOL CConfigData::ReadAwardData(CppSQLite3Query& QueryData)
{
	m_mapAwardItem.clear();

	while(!QueryData.eof())
	{
		StAwardItem stValue;
		stValue.dwAwardID = QueryData.getIntField("awardid");
		std::string strFixDrop = QueryData.getStringField(2);
		std::string strRatioDrop = QueryData.getStringField(4);
		stValue.bDistinct = QueryData.getIntField(4);
		stValue.dwRatioCount  = QueryData.getIntField(4);
		//解析固定掉落
		if (strFixDrop != "NULL")
		{
			std::vector<std::string> vtRet;
			CommonConvert::SpliteString(strFixDrop, ")(", vtRet);
			for(std::vector<std::string>::size_type i = 0; i < vtRet.size(); i++)
			{
				StDropItem item;
				ParseToDropItem(vtRet.at(i), item);
				stValue.FixItems.push_back(item);
			}
		}

		if (strRatioDrop != "NULL")
		{
			UINT32 dwRatioBegin = 1;
			UINT32 dwTempValue = 0;
			std::vector<std::string> vtRet;
			CommonConvert::SpliteString(strFixDrop, ")(", vtRet);

			StDropItem item;
			for(std::vector<std::string>::size_type i = 0; i < vtRet.size(); i++)
			{
				ParseToDropItem(vtRet.at(i), item);
				stValue.RatioItems.push_back(item);

				dwTempValue = stValue.RatioItems[i].dwRatio;
				stValue.RatioItems[i].dwRatio = dwRatioBegin;
				dwRatioBegin += dwTempValue;
			}


			stValue.RatioItems.push_back(item);
			stValue.RatioItems[vtRet.size()].dwItemID = 0;
			stValue.RatioItems[vtRet.size()].dwRatio = 10000;
		}

		if ((stValue.FixItems.size() <= 0) && (stValue.RatioItems.size() <= 0))
		{

		}

		auto itor = m_mapAwardItem.find(stValue.dwAwardID);
		if(itor != m_mapAwardItem.end())
		{
			std::vector<StAwardItem>& vtList = itor->second;

			vtList.push_back(stValue);
		}
		else
		{
			std::vector<StAwardItem> vtList;
			vtList.push_back(stValue);
			m_mapAwardItem.insert(std::make_pair(stValue.dwAwardID, vtList));
		}

		QueryData.nextRow();
	}

	return TRUE;
}

BOOL CConfigData::ParseToDropItem(std::string strDrop, StDropItem& item)
{
	std::vector<std::string> vtRet;
	CommonConvert::SpliteString(strDrop, "|", vtRet);
	if (vtRet.size() < 3)
	{
		return FALSE;
	}

	item.dwItemID = CommonConvert::StringToInt(vtRet[0].c_str());
	item.dwRatio = CommonConvert::StringToInt(vtRet[2].c_str());
	CommonConvert::SpliteString(vtRet[1], "|", vtRet);
	item.dwItemNum[0] =  CommonConvert::StringToInt(vtRet[0].c_str());
	item.dwItemNum[1] =  CommonConvert::StringToInt(vtRet[1].c_str());

	return TRUE;
}

BOOL CConfigData::GetAwardItem(INT32 nAwardID, INT32 nCarrer, StAwardItem& AwardItem)
{
	std::map<UINT32, std::vector<StAwardItem>>::iterator itor =  m_mapAwardItem.find(nAwardID);
	if(itor == m_mapAwardItem.end())
	{
		return FALSE;
	}

	std::vector<StAwardItem>& AwardItemList = itor->second;

	for(std::vector<StAwardItem>::size_type i = 0; i < AwardItemList.size(); i++)
	{
		AwardItem = AwardItemList.at(i);
		if(AwardItem.nCarrer == nCarrer)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CConfigData::GetAwardItemByIndex(INT32 nAwardID, INT32 nCarrer, INT32 nIndex, StItemData& ItemData)
{
	StAwardItem AwardItem;

	if(!GetAwardItem(nAwardID, nCarrer, AwardItem))
	{
		return FALSE;
	}

	if (nIndex >= (INT32)AwardItem.FixItems.size())
	{
		CLog::GetInstancePtr()->LogError("GetItemByIndex Error: Invalid index :%d", nIndex);
		return FALSE;
	}

	ItemData.dwItemID = AwardItem.FixItems[nIndex].dwItemID;
	ItemData.dwItemNum = AwardItem.FixItems[nIndex].dwItemNum[0];
	return TRUE;
}

BOOL CConfigData::GetItemsFromAwardID(INT32 nAwardID, INT32 nCarrer, std::vector<StItemData>& vtItemList)
{
	StAwardItem AwardItem;

	if(!GetAwardItem(nAwardID, nCarrer, AwardItem))
	{
		return FALSE;
	}

	StItemData tempItem;

	for (std::vector<StDropItem>::size_type i = 0; i < AwardItem.FixItems.size(); i++ )
	{
		tempItem.dwItemID = AwardItem.FixItems[i].dwItemID;
		if (AwardItem.FixItems[i].dwItemNum[0] == AwardItem.FixItems[i].dwItemNum[1])
		{
			tempItem.dwItemNum = AwardItem.FixItems[i].dwItemNum[0];
		}
		else
		{
			tempItem.dwItemNum = AwardItem.FixItems[i].dwItemNum[0] + CommonFunc::GetRandNum(0) % (AwardItem.FixItems[i].dwItemNum[1] - AwardItem.FixItems[i].dwItemNum[0] + 1);
		}

		if (tempItem.dwItemNum > 0)
		{
			vtItemList.push_back(tempItem);
		}
	}



	for (int  cycle = 0; cycle < AwardItem.dwRatioCount; cycle++ )
	{
		UINT32 dwRandValue = CommonFunc::GetRandNum(0);
		for (std::vector<StDropItem>::size_type i = 0; i < AwardItem.RatioItems.size() - 1; i++)
		{
			if ((dwRandValue >= AwardItem.RatioItems[i].dwRatio) && (dwRandValue < AwardItem.RatioItems[i + 1].dwRatio))
			{
				tempItem.dwItemID = AwardItem.RatioItems[i].dwItemID;
				if (AwardItem.RatioItems[i].dwItemNum[1] == AwardItem.RatioItems[i].dwItemNum[0])
				{
					tempItem.dwItemNum = AwardItem.RatioItems[i].dwItemNum[0];
				}
				else
				{
					tempItem.dwItemNum = AwardItem.RatioItems[i].dwItemNum[0] + CommonFunc::GetRandNum(0) % (AwardItem.RatioItems[i].dwItemNum[1] - AwardItem.RatioItems[i].dwItemNum[0] + 1);
				}

				if (tempItem.dwItemNum > 0)
				{
					vtItemList.push_back(tempItem);
				}
			}
		}
	}

	return TRUE;
}

BOOL CConfigData::GetItemsAwardIDTimes(INT32 nAwardID, INT32 nCarrer, INT32 nTimes, std::vector<StItemData>& vtItemList)
{
	StAwardItem AwardItem;

	if(!GetAwardItem(nAwardID, nCarrer, AwardItem))
	{
		return FALSE;
	}

	StItemData tempItem;

	for (std::vector<StDropItem>::size_type i = 0; i < AwardItem.FixItems.size(); i++ )
	{
		tempItem.dwItemID = AwardItem.FixItems[i].dwItemID;
		if (AwardItem.FixItems[i].dwItemNum[0] == AwardItem.FixItems[i].dwItemNum[1])
		{
			tempItem.dwItemNum = AwardItem.FixItems[i].dwItemNum[0];
		}
		else
		{
			tempItem.dwItemNum = AwardItem.FixItems[i].dwItemNum[0] + CommonFunc::GetRandNum(0) % (AwardItem.FixItems[i].dwItemNum[1] - AwardItem.FixItems[i].dwItemNum[0] + 1);
		}

		if (tempItem.dwItemNum > 0)
		{
			vtItemList.push_back(tempItem);
		}
	}

	for (int  cycle = 0; cycle < AwardItem.dwRatioCount * nTimes; cycle++ )
	{
		UINT32 dwRandValue = CommonFunc::GetRandNum(0);
		for (std::vector<StDropItem>::size_type i = 0; i < AwardItem.RatioItems.size() - 1; i++)
		{
			if ((dwRandValue >= AwardItem.RatioItems[i].dwRatio) && (dwRandValue < AwardItem.RatioItems[i + 1].dwRatio))
			{
				tempItem.dwItemID = AwardItem.RatioItems[i].dwItemID;
				if (AwardItem.RatioItems[i].dwItemNum[1] == AwardItem.RatioItems[i].dwItemNum[0])
				{
					tempItem.dwItemNum = AwardItem.RatioItems[i].dwItemNum[0];
				}
				else
				{
					tempItem.dwItemNum = AwardItem.RatioItems[i].dwItemNum[0] + CommonFunc::GetRandNum(0) % (AwardItem.RatioItems[i].dwItemNum[1] - AwardItem.RatioItems[i].dwItemNum[0] + 1);
				}

				if (tempItem.dwItemNum > 0)
				{
					vtItemList.push_back(tempItem);
				}
			}
		}
	}
	return TRUE;
}

BOOL CConfigData::ReadItemData(CppSQLite3Query& QueryData)
{
	m_mapItem.clear();

	while(!QueryData.eof())
	{
		StItemInfo stValue;
		stValue.dwItemID = QueryData.getIntField("Id");
		stValue.dwItemType = QueryData.getIntField("ItemType");
		stValue.SellID = QueryData.getIntField("SellMoneyId");
		stValue.SellPrice = QueryData.getIntField("SellMoneyNum");
		stValue.Quality = QueryData.getIntField("Quality");
		stValue.StackMax = QueryData.getIntField("StackMax");
		//stValue.UseType = QueryData.getIntField("usetype");
		stValue.Data1 = QueryData.getIntField("Data1");
		stValue.Data2 = QueryData.getIntField("Data2");
		m_mapItem.insert(std::make_pair(stValue.dwItemID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

BOOL CConfigData::ReadFuncInfo(CppSQLite3Query& QueryData)
{
	while(!QueryData.eof())
	{
		StFuncInfo stValue;
		m_mapFuncInfo.insert(std::make_pair(stValue.dwFuncID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StFuncInfo* CConfigData::GetFuncInfo(UINT32 dwFuncID)
{
	ERROR_RETURN_NULL(dwFuncID != 0);
	auto itor = m_mapFuncInfo.find(dwFuncID);
	if(itor != m_mapFuncInfo.end())
	{
		return &itor->second;
	}

	return NULL;
}

BOOL CConfigData::IsFuncOpen(UINT32 dwFuncID, INT32 level, INT32 viplevel)
{
	StFuncInfo* pFuncInfo = GetFuncInfo(dwFuncID);
	ERROR_RETURN_FALSE (pFuncInfo != NULL);
	ERROR_RETURN_FALSE (pFuncInfo->Logic  > 0);
	ERROR_RETURN_FALSE (pFuncInfo->Logic  < 5);
	if (pFuncInfo->Logic == 1)
	{
		if (level >= pFuncInfo->OpenLevel || viplevel >= pFuncInfo->VipLevel)
		{
			return TRUE;
		}
	}
	else if (pFuncInfo->Logic == 2)
	{
		if (level >= pFuncInfo->OpenLevel && viplevel >= pFuncInfo->VipLevel)
		{
			return TRUE;
		}
	}
	else if (pFuncInfo->Logic == 3)
	{
		if (viplevel >= pFuncInfo->VipLevel)
		{
			return TRUE;
		}
	}
	else if (pFuncInfo->Logic == 4)
	{
		if (level >= pFuncInfo->OpenLevel)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CConfigData::ReadFuncVipInfo(CppSQLite3Query& QueryData)
{
	m_mapFuncVipInfo.clear();

	while(!QueryData.eof())
	{
		StFuncVipInfo stValue;
		m_mapFuncVipInfo.insert(std::make_pair(stValue.dwFuncID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StFuncVipInfo* CConfigData::GetFuncVipInfo(UINT32 dwFuncID)
{
	ERROR_RETURN_NULL(dwFuncID != 0);
	auto itor = m_mapFuncVipInfo.find(dwFuncID);
	if(itor != m_mapFuncVipInfo.end())
	{
		return &itor->second;
	}

	return NULL;
}

BOOL CConfigData::ReadFuncCostInfo(CppSQLite3Query& QueryData)
{
	m_mapFuncCostInfo.clear();

	while(!QueryData.eof())
	{
		UINT32 dwFuncID = 0;
		std::vector<INT32> vtValue;
		m_mapFuncCostInfo.insert(std::make_pair(dwFuncID, vtValue));
		QueryData.nextRow();
	}

	return TRUE;
}

INT32 CConfigData::GetFuncCostInfo(UINT32 dwFuncID, INT32 Times)
{
	ERROR_RETURN_FALSE(dwFuncID > 0);

	auto itor = m_mapFuncCostInfo.find(dwFuncID);
	if(itor != m_mapFuncCostInfo.end())
	{
		std::vector<INT32>& vtCost = itor->second;

		return vtCost[Times];
	}

	return 0;
}

BOOL CConfigData::ReadEquipInfo(CppSQLite3Query& QueryData)
{
	m_mapEquipInfo.clear();

	while(!QueryData.eof())
	{
		StEquipInfo stValue;
		stValue.dwEquipID = QueryData.getIntField("Id");
		stValue.dwSuitID = QueryData.getIntField("Suit");
		stValue.dwPos = QueryData.getIntField("Pos");
		m_mapEquipInfo.insert(std::make_pair(stValue.dwEquipID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StEquipInfo* CConfigData::GetEquipInfo(UINT32 dwEquipID)
{
	ERROR_RETURN_NULL(dwEquipID != 0);
	auto itor = m_mapEquipInfo.find(dwEquipID);
	if(itor != m_mapEquipInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}


BOOL CConfigData::ReadGemInfo(CppSQLite3Query& QueryData)
{
	m_mapGemInfo.clear();

	while (!QueryData.eof())
	{
		StGemInfo stValue;
		stValue.dwGemID = QueryData.getIntField("Id");
		stValue.dwPos = QueryData.getIntField("Pos");
		m_mapGemInfo.insert(std::make_pair(stValue.dwGemID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StGemInfo* CConfigData::GetGemInfo(UINT32 dwGemID)
{
	ERROR_RETURN_NULL(dwGemID != 0);
	auto itor = m_mapGemInfo.find(dwGemID);
	if (itor != m_mapGemInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadPetInfo(CppSQLite3Query& QueryData)
{
	m_mapPetInfo.clear();

	while(!QueryData.eof())
	{
		StPetInfo stValue;
		stValue.dwPetID = QueryData.getIntField("Id");
		m_mapPetInfo.insert(std::make_pair(stValue.dwPetID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StPetInfo* CConfigData::GetPetInfo(UINT32 dwPetID)
{
	ERROR_RETURN_NULL(dwPetID != 0);
	auto itor = m_mapPetInfo.find(dwPetID);
	if(itor != m_mapPetInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadPartnerInfo(CppSQLite3Query& QueryData)
{
	m_mapPartnerInfo.clear();

	while(!QueryData.eof())
	{
		StPartnerInfo stValue;
		stValue.dwPartnerID = QueryData.getIntField("Id");
		stValue.dwActorID = QueryData.getIntField("ActorId");
		m_mapPartnerInfo.insert(std::make_pair(stValue.dwPartnerID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StPartnerInfo* CConfigData::GetPartnerInfo(UINT32 dwPartnerID)
{
	ERROR_RETURN_NULL(dwPartnerID != 0);
	auto itor = m_mapPartnerInfo.find(dwPartnerID);
	if(itor != m_mapPartnerInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadTaskInfo(CppSQLite3Query& QueryData)
{
	m_mapTaskInfo.clear();

	while(!QueryData.eof())
	{
		StTaskInfo stValue;
		m_mapTaskInfo.insert(std::make_pair(stValue.TaskID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StTaskInfo* CConfigData::GetTaskInfo(UINT32 dwTaskID)
{
	ERROR_RETURN_NULL(dwTaskID != 0);
	auto itor = m_mapTaskInfo.find(dwTaskID);
	if(itor != m_mapTaskInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadSkillInfo(CppSQLite3Query& QueryData)
{
	m_mapSkillInfo.clear();

	while(!QueryData.eof())
	{
		StSkillInfo stValue;
		stValue.SkillID = QueryData.getIntField("id");
		stValue.Level = QueryData.getIntField("level");
		UINT32 dwNewID = stValue.Level << 20 | stValue.SkillID;
		m_mapSkillInfo.insert(std::make_pair(dwNewID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StSkillInfo* CConfigData::GetSkillInfo(UINT32 dwSkillID, UINT32 dwLevel)
{
	ERROR_RETURN_NULL(dwSkillID != 0);
	ERROR_RETURN_NULL(dwLevel != 0);
	UINT32 dwNewID = dwLevel << 20 | dwSkillID;
	auto itor = m_mapSkillInfo.find(dwNewID);
	if(itor != m_mapSkillInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadBuffInfo(CppSQLite3Query& QueryData)
{
	m_mapBuffInfo.clear();

	while(!QueryData.eof())
	{
		StBuffInfo stValue;
		stValue.BuffID = QueryData.getIntField("id");

		m_mapBuffInfo.insert(std::make_pair(stValue.BuffID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StBuffInfo* CConfigData::GetBuffInfo(UINT32 dwBuffID)
{
	ERROR_RETURN_NULL(dwBuffID != 0);
	auto itor = m_mapBuffInfo.find(dwBuffID);
	if(itor != m_mapBuffInfo.end())
	{
		return &itor->second;
	}

	return NULL;
}

BOOL CConfigData::ReadStoreInfo(CppSQLite3Query& QueryData)
{
	m_mapStoreInfo.clear();

	while (!QueryData.eof())
	{
		StStoreItemInfo stValue;
		stValue.StoreID = QueryData.getIntField("Id");
		stValue.ItemID = QueryData.getIntField("ItemID");
		stValue.ItemNum = QueryData.getIntField("ItemNum");
		stValue.CostMoneyID = QueryData.getIntField("CostMoneyID");
		stValue.CostMoneyNum = QueryData.getIntField("CostMoneyNum");
		stValue.StoreType = QueryData.getIntField("StoreType");
		m_mapStoreInfo.insert(std::make_pair(stValue.StoreID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StStoreItemInfo* CConfigData::GetStoreItemInfo(UINT32 dwStoreID)
{
	ERROR_RETURN_NULL(dwStoreID != 0);
	auto itor = m_mapStoreInfo.find(dwStoreID);
	if (itor != m_mapStoreInfo.end())
	{
		return &itor->second;
	}

	return NULL;
}

BOOL CConfigData::ReadActivityInfo(CppSQLite3Query& QueryData)
{
	m_mapActivityInfo.clear();

	while(!QueryData.eof())
	{
		StActivityInfo stValue;
		stValue.ActivityID = QueryData.getIntField("id");

		m_mapActivityInfo.insert(std::make_pair(stValue.ActivityID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StActivityInfo* CConfigData::GetActivityInfo(UINT32 dwActivityID)
{
	ERROR_RETURN_NULL(dwActivityID != 0);
	auto itor = m_mapActivityInfo.find(dwActivityID);
	if(itor != m_mapActivityInfo.end())
	{
		return &itor->second;
	}

	return NULL;
}

StItemInfo* CConfigData::GetItemInfo(UINT32 dwItemID)
{
	ERROR_RETURN_NULL(dwItemID != 0);
	auto itor = m_mapItem.find(dwItemID);
	if(itor != m_mapItem.end())
	{
		return &itor->second;
	}

	return NULL;
}

