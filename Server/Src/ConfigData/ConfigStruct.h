#ifndef __CONFIG_STRUCT_H__
#define __CONFIG_STRUCT_H__

#include "../ServerData/ServerDefine.h"
struct  StConstantValue
{
	StConstantValue() {}

	UINT32 xxxx;
};


struct StCopyInfo
{
	StCopyInfo()
	{

	}

	UINT32 dwCopyID;
	UINT32 dwCopyType;     //副要类型
	UINT32 dwCostActID;    //消耗体力ID
	UINT32 dwCostActNum;   //消耗体力数量
	UINT32 dwGetMoneyID;   //获取货币的ID
	UINT32 dwGetMoneyRatio;//获取货币的系数
	UINT32 dwGetExpRation; //获取经验的系数
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
	std::string Language[MAX_LANGUAGE_NUM];
};

struct StActor
{
	StActor()
	{

	}
	UINT32  dwID;
	INT32   Propertys[PROPERTY_NUM];
	std::string strName;

};

struct StCarrerInfo
{
	StCarrerInfo()
	{
		dwID = 0;
		dwActorID = 0;
		dwBornCity = 0;
		strName = "";
	}
	UINT32  dwID;
	std::string strName;
	UINT32  dwActorID;
	UINT32  dwBornCity;
};

struct StLevelInfo
{
	StLevelInfo()
	{
		dwLevel = 0;
		dwFightValue = 0;
		dwNeedExp = 0;
		dwCarrerID = 0;
	}

	UINT32 dwCarrerID;
	UINT32 dwLevel;
	UINT32 dwNeedExp;
	INT32 Propertys[PROPERTY_NUM];
	UINT32 dwFightValue;
};

struct StActionInfo
{
	StActionInfo()
	{
		dwActionID = 0;		// 行动力ID
		UnitTime = 0;		//恢复1个单位需要的时间(秒)
		dwMax = 0;			//最大值
	}

	UINT32 dwActionID;	// 行动力ID
	INT32 UnitTime;		//恢复1个单位需要的时间(秒)
	UINT32 dwMax;		//最大值
};

struct StMoneyInfo
{
	StMoneyInfo()
	{
		dwMoneyID = 0;	//货币ID
		dwMax = 0;		//上限
	}

	UINT32 dwMoneyID;	//货币ID
	UINT32 dwMax;		//上限
};


struct  StItemData
{
	UINT32 dwItemID;
	UINT32 dwItemNum;
};


//掉落项
struct StDropItem
{
	UINT32 dwItemID;		//物品ID
	UINT32 dwItemNum[2];	//物品数量
	UINT32 dwRatio;			//概率范围
};

//奖励项
struct StAwardItem
{
	INT32 dwAwardID;					//奖励ID
	INT32 dwRatioCount;					//概率掉落个数
	BOOL bDistinct;						//是否需要去重
	INT32 nCarrer;						//职业
	std::vector<StDropItem> FixItems;	//必掉物品
	std::vector<StDropItem>  RatioItems;//机率掉落物品
};


struct StItemInfo
{
	UINT32 dwItemID;	//物品ID
	UINT32 dwType;		//物品类型
	UINT32 dwBagType;	//物品背包类型
	UINT32 Quality;		//物品的品质
	UINT32 SellID;		//出售货币ID
	INT32 SellPrice;	//出售价格
	UINT32 UseType;		//使用类型
	UINT32 CarrerID;	//职业
	INT32 StackMax;		//最大堆放数
	INT32 Data1;		//参数1
	INT32 Data2;		//参数2
};


struct StFuncInfo
{
	UINT32 dwFuncID;	//! 功能ID
	INT32 OpenLevel;	//! 开放等级
	INT32 VipLevel;		//! VIP提前开放等级
	INT32 Logic;		//! 逻辑关系 1->优先VIP 2->同时满足
};


struct StFuncVipInfo
{
	UINT32 dwFuncID;				//! 功能ID
	INT32  VipValue[MAX_VIP_LEVEL];	//! 对应VIP数值
};


struct StEquipInfo
{
	UINT32 dwEquipID;		//! ID
	UINT32 dwSuitID;
	UINT32 dwPos;
};

struct StPetInfo
{
	UINT32 dwPetID;		//! ID
};

struct StPartnerInfo
{
	UINT32 dwPartnerID;		//! ID
};


struct StTaskInfo
{
	UINT32 TaskID;		//! 任务ID
	UINT32 TaskType;	//! 任务类型
	UINT32 Condition;	//! 任务条件
	UINT32 TaskEvent;	//! 任务事件
	INT32  NeedLevel;	//! 开放等级
	UINT32 AwardID;		//! 任务奖励ID
};


struct StSkillInfo
{
	UINT32 SkillID;		//! 任务ID
	UINT32 Level;		//! 任务类型
	UINT32 CD;			//! 任务条件
	UINT32 CostMp;		//! 消耗魔法值
	UINT32 Fix;			//! 增加固定攻击
	UINT32 Percent;		//! 增加百分比伤害
	UINT32 SelfBuffID;  //! 给自己加的BuffID;
	UINT32 TargetBuffID;//! 给目标上的BuffID;
};


struct StBuffInfo
{
	UINT32 BuffID;						//BuffID
	UINT32 BuffEffect;					//Buff目标需要加的特效
	INT32  PtyValue[PROPERTY_NUM];		//加属性值
	INT32  PtyPercent[PROPERTY_NUM];	//加属性百分比
	INT32  TotalTime;                   //总时长(ms)
	INT32  Interval;					//间隔时长
	BOOL   OverLay;				        //是否可以叠加
	BOOL   AddLua;
	BOOL   TickLua;
	BOOL   RemoveLua;


};


struct StStoreItemInfo
{
	UINT32 StoreID;		//! 商品ID
	UINT32 ItemID;
	UINT32 ItemNum;
};

struct StActivityInfo
{
	UINT32 ActivityID;		//! 活动ID
	std::string Name;
	std::string Desc;
	std::string Ad;
	UINT32 Statue;
	UINT32 ActivityType;
	UINT32 AwardIndex;
	UINT32 StartTime;
	UINT32 EndTime;
};



#endif //__CONFIG_STRUCT_H__