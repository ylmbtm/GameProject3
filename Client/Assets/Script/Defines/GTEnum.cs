using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public enum ELanguage
{
    Chinese,
    English
}

public enum GameMode
{
    S,
    N
}


public enum EAction
{
    None   = 0,
    Manual = 1,
    Energy = 2,
}

public enum EPosType
{
    BagItem,
    BagGem,
    BagFashion,
    BagRune,
    BagChip,
    RoleEquip,
    RoleGem,
    RoleRune,
    RoleFashion,
    RoleMoney,
    RoleAction,
    Empty,
}

public enum EDataKeyType
{
    Id,
    Instance,
    Pos
}

public enum EMoney
{
    Gold    = 1,
    Diam    = 2,
    Crystal = 3,
    Glyph   = 4,
    Honor   = 5,
    Hoon    = 6,
    Fame    = 7,
    Mete    = 8,
    Apple   = 9,
    Soul    = 10,
    Goul    = 11,
    Poul    = 12
}

public enum EItemType
{
    EQUIP    = 1,
    DRUG     = 2,
    MAT      = 3,
    BOX      = 4,
    KEY      = 5,
    CHIP     = 6,
    GEM      = 7,
    FASHION  = 8,
    MONEY    = 9,
    ACTION   =10,
    EXP      =11,
    RUNE     =12,
    PETSOUL  =13,  
    TASK     =14,
}

public enum EBagType
{
    NONE    = 0,
    ITEM    = 1,
    GEM     = 2,
    FASHION = 3,
    CHIP    = 4,
    RUNE    = 5,
    TASK    = 6,
    SOUL    = 7,
    CARD    = 8  //英雄卡
}

public enum ECarrer
{
    O=0,  //通用
    A=1,  //圣骑士
    B=2,  //法师
    C=3,  //刺客
    D=4   //狂战士
}


public enum ERuneType
{
    LIT = 1,//小雕文
    MID = 2,//中雕文
    BIG = 3 //大雕文
}

//场景类型
public enum ESceneType
{
    TYPE_LOAD   =-1,
    TYPE_INIT   = 0,
    TYPE_LOGIN  = 1,
    TYPE_ROLE   = 2,
    TYPE_CITY   = 3,
    TYPE_PVE    = 4,
    TYPE_WORLD  = 5,
    TYPE_AREA   = 6,
}


public enum ECompare
{
    EQ=0,//等于
    GT=1,//大于
    LT=2,//小于
    GE=3,//大于等于
    LE=4,//小于等于
    NO=5,//不等于
}

public enum EAffect
{
    Self =1,//影响自己
    Enem =2,//影响敌方
    Ally =3,//影响友方
    Each =4,//影响所有
    Boss =5,//影响Boss
    Host =6,//影响主人
}

public enum EValueType
{
    FIX,  //固定值
    PER,  //百分数
    COM,  //以最大值为基准百分比,作用于当前值
}



public enum EWeatherType
{
    NONE     =0,
    SUNNY    =1,   //晴天
    THUNDER  =2,   //打雷
    RAIN     =4,   //下雨
    SNOW     =8,   //下雪
    HAIL     =16,  //冰雹
    STORM    =32,  //风暴
}



//触发器条件之间的关系
public enum ECR
{
    AND = 0,
    OR  = 1,
}


public enum EBattleCamp
{
    A,//我方
    B,//敌方
    C,//中立
    D,//其他
}


public enum EEntinyType
{
    TYPE_ACTOR = 0,//NPC
    TYPE_OBJ   = 1,//场景建筑、花草等
    TYPE_FX    = 2,//场景特效
}