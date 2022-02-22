#include "stdafx.h"
#include "StaticData.h"
#include "RapidXml.h"
#include "ConfigFile.h"
//#include "DBInterface/CppMysql.h"

CStaticData::CStaticData()
{
    InitDataReader();
}

CStaticData::~CStaticData()
{

}

CStaticData* CStaticData::GetInstancePtr()
{
    static CStaticData _StaticMgr;

    return &_StaticMgr;
}

BOOL CStaticData::InitDataReader()
{
    m_vtDataFuncList.push_back(DataFuncNode("Data_Constant",    &CStaticData::ReadConstantData));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Role",        &CStaticData::ReadCarrer));
    m_vtDataFuncList.push_back(DataFuncNode("Data_RoleLevel",   &CStaticData::ReadCarrerLevel));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Actor",       &CStaticData::ReadActor));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Copy",        &CStaticData::ReadCopyInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Item",        &CStaticData::ReadItemData));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Action",      &CStaticData::ReadActionCfg));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Actor_Skill", &CStaticData::ReadActorSkillInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Equip",       &CStaticData::ReadEquipInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Gem",         &CStaticData::ReadGemInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Partner",     &CStaticData::ReadPartnerInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Mount",       &CStaticData::ReadMountInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Store",       &CStaticData::ReadStoreInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Combo_Skill", &CStaticData::ReadComboSkillInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Skill",       &CStaticData::ReadSkillInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_FlyObject",   &CStaticData::ReadBulletInfo));
    m_vtDataFuncList.push_back(DataFuncNode("Data_Pet",         &CStaticData::ReadPetInfo));

//  m_vtDataFuncList.push_back(DataFuncNode("Data_Language",    &CStaticData::ReadLanguage));
//  m_vtDataFuncList.push_back(DataFuncNode("Data_Award",       &CStaticData::ReadAwardData));
//  m_vtDataFuncList.push_back(DataFuncNode("Data_Func",        &CStaticData::ReadFuncInfo));
//  m_vtDataFuncList.push_back(DataFuncNode("Data_Func_Vip",    &CStaticData::ReadFuncVipInfo));
//  m_vtDataFuncList.push_back(DataFuncNode("Data_Func_Cost",   &CStaticData::ReadFuncCostInfo));
//  m_vtDataFuncList.push_back(DataFuncNode("Data_Task",        &CStaticData::ReadTaskInfo));
//  m_vtDataFuncList.push_back(DataFuncNode("Data_Buff",        &CStaticData::ReadBuffInfo));

    return TRUE;
}

BOOL CStaticData::LoadConfigData(std::string strDbFile)
{
#define  SQLITE_TYPE 1
#ifdef SQLITE_TYPE
    try
    {
        CppSQLite3DB    tDBConnection;
        tDBConnection.open(strDbFile.c_str());
        char szSql[SQL_BUFF_LEN] = { 0 };
        for (std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
        {
            DataFuncNode dataNode = (*itor);
            snprintf(szSql, SQL_BUFF_LEN, "select * from %s;", dataNode.m_strTbName.c_str());
            CppSQLite3Query Tabledatas = tDBConnection.execQuery(szSql);
            (this->*dataNode.m_pDataFunc)(Tabledatas);
        }
        tDBConnection.close();
    }
    catch(CppSQLite3Exception& e)
    {
        CLog::GetInstancePtr()->LogError("CConfigData::LoadConfigData Failed!!!, Reason:%s", e.errorMessage());
        return FALSE;
    }
#else
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_type_svr_ip");
    INT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_type_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_type_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_type_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_type_svr_db_name");

    CppMySQL3DB tDBConnection;
    if (!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        return FALSE;
    }
    char szSql[SQL_BUFF_LEN] = { 0 };
    for (std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
    {
        DataFuncNode dataNode = (*itor);
        snprintf(szSql, SQL_BUFF_LEN, "select * from %s;", dataNode.m_strTbName.c_str());
        CppMySQLQuery Tabledatas = tDBConnection.querySQL(szSql);
        (this->*dataNode.m_pDataFunc)(Tabledatas);
    }
    tDBConnection.close();
#endif
    ReadSkillEvent();

    return TRUE;
}

BOOL CStaticData::ReloadConfigData( std::string strTbName )
{
    try
    {
        CppSQLite3DB    tDBConnection;
        tDBConnection.open("Config.db");
        char szSql[SQL_BUFF_LEN] = { 0 };
        for (std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
        {
            DataFuncNode dataNode = (*itor);
            if (dataNode.m_strTbName != strTbName)
            {
                continue;
            }

            snprintf(szSql, SQL_BUFF_LEN, "select * from %s;", dataNode.m_strTbName.c_str());
            CppSQLite3Query Tabledatas = tDBConnection.execQuery(szSql);
            (this->*dataNode.m_pDataFunc)(Tabledatas);
        }

        tDBConnection.close();
    }
    catch(CppSQLite3Exception& e)
    {
        CLog::GetInstancePtr()->LogError("CConfigData::ReloadConfigData Failed!!!, Reason:%s", e.errorMessage());
        return FALSE;
    }

    return TRUE;
}

BOOL CStaticData::ReadConstantData(CppSQLite3Query& QueryData)
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

INT32 CStaticData::GetConstantValue(std::string& strName)
{
    std::map<std::string, INT32>::iterator itor = m_mapConstantValue.find(strName);
    if (itor != m_mapConstantValue.end())
    {
        return itor->second;
    }

    return 0;
}


INT32 CStaticData::GetConstantValue(char* pszName)
{
    std::map<std::string, INT32>::iterator itor = m_mapConstantValue.find(pszName);
    if (itor != m_mapConstantValue.end())
    {
        return itor->second;
    }

    return 0;
}

INT64 CStaticData::GetActoinMaxValue(UINT32 dwActionID)
{
    if((dwActionID <= 0) || (dwActionID >= m_vtActionList.size()))
    {
        return 1;
    }

    return m_vtActionList.at(dwActionID - 1).dwMax;
}

UINT32 CStaticData::GetActoinUnitTime(UINT32 dwActionID)
{
    if((dwActionID <= 0) || (dwActionID >= m_vtActionList.size()))
    {
        return 1;
    }

    return m_vtActionList.at(dwActionID - 1).UnitTime;
}

BOOL CStaticData::ReadCarrer(CppSQLite3Query& QueryData)
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

StCarrerInfo* CStaticData::GetCarrerInfo(UINT32 dwCarrerID)
{
    std::map<UINT32, StCarrerInfo>::iterator itor = m_mapCarrer.find(dwCarrerID);

    ERROR_RETURN_NULL(itor != m_mapCarrer.end());

    return &itor->second;
}


BOOL CStaticData::ReadActionCfg(CppSQLite3Query& QueryData)
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

BOOL CStaticData::ReadCarrerLevel(CppSQLite3Query& QueryData)
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

StLevelInfo* CStaticData::GetCarrerLevelInfo(UINT32 dwCarrerID, UINT32 dwLevel)
{
    return &m_CarrerLevel[dwCarrerID - 1][dwLevel - 1];
}

BOOL CStaticData::ReadActor(CppSQLite3Query& QueryData)
{
    m_mapActor.clear();

    while(!QueryData.eof())
    {
        StActorInfo stValue;
        stValue.dwID        = QueryData.getIntField("Id");
        stValue.InitLevel   = QueryData.getIntField("Level");
        stValue.fDefSpeed   = QueryData.getFloatField("DefSpeed");
        stValue.fRadius     = QueryData.getFloatField("Radius");
        stValue.dwType      = QueryData.getIntField("Type");
        stValue.AiID        = QueryData.getIntField("AiId");
        int nIndex          = QueryData.fieldIndex("P1");
        for(int i = 0; i < PROPERTY_NUM; i++)
        {
            stValue.Propertys[i] = QueryData.getIntField(i + nIndex, 0);
        }

        m_mapActor.insert(std::make_pair(stValue.dwID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StActorInfo* CStaticData::GetActorInfo(UINT32 dwActorID)
{
    ERROR_RETURN_NULL(dwActorID != 0);
    std::map<UINT32, StActorInfo>::iterator itor = m_mapActor.find(dwActorID);
    if(itor != m_mapActor.end())
    {
        return &itor->second;
    }
    return NULL;
}

BOOL CStaticData::ReadActorSkillInfo(CppSQLite3Query& QueryData)
{
    m_mapActorSkill.clear();

    while (!QueryData.eof())
    {
        StActorSkillInfo stValue;
        stValue.dwActorID = QueryData.getIntField("Id");
        stValue.NormalID = QueryData.getIntField("Normal1");
        stValue.Specials[0] = QueryData.getIntField("Special1");
        stValue.Specials[1] = QueryData.getIntField("Special2");
        stValue.Specials[2] = QueryData.getIntField("Special3");
        stValue.Specials[3] = QueryData.getIntField("Special4");
        stValue.Specials[4] = QueryData.getIntField("Special5");
        m_mapActorSkill.insert(std::make_pair(stValue.dwActorID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StActorSkillInfo* CStaticData::GetActorSkillInfo(UINT32 dwActorID)
{
    ERROR_RETURN_NULL(dwActorID != 0);
    std::map<UINT32, StActorSkillInfo>::iterator itor = m_mapActorSkill.find(dwActorID);
    if (itor != m_mapActorSkill.end())
    {
        return &itor->second;
    }
    return NULL;
}

BOOL CStaticData::ReadCopyInfo(CppSQLite3Query& QueryData)
{
    m_mapCopyInfo.clear();

    while(!QueryData.eof())
    {
        StCopyInfo stValue;
        stValue.dwCopyID        = QueryData.getIntField("Id");
        stValue.dwCopyType      = QueryData.getIntField("CopyType");
        stValue.dwCostActID     = QueryData.getIntField("CostActionId");
        stValue.dwCostActNum    = QueryData.getIntField("CostActionNum");
        stValue.dwGetMoneyID    = QueryData.getIntField("GetMoneyId");
        stValue.dwGetMoneyRatio = QueryData.getIntField("GetMoneyRatio");
        m_mapCopyInfo.insert(std::make_pair(stValue.dwCopyID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StCopyInfo* CStaticData::GetCopyInfo(UINT32 dwCopyID)
{
    ERROR_RETURN_NULL(dwCopyID != 0);
    std::map<UINT32, StCopyInfo>::iterator itor = m_mapCopyInfo.find(dwCopyID);
    if(itor != m_mapCopyInfo.end())
    {
        return &itor->second;
    }
    return NULL;
}

UINT32 CStaticData::GetCopyType(UINT32 dwCopyID)
{
    StCopyInfo* pCopyInfo = GetCopyInfo(dwCopyID);
    ERROR_RETURN_VALUE(pCopyInfo != NULL, 0);
    return pCopyInfo->dwCopyType;
}

BOOL CStaticData::ReadLanguage(CppSQLite3Query& QueryData)
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

std::string& CStaticData::GetLanguageText( UINT32 dwID, UINT32 dwLang )
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

std::string& CStaticData::GetLanguageText(std::string strID, UINT32 dwLang)
{
    static std::string strNull = "null";
    return strNull;
}

BOOL CStaticData::ReadAwardData(CppSQLite3Query& QueryData)
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
            std::vector<std::string> vtRet;
            CommonConvert::SpliteString(strFixDrop, ")(", vtRet);

            INT32 nCheckRatio = 0;

            StDropItem item;
            for(std::vector<std::string>::size_type i = 0; i < vtRet.size(); i++)
            {
                ParseToDropItem(vtRet.at(i), item);
                stValue.RatioItems.push_back(item);
                nCheckRatio += item.dwRatio;
            }

            if (nCheckRatio != 10000)
            {
                CLog::GetInstancePtr()->LogError("ReadAwardData Error: Invalid awardid :%d", stValue.dwAwardID);
            }
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

BOOL CStaticData::ParseToDropItem(std::string strDrop, StDropItem& item)
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

BOOL CStaticData::GetAwardItem(INT32 nAwardID, INT32 nCarrer, StAwardItem& AwardItem)
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

BOOL CStaticData::GetAwardItemByIndex(INT32 nAwardID, INT32 nCarrer, INT32 nIndex, StItemData& ItemData)
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

BOOL CStaticData::GetItemsFromAwardID(INT32 nAwardID, INT32 nCarrer, std::vector<StItemData>& vtItemList)
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

    //多次可取到同样的物品
    for (int  cycle = 0; cycle < AwardItem.dwRatioCount; cycle++ )
    {
        UINT32 dwRandValue = CommonFunc::GetRandNum(0);
        for (std::vector<StDropItem>::size_type i = 0; i < AwardItem.RatioItems.size() - 1; i++)
        {
            if (dwRandValue <= AwardItem.RatioItems[i].dwRatio)
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
            else
            {
                dwRandValue -= AwardItem.RatioItems[i].dwRatio;
            }
        }
    }

    //确保多次都取到不同样的物品
    /*
    bool UsedFlag[100] = { 0 };
    UINT32 UsedValue = 0;
    for (int cycle = 0; cycle < AwardItem.dwRatioCount; cycle++)
    {
        UINT32 dwRandValue = CommonFunc::GetRandNum(0) - UsedValue;
        for (std::vector<StDropItem>::size_type i = 0; i < AwardItem.RatioItems.size() - 1; i++)
        {
            if (UsedFlag[i])
            {
                continue;
            }

            if (dwRandValue <= AwardItem.RatioItems[i].dwRatio)
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
                    UsedFlag[i] = true;
                    UsedValue += AwardItem.RatioItems[i].dwRatio;
                }
            }
            else
            {
                dwRandValue -= AwardItem.RatioItems[i].dwRatio;
            }
        }
    }
    */

    return TRUE;
}

BOOL CStaticData::GetItemsAwardIDTimes(INT32 nAwardID, INT32 nCarrer, INT32 nTimes, std::vector<StItemData>& vtItemList)
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

BOOL CStaticData::ReadItemData(CppSQLite3Query& QueryData)
{
    m_mapItem.clear();

    while(!QueryData.eof())
    {
        StItemInfo stValue;
        stValue.dwItemID = QueryData.getIntField("Id");
        stValue.eItemType = (EItemType)QueryData.getIntField("ItemType");
        stValue.dwBagType = QueryData.getIntField("BagType");
        stValue.SellID = QueryData.getIntField("SellMoneyId");
        stValue.SellPrice = QueryData.getIntField("SellMoneyNum");
        stValue.Quality = QueryData.getIntField("Quality");
        stValue.StackMax = QueryData.getIntField("StackMax");
        stValue.CarrerID = QueryData.getIntField("Carrer");
        stValue.Data1 = QueryData.getIntField("Data1");
        stValue.Data2 = QueryData.getIntField("Data2");
        m_mapItem.insert(std::make_pair(stValue.dwItemID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

BOOL CStaticData::ReadFuncInfo(CppSQLite3Query& QueryData)
{
    while(!QueryData.eof())
    {
        StFuncInfo stValue;
        m_mapFuncInfo.insert(std::make_pair(stValue.dwFuncID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StFuncInfo* CStaticData::GetFuncInfo(UINT32 dwFuncID)
{
    ERROR_RETURN_NULL(dwFuncID != 0);
    auto itor = m_mapFuncInfo.find(dwFuncID);
    if(itor != m_mapFuncInfo.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::IsFuncOpen(UINT32 dwFuncID, INT32 level, INT32 viplevel)
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

BOOL CStaticData::ReadFuncVipInfo(CppSQLite3Query& QueryData)
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

StFuncVipInfo* CStaticData::GetFuncVipInfo(UINT32 dwFuncID)
{
    ERROR_RETURN_NULL(dwFuncID != 0);
    auto itor = m_mapFuncVipInfo.find(dwFuncID);
    if(itor != m_mapFuncVipInfo.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::ReadFuncCostInfo(CppSQLite3Query& QueryData)
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

INT32 CStaticData::GetFuncCostInfo(UINT32 dwFuncID, INT32 Times)
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

BOOL CStaticData::ReadEquipInfo(CppSQLite3Query& QueryData)
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

StEquipInfo* CStaticData::GetEquipInfo(UINT32 dwEquipID)
{
    ERROR_RETURN_NULL(dwEquipID != 0);
    auto itor = m_mapEquipInfo.find(dwEquipID);
    if(itor != m_mapEquipInfo.end())
    {
        return &itor->second;
    }
    return NULL;
}


BOOL CStaticData::ReadGemInfo(CppSQLite3Query& QueryData)
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

StGemInfo* CStaticData::GetGemInfo(UINT32 dwGemID)
{
    ERROR_RETURN_NULL(dwGemID != 0);
    auto itor = m_mapGemInfo.find(dwGemID);
    if (itor != m_mapGemInfo.end())
    {
        return &itor->second;
    }
    return NULL;
}

BOOL CStaticData::ReadPetInfo(CppSQLite3Query& QueryData)
{
    m_mapPetInfo.clear();

    while(!QueryData.eof())
    {
        StPetInfo stValue;
        stValue.dwPetID = QueryData.getIntField("Id");
        stValue.dwActorID = QueryData.getIntField("ActorId");
        m_mapPetInfo.insert(std::make_pair(stValue.dwPetID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StPetInfo* CStaticData::GetPetInfo(UINT32 dwPetID)
{
    ERROR_RETURN_NULL(dwPetID != 0);
    auto itor = m_mapPetInfo.find(dwPetID);
    if(itor != m_mapPetInfo.end())
    {
        return &itor->second;
    }
    return NULL;
}

BOOL CStaticData::ReadPartnerInfo(CppSQLite3Query& QueryData)
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

StPartnerInfo* CStaticData::GetPartnerInfo(UINT32 dwPartnerID)
{
    ERROR_RETURN_NULL(dwPartnerID != 0);
    auto itor = m_mapPartnerInfo.find(dwPartnerID);
    if(itor != m_mapPartnerInfo.end())
    {
        return &itor->second;
    }
    return NULL;
}

BOOL CStaticData::ReadMountInfo(CppSQLite3Query& QueryData)
{
    m_mapMountInfo.clear();

    while (!QueryData.eof())
    {
        StMountInfo stValue;
        stValue.dwMountID = QueryData.getIntField("Id");
        stValue.dwMountID = QueryData.getIntField("ActorId");
        m_mapMountInfo.insert(std::make_pair(stValue.dwMountID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StMountInfo* CStaticData::GetMountInfo(UINT32 dwMountID)
{
    ERROR_RETURN_NULL(dwMountID != 0);
    auto itor = m_mapMountInfo.find(dwMountID);
    if (itor != m_mapMountInfo.end())
    {
        return &itor->second;
    }
    return NULL;
}

BOOL CStaticData::ReadTaskInfo(CppSQLite3Query& QueryData)
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

StTaskInfo* CStaticData::GetTaskInfo(UINT32 dwTaskID)
{
    ERROR_RETURN_NULL(dwTaskID != 0);
    auto itor = m_mapTaskInfo.find(dwTaskID);
    if(itor != m_mapTaskInfo.end())
    {
        return &itor->second;
    }
    return NULL;
}

BOOL CStaticData::ReadSkillInfo(CppSQLite3Query& QueryData)
{
    m_mapSkillInfo.clear();

    while(!QueryData.eof())
    {
        StSkillInfo stValue;
        stValue.SkillID = QueryData.getIntField("Id");
        stValue.Level = QueryData.getIntField("Level");
        stValue.CD = QueryData.getIntField("CountDown");
        stValue.HurtFix = QueryData.getIntField("HurtFix");
        stValue.HurtMuti = QueryData.getIntField("HurtMuti");
        stValue.SkillType = QueryData.getIntField("SkillType");
        stValue.HitShipType = (EHitShipType)QueryData.getIntField("HitShipType");
        stValue.HitMyself = QueryData.getIntField("HitMyself");
        UINT32 dwNewID = stValue.Level << 20 | stValue.SkillID;
        m_mapSkillInfo.insert(std::make_pair(dwNewID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StSkillInfo* CStaticData::GetSkillInfo(UINT32 dwSkillID, UINT32 dwLevel)
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

BOOL CStaticData::ReadSkillEvent()
{
    std::string strPath = "Skill/Battle_Skill.xml";

    rapidxml::xml_document<char>* pXMLDoc = NULL;

    FILE* pFile = fopen(strPath.c_str(), "rb");
    ERROR_RETURN_FALSE(pFile != NULL);
    fseek(pFile, 0, SEEK_END);
    INT32 nSize = ftell(pFile);
    rewind(pFile);
    char* pBuff = (char*)malloc(nSize + 1);
    memset(pBuff, 0, nSize + 1);
    fread(pBuff, nSize, 1, pFile);
    fclose(pFile);
    pXMLDoc = new rapidxml::xml_document<char>();
    pXMLDoc->parse<0>(pBuff);

    rapidxml::xml_node<char>* pXmlRoot = pXMLDoc->first_node("Root");
    ERROR_RETURN_FALSE(pXmlRoot != NULL);

    for (auto pSkillNode = pXmlRoot->first_node("Skill"); pSkillNode != NULL; pSkillNode = pSkillNode->next_sibling("Skill"))
    {
        StSkillEventInfo tSkillEventInfo;

        //取技能ID
        auto pAttr = pSkillNode->first_attribute("ID", strlen("ID"), false);
        tSkillEventInfo.nSkillID = CommonConvert::StringToInt(pAttr->value());

        pAttr = pSkillNode->first_attribute("Duration", strlen("Duration"), false);
        tSkillEventInfo.uDuration = (UINT64)(CommonConvert::StringToFloat(pAttr->value()) * 1000);

        pAttr = pSkillNode->first_attribute("CastType", strlen("CastType"), false);
        tSkillEventInfo.dwCastType = CommonConvert::StringToInt(pAttr->value());

        for (auto pEventNode = pSkillNode->first_node("ActScope"); pEventNode != NULL; pEventNode = pEventNode->next_sibling("ActScope"))
        {
            StSkillEvent tEvent;
            pAttr = pEventNode->first_attribute("RangeType", strlen("RangeType"), false);
            if (pAttr == NULL)
            {
                continue;
            }

            tEvent.RangeType = (ERangeType)CommonConvert::StringToInt(pAttr->value());;
            pAttr = pEventNode->first_attribute("RangeParams", strlen("RangeParams"), false);
            if (pAttr == NULL)
            {
                continue;
            }
            CommonConvert::StringToVector(pAttr->value(), tEvent.RangeParams, 5, '~');;


            pAttr = pEventNode->first_attribute("StTime", strlen("StTime"), false);
            if (pAttr == NULL)
            {
                continue;
            }

            tEvent.TrigerTime = (UINT64)(CommonConvert::StringToFloat(pAttr->value()) * 1000);

            pAttr = pEventNode->first_attribute("HitActionID", strlen("HitActionID"), false);
            if (pAttr == NULL)
            {
                continue;
            }

            tEvent.HitActionID = (UINT32)CommonConvert::StringToInt(pAttr->value());

            pAttr = pEventNode->first_attribute("HitEffectID", strlen("HitEffectID"), false);
            if (pAttr == NULL)
            {
                continue;
            }

            tEvent.HitEffect = (UINT32)CommonConvert::StringToInt(pAttr->value());

            pAttr = pEventNode->first_attribute("HitDistance", strlen("HitDistance"), false);
            if (pAttr == NULL)
            {
                continue;
            }

            tEvent.HitDistance = CommonConvert::StringToFloat(pAttr->value());

            //////////////////////////////////////////////////////////////////////////
            //解析子弹

            for (auto pBulletNode = pEventNode->first_node("ActFlyObject"); pBulletNode != NULL; pBulletNode = pBulletNode->next_sibling("ActFlyObject"))
            {
                StBulletObject bulletObject;

                pAttr = pBulletNode->first_attribute("ID", strlen("ID"), false);
                if (pAttr == NULL)
                {
                    continue;
                }
                bulletObject.BulletID = CommonConvert::StringToInt(pAttr->value());

                pAttr = pBulletNode->first_attribute("Angle", strlen("Angle"), false);
                if (pAttr == NULL)
                {
                    continue;
                }
                bulletObject.fAngle = CommonConvert::StringToFloat(pAttr->value());

                tEvent.vtBullets.push_back(bulletObject);
            }

            tSkillEventInfo.vtEvents.push_back(tEvent);
        }

        m_mapSkillEvent.insert(std::make_pair(tSkillEventInfo.nSkillID, tSkillEventInfo));
    }

    return TRUE;
}

StSkillEventInfo* CStaticData::GetSkillEventInfo(UINT32 dwSkillID)
{
    ERROR_RETURN_NULL(dwSkillID != 0);
    auto itor = m_mapSkillEvent.find(dwSkillID);
    if (itor != m_mapSkillEvent.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::ReadComboSkillInfo(CppSQLite3Query& QueryData)
{
    m_mapComboSkill.clear();

    while (!QueryData.eof())
    {
        StComboSkillInfo stValue;
        stValue.SkillID = QueryData.getIntField("SkillId");

        INT32 nValue = QueryData.getIntField("Combo1");
        if (nValue != 0)
        {
            stValue.vtComboSkill.push_back(nValue);
        }

        nValue = QueryData.getIntField("Combo2");
        if (nValue != 0)
        {
            stValue.vtComboSkill.push_back(nValue);
        }

        nValue = QueryData.getIntField("Combo3");
        if (nValue != 0)
        {
            stValue.vtComboSkill.push_back(nValue);
        }

        nValue = QueryData.getIntField("Combo4");
        if (nValue != 0)
        {
            stValue.vtComboSkill.push_back(nValue);
        }

        m_mapComboSkill.insert(std::make_pair(stValue.SkillID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StComboSkillInfo* CStaticData::GetComboSkillInfo(UINT32 dwSkillID)
{
    ERROR_RETURN_NULL(dwSkillID != 0);
    auto itor = m_mapComboSkill.find(dwSkillID);
    if (itor != m_mapComboSkill.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::ReadBuffInfo(CppSQLite3Query& QueryData)
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

StBuffInfo* CStaticData::GetBuffInfo(UINT32 dwBuffID)
{
    ERROR_RETURN_NULL(dwBuffID != 0);
    auto itor = m_mapBuffInfo.find(dwBuffID);
    if(itor != m_mapBuffInfo.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::ReadBulletInfo(CppSQLite3Query& QueryData)
{
    m_mapBulletInfo.clear();

    while (!QueryData.eof())
    {
        StBulletInfo stValue;
        stValue.BulletID = QueryData.getIntField("Id");
        stValue.BulletType = (EBulletType)QueryData.getIntField("Type");
        stValue.InitSpeed = QueryData.getFloatField("InitSpeed");
        stValue.AccSpeed = QueryData.getFloatField("AcceSpeed");
        stValue.LifeTime = QueryData.getIntField("LifeTime");
        stValue.RangeType = (ERangeType)QueryData.getIntField("RangeType");
        CommonConvert::StringToVector(QueryData.getStringField("RangeParams"), stValue.RangeParams, 5, '~');
        m_mapBulletInfo.insert(std::make_pair(stValue.BulletID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StBulletInfo* CStaticData::GetBulletInfo(UINT32 dwBulletID)
{
    ERROR_RETURN_NULL(dwBulletID != 0);
    auto itor = m_mapBulletInfo.find(dwBulletID);
    if (itor != m_mapBulletInfo.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::ReadChargeInfo(CppSQLite3Query& QueryData)
{
    m_mapChargeInfo.clear();

    while (!QueryData.eof())
    {
        StChargeInfo stValue;
        stValue.dwProductID = QueryData.getIntField("Id");
        m_mapChargeInfo.insert(std::make_pair(stValue.dwProductID, stValue));
        QueryData.nextRow();
    }

    return TRUE;
}

StChargeInfo* CStaticData::GetChargeInfo(UINT32 dwProductID)
{
    ERROR_RETURN_NULL(dwProductID != 0);
    auto itor = m_mapChargeInfo.find(dwProductID);
    if (itor != m_mapChargeInfo.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::ReadStoreInfo(CppSQLite3Query& QueryData)
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

StStoreItemInfo* CStaticData::GetStoreItemInfo(UINT32 dwStoreID)
{
    ERROR_RETURN_NULL(dwStoreID != 0);
    auto itor = m_mapStoreInfo.find(dwStoreID);
    if (itor != m_mapStoreInfo.end())
    {
        return &itor->second;
    }

    return NULL;
}

BOOL CStaticData::ReadActivityInfo(CppSQLite3Query& QueryData)
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

StActivityInfo* CStaticData::GetActivityInfo(UINT32 dwActivityID)
{
    ERROR_RETURN_NULL(dwActivityID != 0);
    auto itor = m_mapActivityInfo.find(dwActivityID);
    if(itor != m_mapActivityInfo.end())
    {
        return &itor->second;
    }

    return NULL;
}

StItemInfo* CStaticData::GetItemInfo(UINT32 dwItemID)
{
    ERROR_RETURN_NULL(dwItemID != 0);
    auto itor = m_mapItem.find(dwItemID);
    if(itor != m_mapItem.end())
    {
        return &itor->second;
    }

    return NULL;
}

