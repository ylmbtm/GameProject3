#ifndef __CONFIG_STRUCT_H__
#define __CONFIG_STRUCT_H__

struct  StConstantValue
{
	StConstantValue(){}

	UINT32 xxxx;
};


struct StCopyInfo
{
	StCopyInfo()
	{

	}

	UINT32 dwCopyID;
	UINT32 dwCopyType;     //副要类型
    UINT32 dwLogicType;    //副本逻辑类型
    UINT32 dwCostActID;    //消耗体力ID
    UINT32 dwCostActNum;   //消耗体力数量
    UINT32 dwAwardID;      //奖励ID
	UINT32 dwFirstAwardID; //首胜奖励ID
    UINT32 dwBattleTimes;  //每日可战斗次数

	std::string strXml;
};


struct StLocalString
{
	StLocalString()
	{

	}
	UINT32  dwID;
	std::string Language[15];
};

struct StActor
{
	StActor()
	{

	}
	UINT32  dwID;
	std::string strName;
};


struct StActionInfo
{
	StActionInfo()
	{

	}
};

struct StMoneyInfo
{
	StMoneyInfo()
	{

	}
};


struct  StItemData
{
	UINT32 dwItemID;
	UINT32 dwItemNum;
};


//掉落项
struct StDropItem 
{
	UINT32 dwItemID; //物品ID
	UINT32 dwItemNum[2];//物品数量
	UINT32 dwRatio;//概率范围
};

//奖励项
struct StAwardItem 
{
	INT32 dwAwardID;					//奖励ID
	INT32 dwRatioCount;					//概率掉落个数
	BOOL bDistinct;						//是否需要去重
	std::vector<StDropItem> FixItems;	//必掉物品
	std::vector<StDropItem>  RatioItems;//机率掉落物品
};

#endif //__CONFIG_STRUCT_H__