using UnityEngine;
using System.Collections;

public enum ESkillPos
{
    Skill_0,
    Skill_1,
    Skill_2,
    Skill_3,
    Skill_4,
    Skill_5,
    Skill_6,
    Skill_7,
    Skill_8,
    Skill_9,
}

public enum EActType
{
    TYPE_NONE             = 0,

    TYPE_ADDATTR          = 1,  //增加属性
    TYPE_SUBATTR          = 2,  //减少属性
    TYPE_HOT              = 3,  //HOT
    TYPE_DOT              = 4,  //DOT
    TYPE_SUPER            = 5,  //霸体
    TYPE_VARIATION        = 6,  //变形
    TYPE_STUN             = 7,  //昏迷
    TYPE_FIXBODY          = 8,  //定身
    TYPE_STEALTH          = 9,  //隐身
    TYPE_FROZEN           = 10, //冻住

    TYPE_BLIND            = 11, //致盲
    TYPE_SILENT           = 12, //沉默
    TYPE_SLEEP            = 13, //睡眠
    TYPE_ABSORB           = 14, //吸收伤害
    TYPE_SUCKBLOOD        = 15, //吸血
    TYPE_DIVIVE           = 16, //无敌
    TYPE_PARALY           = 17, //麻痹
    TYPE_FEAR             = 18, //恐惧
    TYPE_REFLEX           = 19, //反弹伤害
    TYPE_TIMEDDEATH       = 20, //倒计时死亡

    TYPE_FLASH            = 21, //闪现
    TYPE_SPURT            = 22, //冲锋
    TYPE_BEATFLY          = 23, //击飞
    TYPE_BEATDOWN         = 24, //击倒
    TYPE_BEATBACK         = 25, //击退
    TYPE_HIT              = 26, //受击
    TYPE_ANIM             = 27, //动画
    TYPE_AUDIO            = 28, //声音
    TYPE_AUDIOLOOP        = 29, //声音Loop

    TYPE_FLOAT            = 31, //浮空
    TYPE_PUSH             = 32, //推人
    TYPE_FACETOTARGET     = 33, //朝向
    TYPE_TAUNT            = 34, //嘲讽
    TYPE_GATHER           = 35, //聚怪
    TYPE_FALSEDEAD        = 36, //假死

    TYPE_BEAM             = 40, //光束
    TYPE_TRIGGER          = 41, //触发器
    TYPE_EFFECT           = 42, //特效
    TYPE_FLYWEAPON        = 43, //飞行武器
    TYPE_LIGHTING         = 44, //闪电
    TYPE_MISSILE          = 45, //基础飞弹
    TYPE_RAY              = 46, //射线
    TYPE_WALL             = 47, //阻挡墙
    TYPE_BINDEFFECT       = 48, //绑定特效
    TYPE_SUMMON           = 49, //召唤

    TYPE_MOVE             = 56, //位移
    TYPE_THROUGHMOVESTART = 57, //穿插位移开始
    TYPE_THROUGHMOVE      = 58, //穿插位移
    TYPE_THROUGHMOVEEND   = 59, //穿插位移结束

    TYPE_DAMAGE           = 62, //伤害
    TYPE_HEAL             = 63, //治疗


    TYPE_SCOPESPHERE      = 71, //球形范围检测目标
    TYPE_SCOPECYLINDER    = 72, //弧形范围检测目标
    TYPE_SCOPEBOX         = 73, //盒子范围检测目标
    TYPE_SCOPETRIANGLE    = 74, //三角范围检测目标
    TYPE_ADDBUFF          = 75, //添加BUFF
    TYPE_DISPEL           = 76, //驱散
    TYPE_STEALBUFF        = 77, //偷取BUFF
    TYPE_WRITEWHEELEYE    = 78, //写轮眼
    TYPE_AURA             = 79, //光环

    TYPE_SHADER           = 81, //Shader效果
    TYPE_SCALE            = 82, //变换大小
    TYPE_MATERIAL         = 83, //材质效果

    TYPE_COLORSKY         = 85, //天空变色
    TYPE_SHAKE            = 86, //震屏效果
    TYPE_ROTATE           = 87, //旋转
    TYPE_TIMESCALE        = 88, //时间控制
    TYPE_HIDEWEAPON       = 89, //隐藏武器

    TYPE_RESETSKILLCD     = 91, //重置技能CD
    TYPE_SHORTENSKILLCD   = 92, //减少技能CD
    TYPE_EXTENDSKILLID    = 93, //延长技能CD
    TYPE_BREAKSKILL       = 94, //打断技能
    TYPE_UPSKILLRESULT    = 95, //提高技能效果
    TYPE_CHANGESKILL      = 96, //使当前技能升级为新的技能

    TYPE_PROBABLY         = 101, //执行子节点们的可能性
    TYPE_SEQUENCE         = 102, //对子节点执行队列操作
    TYPE_CLONE            = 103, //克隆节点
    TYPE_RANDOM           = 104, //对子节点执行随机执行一个的操作
    TYPE_RANDOMMANY       = 105, //对子节点执行随机执行多个的操作
    TYPE_TREE             = 106,
    TYPE_LOG              = 107,
}

public enum ESkillCostType
{
    NO,
    MP = 1,//魔法
    HP = 2,//生命
    SP = 3,//特殊资源
    XP = 4,//经验
}

public enum ESkillType
{
    Postive = 0,//主动
    Passive = 1,//被动
}

public enum ESkillMoveType
{
    TYPE_CANNOT_MOVE = 0,
    TYPE_NOANIM_MOVE = 1,
    TYPE_IKANIM_MOVE = 2,
}

public enum EDamageType
{
    TYPE_PHYSICS = 0,//物理
    TYPE_FIRE    = 1,//火焰
    TYPE_ICE     = 2,//冰霜
    TYPE_DARK    = 3,//暗影
    TYPE_LIGHT   = 4 //闪电
}


public enum ESelectTarget
{
    TYPE_CASTER     = 0,
    TYPE_TARGET     = 1,
    TYPE_TARGETLIST = 2,
}

public enum ESelectTargetPolicy
{
    Normal = 0,
    MaxHP  = 1,
    MinHP  = 2,
    MaxDis = 3,
    MinDis = 4,
}

public enum EActStatus
{
    INITIAL,//初始化
    STARTUP,//开始
    TRIGGER,//触发
    RUNNING,//运行
    SELFEND,//本体运行结束
    SUCCESS,//成功
}


public enum ESkillBreakReason
{
    TYPE_NONE,      //无
    TYPE_BUFF,      //Buff打断
    TYPE_STIFF,     //硬直打断
    TYPE_NEWSKILL,  //新技能打断
    TYPE_BEDAMAGE,  //受到伤害打断
    TYPE_SPECIAL,   //特殊打断
}


public enum EActEventType
{
    Special,//特殊
    Instant,//瞬间
    Subtain,//持续
}

public enum EnumCameraShake
{
    Horizontal,             //水平
    Vertical,               //垂直
    HorizontalAndVertical   //水平和垂直都摇晃
}

//Buff叠加类型
public enum EBuffOverlayType
{
    UnChange,
    Overlay,
    Refresh,
    OverlayAndRefresh,
}

//Buff销毁类型
public enum EBuffDestroyType
{
    BY_NONE       = 0,
    BY_BATTLEEND  = 1,
    BY_TIMEEND    = 2,
    BY_OFFLINE    = 3,
    BY_DEAD       = 4,
    BY_NEXTATTACK = 5,
    BY_ALL        = 6,
}

public enum EBuffType
{
    None,
    Buff,
    Debuff,
}


//特效绑定位置
public enum EBind
{
    None  =-1,
    Head  = 0, //出现在头部位置
    Body  = 1, //出现在身体位置
    Foot  = 2, //出现在脚部位置
    LHand = 3, //出现在左手上
    RHand = 4, //出现在右手上
    Buff  = 5, //Buff点
}

public enum EBindTarget
{
    TYPE_CASTER = 0,
    TYPE_TARGET = 1,
}

//移动朝向
public enum EMoveToward
{
    MoveToTarget,
    MoveToForward,
}

public enum EEffectType
{
    UV,
    PA,
    EP,
    TR,
}

public enum EEffectPeform
{
    TYPE_SWORD      = 1,
    TYPE_MISSILE    = 2,
    TYPE_RELICS     = 3,
    TYPE_AOE        = 4,
    TYPE_BORNORDEAD = 5,
    TYPE_LIGHT      = 6,
    TYPE_HIT        = 7,
    TYPE_BUFF       = 8,
}

public enum EHealType
{
    HP = 0,
    MP = 1,
    SP = 2
}