using UnityEngine;
using System.Collections;


public enum EWindowType
{
    BOTTOM,
    FLYWORD,
    FIXED,
    WINDOW,
    DIALOG,
    MSGTIP,
    LOADED,
    MASKED,
}

public enum EDialogType
{
    SureToBuy,
}

public enum EWindowID
{
    UI_START,
    UI_LOGIN,
    UI_REGISTER,
    UI_SERVER,
    UI_NOTICE,
    UI_CREATEROLE,
    UI_HOME,

    UI_HEROINFO,
    UI_BAG,
    UI_ITEMINFO,
    UI_ITEMUSE,
    UI_EQUIPINFO,
    UI_GEMINFO,
    UI_CHIPINFO,
    UI_FASHIONINFO,
    UI_RUNEINFO,

    UI_ROLEEQUIP,
    UI_ROLEGEM,
    UI_ROLEFASHION,
    UI_ROLERUNE,
    UI_ROLEFETTER,

    UI_EQUIP,
    UI_GEM,

    UI_MAINRAID,
    UI_MAINGATE,
    UI_MAINRESULT,

    UI_MESSAGETIP,
    UI_MESSAGEBOX,
    UI_AWARDTIP,
    UI_AWARDBOX,

    UI_LOADING,

    UI_PET,

    UI_PARTNER,
    UI_PARTNERADVANCE,
    UI_PARTNERSTRENGH,
    UI_PARTNERSTAR,
    UI_PARTNERWAKE,
    UI_PARTNERWASH,
    UI_PARTNERSKILL,
    UI_PARTNERFETTER,
    UI_PARTNERFPROPERTY,
    UI_PARTNERBATTLE,

    UI_MOUNT,
    UI_MOUNTLIBRARY,
    UI_MOUNTBLOOD,
    UI_MOUNTTAME,
    UI_MOUNTTURNED,


    UI_RELICS,
    UI_RELICSSKILL,

    UI_MASK,

    UI_STORE,
    UI_TASK,
    UI_WORLDMAP,

    UI_GUIDE,
    UI_TASKTALK,
    UI_TASKINTERACTIVE,
    UI_PLOTCUTSCENE,

    UI_ADVENTURE,
    UI_SKILL,
    UI_REBORN,
    UI_FLUTTER,
    UI_BOARDCONTAINER,

    UI_SETTING,
    UI_MAINBOSSHP,
}

public enum EWindowMaskType
{
    None,
    BlackTransparent,
    WhiteTransparent,
    Blur,
    Black,
    Net,
}

public enum EWindowHideType
{
    Normal,
    Scale,
}

public enum EWindowOpenType
{
    Normal,
    Scale,
}