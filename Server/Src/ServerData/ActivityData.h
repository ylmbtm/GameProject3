#ifndef __ACTIVITY_DATA_OBJECT_H__
#define __ACTIVITY_DATA_OBJECT_H__
#include "ServerStruct.h"

//登录奖励
struct LoginAward
{

};

struct  FirstCharge
{

};

struct  OpenFund
{

};

struct  LevelGift
{
	UINT32 GiftID[20];
	UINT32 BuyTimes[20];
	UINT32 DeadLine[20];
};


struct  VipGift
{
	UINT32 GiftID[20];
	UINT32 BuyTimes[20];
	UINT32 DeadLine[20];
};

//累充
struct  SumRecharge
{
	UINT32 m_SumMoney;  //累计充值钱数
	BOOL   m_AwardMark; //领奖标计
};

//单充
struct  SinRecharge
{

};



struct ActivityDataObject : public ShareObject
{
	ActivityDataObject()
	{

	}
	UINT64 m_uRoleID;		//角色ID
	UINT32 m_dwActivityID;  //活动ID
	UINT32 m_dwActivityType;//活动类型
	UINT32 m_dwJoinTime;    //参与时间
	UINT32 m_dwDataLen;     //数据时间长度
	union
	{
		BYTE			m_Bytes[1024];    //活动数据
		FirstCharge		m_FirstCharge;
		OpenFund		m_OpenFund;
		LevelGift       m_LevelGift;
		VipGift			m_VipGift;
		SumRecharge		m_SumRecharge;
		SinRecharge	    m_SinRecharge;
	} m_Data;


	BOOL Save(IDataBase* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		return TRUE;
	}
};

#endif //__ACTIVITY_DATA_OBJECT_H__