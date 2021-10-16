#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_
#include "../Message/Msg_Move.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Game_Define.pb.h"
#include "../Message/Msg_Struct.h"

#include "Position.h"
#include "SkillObject.h"
#include "MapObject.h"
#include "CommonMath.h"
#include "../GameStruct.h"
#include "StaticStruct.h"

class CScene;
class ResultPlayer;
class CBuffObject;
class CSceneObject : public CMapObject
{
public:
	CSceneObject(UINT64 uGuid, CScene* pScene);

	~CSceneObject();
public:
	BOOL            SendMsgProtoBuf(INT32 nMsgID, const google::protobuf::Message& pdata);
	BOOL            SendMsgRawData(INT32 nMsgID, const char* pdata, UINT32 dwLen);
	BOOL            SetConnectID(UINT32 dwProxyID, UINT32 dwClientID);
	BOOL            OnUpdate(UINT64 uTick);
	BOOL            SaveNewData(ObjectNewNty& Nty);
	BOOL            SaveUpdateData(ObjectActionNty& Nty);
	BOOL            Reset();

	//以下为对象的操作方法
public:
	UINT32          GetHp();
	UINT32          GetMp();
	UINT64          GetObjectGUID();
	UINT32          GetActorID();
	UINT32          GetObjType();
	UINT32          GetCamp();
	UINT64          GetControllerID();
	VOID            SetControllerID(UINT64 uID);

	VOID            ChangeHp(INT32 nValue);
	VOID            ChangeMp(INT32 nValue);

	BOOL            IsChanged();

	BOOL            IsConnected();
	BOOL            IsEnterCopy();
	VOID            SetEnterCopy();

	VOID            SetActionID(UINT32 dwActionID);
	UINT32          GetActionID();

	BOOL            IsDead();
	BOOL			SetDead(BOOL bDead);

	BOOL			SetPos(FLOAT x, FLOAT y, FLOAT z, FLOAT ft = 0);
	Vector3D&       GetPos();

	FLOAT           GetFaceTo();

	BOOL			SaveBattleRecord(ResultPlayer* pResult);

	BOOL			ChangeEquip(INT32 nPos, UINT32 dwEquipID);
	BOOL            ChangeMount(UINT32 dwMountID);
	BOOL            SetRiding(BOOL bRiding);
	FLOAT           GetCurSpeed();
	BOOL            IsRobot();
public:
	//////////////////////////////////////////////////////////////////////////
	//buff的处理
	BOOL			AddBuff(UINT32 dwBuffID);
	BOOL			RemoveBuff(UINT32 dwBuffID);
	BOOL			UpdateBuff(UINT64 uTick);
	BOOL			ClearBuff();
	std::map<UINT32, CBuffObject*> m_mapBuff;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//技能
	std::vector<St_SkillData> m_vtNormals;
	std::vector<St_SkillData> m_vtSpecials;
	CSkillObject	m_SkillObject;
	UINT32			ProcessSkill(const SkillCastReq& Req);
	UINT32			ProcessAction(const ActionReqItem& Item);
	UINT64			GetLastSkillTick(UINT32 dwSkillID);
	BOOL			SetLastSkillTick(UINT32 dwSkillID, UINT64 uTick);
	St_SkillData*	GetSkillData(UINT32 dwSkillID);
	INT32			GetSkillLevel(UINT32 dwSkillID);
	BOOL			InitSkills(const google::protobuf::RepeatedPtrField<::SkillItem>& vtSkills);
	BOOL			InitSkills(); //怪物和招唤物使用
	UINT32          GetNextComboSkill(UINT32 dwSkillID);
	BOOL            CheckSkillCD(UINT32 dwSkillID, UINT64 uCD);
	INT32           GetShip(CSceneObject* pTarget);

	BOOL			IsInCircle(Vector3D hitPoint, float radius, float height);
	BOOL			IsInSquare(Vector3D hitPoint, float hitDegree, float length, float width);
	BOOL			IsInSector(Vector3D hitPoint, float hitDegree, float radius, float hAngle);
	VOID            SetBattleResult(ECopyResult nBattleResult);
	ECopyResult     GetBattleResult();

	//////////////////////////////////////////////////////////////////////////
	//更新角色坐标
	BOOL            UpdatePosition(UINT64 uTick);
	//复活
	BOOL            Revive();
	//////////////////////////////////////////////////////////////////////////

public:
	//对象的基本信息
	//////////////////////////////////////////////////////////
	std::string     m_strName;						//对象的名字
	UINT64          m_uGuid;						//实例ID, 对玩家是角色ID
	UINT32          m_dwActorID;					//对象ID
	UINT32          m_dwObjType;					//对象类型 玩家，宠物， NPC之类的
	INT32           m_dwCamp;						//阵营
	UINT32          m_dwActionID;					//当前动作状态
	Vector3D        m_Pos;							//位置
	FLOAT           m_ft;							//对象坐标, 朝向
	UINT32          m_dwStatus;				        //对象当前的状态
	INT32           m_dwLevel;						//等级
	INT32           m_Propertys[PROPERTY_NUM];		//15个属性的数值
	UINT32          m_Equips[EQUIP_MAX_NUM];		//角色装备
	UINT32          m_dwMountID;                    //坐骑ID
	BOOL            m_bRiding;                      //是否正在骑乘
	BOOL            m_bRobot;                       //机器人
	St_ChangeStatus m_ChangeFlag;					//数据改变标记

	UINT64          m_uHostGuid;					//主人的GUID
	UINT64          m_uControlerID;					//AI控制人的GUID
	UINT64          m_uSummonerID;					//招唤者的GUID
	UINT64          m_uPetGuid;                     //宠物guid
	UINT64          m_uPartnerGuid;                 //伙伴guid
	UINT64          m_uLastMoveTick;

	BOOL            m_bIsCampCheck;					//是否影响阵营结算
	BOOL            m_bIsMonsCheck;					//是否影响刷怪(玩家阵营的都不影响, 宠物，招唤物, 配制的特定物)
	StActorInfo*    m_pActorInfo;                   //对象的基本信息
	ECopyResult     m_nBattleResult;                //副本战斗结果

	HitEffectNtf    m_EffectNtf;
	//////////////////////////////////////////////////////////
	//对象的一些标记
	BOOL            m_bEnter;   //玩家是否己经进入副本

	//对象的副本结算数据

	//对象的连接情况，仅玩家有效
	////////////////////////////////////////////////////////////////
	UINT32          m_dwProxyConnID;
	UINT32          m_dwClientConnID;
};


#endif //_SCENE_OBJECT_H_
