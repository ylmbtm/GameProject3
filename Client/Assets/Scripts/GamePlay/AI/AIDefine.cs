using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    public enum EAIFight
    {
        TYPE_FIGHTNONE,
        TYPE_FIGHT,
    }

    public enum EAIState
    {
        BORN,  //出生
        IDLE,  //闲逛
        FIGHT, //战斗
        FOLLOW,//跟随
        PATROL,//巡逻
        DEAD,  //死亡
        BACK,  //回家
        CHASE, //追击
        FLEE,  //避开
        ESCAPE,//逃跑
        PLOT,  //剧情
    }

    public enum EAITarget
    {
        TYPE_SELF   = 0,
        TYPE_TARGET = 1,
        TYPE_HOST   = 2,
    }

    public class AIDefine
    {
        public const string KEY_WARNER_DIST   = "WarnerDist";
        public const string KEY_ATTACK_DIST   = "AttackDist";
        public const string KEY_GOHOME_DIST   = "GoHomeDist";
    }
}