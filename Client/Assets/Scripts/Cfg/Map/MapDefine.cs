using UnityEngine;
using System.Collections;

namespace CFG
{
    public enum EMonsterWaveSpawn
    {
        TYPE_ALONG,//按顺序刷
        TYPE_WHOLE,//一次性全部刷
        TYPE_RADOM,//随机刷一个
    }

    public enum EObjectSpawn
    {
        TYPE_MINE,
        TYPE_TASK,
        TYPE_MONSTER,
    }

    public enum EMapTrigger
    {
        TYPE_NONE,                //什么也不做
        TYPE_WAVESET       = 1,   //触发怪物波次
        TYPE_TASK          = 2,   //触发任务
        TYPE_PLOT          = 3,   //触发剧情
        TYPE_MACHINE       = 4,   //触发机关
        TYPE_BARRIER       = 5,   //触发光墙  
        TYPE_REGION        = 6,   //触发新的触发器
        TYPE_RESULT        = 7,   //触发副本结算
        TYPE_CUTSCENE      = 8,   //触发过场动画
        TYPE_PORTAL        = 9,   //触发一个传送门
        TYPE_BUFF          = 10,  //触发Buff、光环
        TYPE_MONSTEGROUP   = 11,  //触发怪物堆
        TYPE_MINEGROUP     = 12 , //触发矿石堆
    }

    public enum EPathType
    {
        Linear,//线性
        Bezier,//贝塞尔
    }

    public enum ETriggerCondition
    {
        TYPE_AWAKE_REGION  = 0,//触发器初始化时触发
        TYPE_ENTER_REGION  = 1,//进入区域
        TYPE_LEAVE_REGION  = 2,//离开区域
        TYPE_WAVES_FINISH  = 3,//波次结束后触发
    }
}

