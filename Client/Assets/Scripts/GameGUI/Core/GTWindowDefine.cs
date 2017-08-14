using UnityEngine;
using System.Collections;


public enum EWindowType
{
    Bottom,
    Window,
    Message,
    NewBie,
    Loaded,
}

public enum EWindowShowMode
{
    DoNothing,              //打开时啥事也不干
    HideOther,              //打开时会关闭其它窗口
    SaveTarget,             //打开时会保存当前的全屏大窗口，以便全屏窗口管理
}

public enum EWindowID
{
    UINone,

    UILogin,
    UIAccount,
    UIServer,
    UINotice,

    UINetWaiting,
    UILoading,
    UIMask,

    UISetting,

    UIMessageTip,
    UIMessageBox,
    UIMessageBoxForNetwork,
    UIAwardTip,
    UIAwardBox,
    UIDialogue,

    UICreateRole,
    UIHome,
    UIBag,
    UIEquip,
    UIGem,
    UIPet,
    UIStore,
    UIWorldMap,
    UIAdventure,
    UISkill,
    UIReborn,

    UITask,
    UITaskTalk,
    UITaskInterActive,

    UIHeroInfo,
    UIItemInfo,
    UIItemUse,
    UIEquipInfo,
    UIGemInfo,
    UIChipInfo,
    UIFashionInfo,
    UIRuneInfo,

    UIRoleEquip,
    UIRoleGem,
    UIRoleFashion,
    UIRoleRune,
    UIRoleFetter,

    UIMainRaid,
    UIMainCopy,
    UIMainResult,
    UIMainBossHP,



    UIPartner,
    UIPartnerAdvance,
    UIPartnerStrength,
    UIPartnerStar,
    UIPartnerWake,
    UIPartnerWash,
    UIPartnerSkill,
    UIPartnerFetter,
    UIPartnerProperty,
    UIPartnerBattle,

    UIMount,
    UIMountLibrary,
    UIMountBlood,
    UIMountTame,
    UIMountTurned,

    UIRelics,
    UIRelicsSkill,

    UIGuide,

    UIPlotCutscene,
}

public enum EWindowMaskType
{
    None,
    BlackTransparent,
    WhiteTransparent,
    Blur,
    Black,
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