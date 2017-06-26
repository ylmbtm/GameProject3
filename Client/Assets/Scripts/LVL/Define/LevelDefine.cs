using UnityEngine;
using System.Collections;

public enum EMapHolder
{
    Born,
    MonsterSet,
    WaveSet,
    Barrier,
    Region,
    Portal,
    Npc,
    MineSet,
    Obj,
}

public enum EMapObjType
{
    TYPE_BUILD,
    TYPE_PLANT,
    TYPE_STONE
}

public enum ELevelType
{
    Level,
    World,
}

public enum EDropObjectAbsorbMode
{
    LineChase          = 1,    //直线追踪
    LineChaseAndCircle = 2,    //先上升，然后旋转追踪目标吸附
}

public enum EDropObjectState
{
    None,
    Created,       //已创建
    Splash,        //四处溅射，宝物向四周随机角度抛物线弹出
    Raise,         //升高
    Wait,          //等待
    CircleFly,     //曲线飞向主角
    LineFly,       //直线飞向主角
    Dead,          //消亡
}