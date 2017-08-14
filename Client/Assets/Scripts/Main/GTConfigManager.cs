using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System;
using ACT;

public class GTConfigManager : GTSingleton<GTConfigManager>
{
    public override void Init()
    {
        ReadCfgLocalString.           Read("Text/Local/Data_LocalString");
        ReadCfgCurve.                 Read("Text/Local/Data_Curve");

        ReadCfgStageMount.            Read("Text/Local/Data_StageMount");
        ReadCfgStagePartner.          Read("Text/Local/Data_StagePartner");
        ReadCfgStagePet.              Read("Text/Local/Data_StagePet");
        ReadCfgStageRelics.           Read("Text/Local/Data_StageRelics");
        ReadCfgStageRole.             Read("Text/Local/Data_StageRole");

        ReadCfgSearch.                Read("Text/Local/Data_Search");
        ReadCfgRandomName.            Read("Text/Local/Data_RandomNames");

        ReadCfgProperty.              Read("Text/Data/Data_Property");
        ReadCfgQuality.               Read("Text/Data/Data_Quality");

        ReadCfgItem.                  Read("Text/Data/Data_Item");
        ReadCfgEquip.                 Read("Text/Data/Data_Equip");
        ReadCfgAction.                Read("Text/Data/Data_Action");

        ReadCfgRole.                  Read("Text/Data/Data_Role");
        ReadCfgRoleLevel.             Read("Text/Data/Data_RoleLevel");

        ReadCfgEquip.                 Read("Text/Data/Data_Equip");
        ReadCfgEquipAdvance.          Read("Text/Data/Data_EquipAdvance");
        ReadCfgEquipAdvanceCost.      Read("Text/Data/Data_EquipAdvanceCost");
        ReadCfgEquipStreng.           Read("Text/Data/Data_EquipStrengthen");
        ReadCfgEquipStrengLevel.      Read("Text/Data/Data_EquipStrengthenLevel");
        ReadCfgEquipStar.             Read("Text/Data/Data_EquipStar");
        ReadCfgEquipSuit.             Read("Text/Data/Data_EquipSuit");

        ReadCfgGem.                   Read("Text/Data/Data_Gem");
        ReadCfgGemLevel.              Read("Text/Data/Data_GemLevel");
        ReadCfgGemSuit.               Read("Text/Data/Data_GemSuit");

        ReadCfgAward.                 Read("Text/Data/Data_Award");

        ReadCfgCopyMainChapter.       Read("Text/Data/Data_CopyMainChapter");
        ReadCfgCopy.                  Read("Text/Data/Data_Copy");

        ReadCfgEffect.                Read("Text/Data/Data_Effect");
        ReadCfgRelics.                Read("Text/Data/Data_Relics");
        ReadCfgMachine.               Read("Text/Data/Data_Machine");

        ReadCfgActorGroup.            Read("Text/Data/Data_ActorGroup");
        ReadCfgActor.                 Read("Text/Data/Data_Actor");
        ReadCfgActorRace.             Read("Text/Data/Data_ActorRace");
        ReadCfgActorModel.            Read("Text/Data/Data_ActorModel");

        ReadCfgStore.                 Read("Text/Data/Data_Store");
        ReadCfgStoreType.             Read("Text/Data/Data_StoreType");

        ReadCfgPet.                   Read("Text/Data/Data_Pet");
        ReadCfgPetLevel.              Read("Text/Data/Data_PetLevel");

        ReadCfgPartner.               Read("Text/Data/Data_Partner");
        ReadCfgPartnerLevel.          Read("Text/Data/Data_PartnerLevel");
        ReadCfgPartnerWake.           Read("Text/Data/Data_PartnerWake");
        ReadCfgPartnerAdvance.        Read("Text/Data/Data_PartnerAdvance");
        ReadCfgPartnerWash.           Read("Text/Data/Data_PartnerWash");
        ReadCfgPartnerFetter.         Read("Text/Data/Data_PartnerFetter");
        ReadCfgPartnerStar.           Read("Text/Data/Data_PartnerStar");

        ReadCfgTask.                  Read("Text/Data/Data_Task");
        ReadCfgAdventure.             Read("Text/Data/Data_Adventure");
        ReadCfgSkillTalent.           Read("Text/Data/Data_SkillTalent");
        ReadCfgMine.                  Read("Text/Data/Data_Mine");
        ReadCfgSkill.                 Read("Text/Data/Data_Skill");
        ReadCfgVideo.                 Read("Text/Data/Data_Video");
        ReadCfgDialogue.              Read("Text/Data/Data_Dialogue");

        ActBuffSystem.                Read("Text/Buff/Buff");
    }
}