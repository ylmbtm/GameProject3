#include <stdafx.h>
#include "ConfigData.h"
#include "Utility\CommonConvert.h"
#include "Utility\Log\Log.h"
#include "Utility\CommonFunc.h"

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
	m_vtDataFuncList.push_back(DataFuncNode("Data_Role", &CConfigData::ReadCarrer));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Actor", &CConfigData::ReadActor));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Copy", &CConfigData::ReadCopyInfo));
	m_vtDataFuncList.push_back(DataFuncNode("Data_Item", &CConfigData::ReadItemData));
	return TRUE;
}

BOOL CConfigData::ReadConfigData(std::string strDbFile)
{
	try
	{
		m_DBConnection.open(strDbFile.c_str());
	}
	catch(CppSQLite3Exception& e)
	{
		printf("%s", e.errorMessage());
		return FALSE;
	}

	char szSql[1024]  = {0};
	for(std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
	{
		DataFuncNode dataNode = (*itor);
		sprintf(szSql, "select * from %s;", dataNode.m_strTbName.c_str());
		CppSQLite3Query Tabledatas = m_DBConnection.execQuery(szSql);
		(this->*dataNode.m_pDataFunc)(Tabledatas);
	}

	m_DBConnection.close();

	return TRUE;
}


BOOL CConfigData::ReadConstantValue(CppSQLite3Query& QueryData)
{
	while(QueryData.eof())
	{
		std::string strName = QueryData.getStringField("name");
		std::string strValue = QueryData.getStringField("value");

		if(strName == "xxxxx")
		{
			m_ConstantValue.xxxx = CommonConvert::StringToInt(strValue.c_str());
		}

		QueryData.nextRow();
	}

	return TRUE;
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
	while(!QueryData.eof())
	{
		StCarrerInfo stValue;
		stValue.dwID = QueryData.getIntField("Carrer");
		stValue.dwActorID = QueryData.getIntField("ActorID");
		m_mapCarrer.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StCarrerInfo* CConfigData::GetCarrerInfo(UINT32 dwCarrerID)
{
	std::map<UINT32, StCarrerInfo>::iterator itor = m_mapCarrer.find(dwCarrerID);
	if(itor != m_mapCarrer.end())
	{
		return &itor->second;
	}

    ASSERT_FAIELD;
	return NULL;
}

BOOL CConfigData::ReadActor(CppSQLite3Query& QueryData)
{
	while(!QueryData.eof())
	{
		StActor stValue;
		stValue.dwID = QueryData.getIntField("Id");
		m_mapActor.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StActor* CConfigData::GetActorInfo(UINT32 dwActorID)
{
	std::map<UINT32, StActor>::iterator itor = m_mapActor.find(dwActorID);
	if(itor != m_mapActor.end())
	{
		return &itor->second;
	}
    ASSERT_FAIELD;
	return NULL;
}

BOOL CConfigData::ReadCopyInfo(CppSQLite3Query& QueryData)
{
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
	std::map<UINT32, StCopyInfo>::iterator itor = m_mapCopyInfo.find(dwCopyID);
	if(itor != m_mapCopyInfo.end())
	{
		return &itor->second;
	}
    ASSERT_FAIELD;
	return NULL;
}

BOOL CConfigData::ReadLanguage(CppSQLite3Query& QueryData)
{
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
		m_mapLocalString.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

std::string& CConfigData::GetLanguageText( UINT32 dwID, UINT32 dwLang )
{
	if((dwLang < 0) || (dwLang > 14))
	{
		return m_strNull;
	}

	std::map<UINT32, StLocalString>::iterator itor = m_mapLocalString.find(dwID);
	if(itor != m_mapLocalString.end())
	{
		return itor->second.Language[dwLang];
	}

	return m_strNull;
}

BOOL CConfigData::ReadAwardData(CppSQLite3Query& QueryData)
{
	while(QueryData.eof())
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

	for(auto i = 0; i < AwardItemList.size(); i++)
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
	while(QueryData.eof())
	{
		StItemInfo stValue;
		stValue.dwItemID = QueryData.getIntField("id");
		stValue.dwType = QueryData.getIntField("type");
		stValue.SellID = QueryData.getIntField("sell_money_id");
		stValue.SellPrice = QueryData.getIntField("sell_money_num");
		stValue.Quality = QueryData.getIntField("quality");
		stValue.UseType = QueryData.getIntField("usetype");
		stValue.Data1 = QueryData.getIntField("data1");
		stValue.Data2 = QueryData.getIntField("data2");
		m_mapItem.insert(std::make_pair(stValue.dwItemID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

BOOL CConfigData::ReadFuncInfo(CppSQLite3Query& QueryData)
{
	while(QueryData.eof())
	{
		StFuncInfo stValue;
		m_mapFuncInfo.insert(std::make_pair(stValue.dwFuncID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StFuncInfo* CConfigData::GetFuncInfo(UINT32 dwFuncID)
{
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
	else
	{
		ASSERT_FAIELD;
	}

	return FALSE;
}

BOOL CConfigData::ReadFuncVipInfo(CppSQLite3Query& QueryData)
{
	while(QueryData.eof())
	{
		StFuncVipInfo stValue;
		m_mapFuncVipInfo.insert(std::make_pair(stValue.dwFuncID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StFuncVipInfo* CConfigData::GetFuncVipInfo(UINT32 dwFuncID)
{
	auto itor = m_mapFuncVipInfo.find(dwFuncID);
	if(itor != m_mapFuncVipInfo.end())
	{
		return &itor->second;
	}

	return NULL;
}

BOOL CConfigData::ReadFuncCostInfo(CppSQLite3Query& QueryData)
{
	while(QueryData.eof())
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
	while(QueryData.eof())
	{
		StEquipInfo stValue;
		m_mapEquipInfo.insert(std::make_pair(stValue.dwEquipID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StEquipInfo* CConfigData::GetEquipInfo(UINT32 dwEquipID)
{
	auto itor = m_mapEquipInfo.find(dwEquipID);
	if(itor != m_mapEquipInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadPetInfo(CppSQLite3Query& QueryData)
{
	while(QueryData.eof())
	{
		StPetInfo stValue;
		m_mapPetInfo.insert(std::make_pair(stValue.dwPetID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StPetInfo* CConfigData::GetPetInfo(UINT32 dwPetID)
{
	auto itor = m_mapPetInfo.find(dwPetID);
	if(itor != m_mapPetInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

BOOL CConfigData::ReadPartnerInfo(CppSQLite3Query& QueryData)
{
	while(QueryData.eof())
	{
		StPartnerInfo stValue;
		m_mapPartnerInfo.insert(std::make_pair(stValue.dwPartnerID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StPartnerInfo* CConfigData::GetPartnerInfo(UINT32 dwPartnerID)
{
	auto itor = m_mapPartnerInfo.find(dwPartnerID);
	if(itor != m_mapPartnerInfo.end())
	{
		return &itor->second;
	}
	return NULL;
}

StItemInfo* CConfigData::GetItemInfo(UINT32 dwItemID)
{
	auto itor = m_mapItem.find(dwItemID);
	if(itor != m_mapItem.end())
	{
		return &itor->second;
	}

	return NULL;
}
