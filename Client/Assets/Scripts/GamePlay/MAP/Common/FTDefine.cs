using UnityEngine;
using System.Collections;

namespace MAP
{
    public enum EAreaShape
    {
        TYPE_CIRCLE   = 0,
        TYPE_RECT     = 1,
    }

    public enum EPathNodeType
    {
        Linear        = 0,
        Bezier        = 1,
    }

    public enum EMapGroup
    {
        Barrier       = 1,
        Born          = 2,
        Mine          = 3,
        Boss          = 4,
        Monster       = 5,
        Npc           = 6,
        Path          = 7,
        Portal        = 8,
        Area          = 9,
    }

    public enum EChunkType
    {
        TYPE_GROUND   = 0,
        TYPE_LIGHT    = 1,
        TYPE_BUILD    = 2,
        TYPE_CLOUD    = 3,
        TYPE_WATER    = 4,
        TYPE_TREE     = 5,
        TYPE_GRASS    = 6,
        TYPE_STONE    = 7,
        TYPE_EFFECT   = 8,
        TYPE_SOUND    = 9,
    }

    public enum EWeatherType
    {
        None          = 0,
        Sunny         = 1,   //晴天
        Thunder       = 2,   //打雷
        Rain          = 4,   //下雨
        Snow          = 8,   //下雪
        Hail          = 16,  //冰雹
        Storm         = 32,  //风暴
    }

    public enum EOpenConditionRelation
    {
        And           = 0,
        Or            = 1,
    }

    public enum EFTEventType
    {
        FTEventBegin               = 0,

        FTEventTriggerArea         = 1,  //触发一个区域
        FTEventDestroyArea         = 2,  //销毁一个区域
        FTEventTriggerBarrier      = 3,  //触发光墙
        FTEventDestroyBarrier      = 4,  //销毁光墙
        FTEventTriggerSkill        = 5,  //触发关卡技能
        FTEventTriggerSound        = 6,  //触发声音
        FTEventTriggerStep         = 7,  //触发副本阶段
        FTEventTriggerTask         = 8,  //触发任务
        FTEventTriggerTeleport     = 9,  //触发传送
        FTEventTriggerTimer        = 10, //触发计时器
        FTEventDestroyTimer        = 11, //停止定时器
        FTEventKillMe              = 12, //被秒杀
        FTEventTriggerEffect       = 13, //触发特效
        FTEventDestroyEffect       = 14, //销毁特效

        FTEventDestroyAllMonsters  = 15, 
        FTEventDestroyAllObjs      = 16,
        
        FTEventCallMonster         = 17,
        FTEventCallMine            = 18,

        FTEventTaskCameraEffect    = 19,
        FTEventTaskCG              = 20,
        FTEventTaskCutscene        = 21,
        FTEventTaskDelay           = 22,
        FTEventTaskDialogue        = 23,
    }
}