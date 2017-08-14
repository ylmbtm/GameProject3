using UnityEngine;
using System.Collections;
using Protocol;
using ACT;
using System;

public class CharacterHelper
{
    public static bool CalcDamage(Character attacker, Character defender, ActSkill skill, EDamageType type, float percent, int fixValue, bool ignoreDefense)
    {
        XDamage dmg = new XDamage();
        dmg.AttackerName = attacker.Name;
        dmg.DefenderName = defender.Name;
        dmg.Skill = skill == null ? string.Empty : skill.Name;
        dmg.Type = (int)type;
        switch (type)
        {
            case EDamageType.TYPE_PHYSICS:
                dmg.Value = (int)(percent * attacker.CurrAttr.AP + fixValue);
                break;
            case EDamageType.TYPE_DARK:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.BAP) + fixValue);
                break;
            case EDamageType.TYPE_ICE:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.IAP) + fixValue);
                break;
            case EDamageType.TYPE_LIGHT:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.LAP) + fixValue);
                break;
            case EDamageType.TYPE_FIRE:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.FAP) + fixValue);
                break;
        }
        dmg.Value = (int)(UnityEngine.Random.Range(0.80f, 0.95f) * dmg.Value);
        CalcDamage(attacker, defender, dmg, ignoreDefense);
        return true;
    }

    public static bool CalcDamage(Character attacker, Character defender, XDamage dmg, bool ignoreDefense)
    {
        if (defender.IsDivive)
        {
            return false;
        }
        int hurtValue = dmg.Value;
        Vector3 defenderPos = defender.Pos;
        defender.BeDamage(hurtValue);
        if (attacker.IsMain && defender != attacker)
        {
            GTTimerManager.Instance.DelListener(defender.Avatar.SetHitShaderOff);
            defender.Avatar.SetHitShaderOn();
            GTTimerManager.Instance.AddListener(0.2f, defender.Avatar.SetHitShaderOff);
        }

        if (hurtValue > 0)
        {
            bool crit = UnityEngine.Random.Range(0, 3) == 1;

            if (defender.IsMain)
            {
                if (crit)
                {
                    GTWorld.Instance.Fly.Show(hurtValue.ToString(), defenderPos, EFlyWordType.TYPE_ENEMY_CRIT);
                }
                else
                {
                    GTWorld.Instance.Fly.Show(hurtValue.ToString(), defenderPos, EFlyWordType.TYPE_ENEMY_HURT);
                }
            }
            else
            {
                if (attacker.IsMain)
                {
                    if (crit)
                    {
                        GTWorld.Instance.Fly.Show(hurtValue.ToString(), defenderPos, EFlyWordType.TYPE_AVATAR_CRIT);
                    }
                    else
                    {
                        GTWorld.Instance.Fly.Show(hurtValue.ToString(), defenderPos, EFlyWordType.TYPE_AVATAR_HURT);
                    }
                }
                if (attacker.Type == EActorType.PARTNER && attacker.Host == GTWorld.Main)
                {
                    if (crit)
                    {
                        GTWorld.Instance.Fly.Show(hurtValue.ToString(), defenderPos, EFlyWordType.TYPE_PARTNER_CRIT);
                    }
                    else
                    {
                        GTWorld.Instance.Fly.Show(hurtValue.ToString(), defenderPos, EFlyWordType.TYPE_PARTNER_HURT);
                    }
                }
            }
        }
        UpdateHP(defender);
        return true;
    }

    public static bool CalcHeal(EHealType healType, Character attacker, Character defender, string name, float percent, int fixValue)
    {
        switch (healType)
        {
            case EHealType.HP:
                return CalcBackHP(attacker, defender, name, percent, fixValue);
            case EHealType.MP:
                return CalcBackMP(attacker, defender, name, percent, fixValue);
            default:
                return false;
        }
    }

    public static bool CalcDot(Character attacker, Character defender, ActBuff buff, EDamageType type, float percent, int fixValue, bool ignoreDefense)
    {
        XDamage dmg = new XDamage();
        dmg.AttackerName = attacker.Name;
        dmg.DefenderName = defender.Name;
        dmg.Skill = buff.Name;
        dmg.Type = (int)type;
        switch (type)
        {
            case EDamageType.TYPE_PHYSICS:
                dmg.Value = (int)(percent * attacker.CurrAttr.AP + fixValue);
                break;
            case EDamageType.TYPE_DARK:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.BAP) + fixValue);
                break;
            case EDamageType.TYPE_ICE:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.IAP) + fixValue);
                break;
            case EDamageType.TYPE_LIGHT:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.LAP) + fixValue);
                break;
            case EDamageType.TYPE_FIRE:
                dmg.Value = (int)(percent * (attacker.CurrAttr.AP + attacker.CurrAttr.FAP) + fixValue);
                break;
        }
        CalcDamage(attacker, defender, dmg, ignoreDefense);
        return true;
    }

    public static bool CalcHot(EHealType healType, Character attacker, Character defender, string name, float percent, int fixValue)
    {
        switch (healType)
        {
            case EHealType.HP:
                return CalcBackHP(attacker, defender, name, percent, fixValue);
            case EHealType.MP:
                return CalcBackMP(attacker, defender, name, percent, fixValue);
            default:
                return false;
        }
    }

    public static bool CalcBackMP(Character attacker, Character defender, string name, float percent, int fixValue)
    {
        int calcValue = (int)(percent * attacker.CurrAttr.AP + fixValue);
        int healValue = defender.AddMP(calcValue);
        if (healValue > 0 && attacker.IsMain)
        {
            GTWorld.Instance.Fly.Show(healValue.ToString(), defender.Pos, EFlyWordType.TYPE_AVATAR_BACKMAGIC);
        }
        if (defender.IsMain)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_AVATAR_MP);
        }
        if (defender.Type == EActorType.PARTNER && defender.Host == GTWorld.Main)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_PARTNER_HP);
        }
        return true;
    }

    public static bool CalcBackHP(Character attacker, Character defender, string name, float percent, int fixValue)
    {
        int calcValue = (int)(percent * attacker.CurrAttr.AP + fixValue);
        int healValue = defender.AddHP(calcValue);
        if (healValue > 0 && attacker.IsMain)
        {
            GTWorld.Instance.Fly.Show(healValue.ToString(), defender.Pos, EFlyWordType.TYPE_AVATAR_HEAL);
        }
        UpdateHP(defender);
        return true;
    }

    public static bool CalcSuckBlood(Character attacker, Character defender, int fixValue, float percent)
    {
        int calcValue = (int)(percent * attacker.CurrAttr.AP + fixValue);
        Vector3 defenderPos = defender.Pos;
        int hurtValue = defender.BeDamage(calcValue);
        int healValue = attacker.AddHP(hurtValue);
        if (hurtValue > 0 && attacker.IsMain)
        {
            GTWorld.Instance.Fly.Show(hurtValue.ToString(), defenderPos, EFlyWordType.TYPE_AVATAR_HURT);
        }
        if (healValue > 0 && attacker.IsMain)
        {
            GTWorld.Instance.Fly.Show(healValue.ToString(), defenderPos, EFlyWordType.TYPE_AVATAR_HEAL);
        }
        if (defender.IsMain || attacker.IsMain)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_AVATAR_HP);
        }
        return true;
    }

    public static bool CalcAddBuff(Character attacker, Character defender, int buffID)
    {
        defender.Skill.AddBuff(buffID, defender, attacker);
        return true;
    }

    public static void CalcAddAttr(Character carryer, EAttr attrID, EAttr refAttrID, float percent, int fixValue, bool onBegin)
    {
        int curAttrValue = carryer.CurrAttr.GetAttr(attrID);
        int perAttrValue = carryer.CurrAttr.GetAttr(refAttrID);
        int newAttrValue = 0;
        if(onBegin)
        {
            newAttrValue = curAttrValue + (int)((perAttrValue * (percent))) + fixValue;
        }
        else
        {
            newAttrValue = curAttrValue - (int)((perAttrValue * (percent))) - fixValue;
        }
        carryer.CurrAttr.Update(attrID, newAttrValue);
    }

    public static void CalcSubAttr(Character carryer, EAttr attrID, EAttr refAttrID, float percent, int fixValue, bool onBegin)
    {
        int curAttrValue = carryer.CurrAttr.GetAttr(attrID);
        int perAttrValue = carryer.CurrAttr.GetAttr(refAttrID);
        int newAttrValue = 0;
        if (onBegin)
        {
            newAttrValue = curAttrValue - (int)((perAttrValue * (percent))) - fixValue;
        }
        else
        {
            newAttrValue = curAttrValue + (int)((perAttrValue * (percent))) + fixValue;
        }
        carryer.CurrAttr.Update(attrID, newAttrValue);
    }

    public static void CalcCharacterOperateError(Resp resp)
    {
        if (resp == Resp.TYPE_NO || resp == Resp.TYPE_YES)
        {
            return;
        }
        string key = resp.ToString();
        DLocalString db = ReadCfgLocalString.GetDataById(key);
        if (db == null)
        {
            return;
        }
        GTItemHelper.ShowTip(db.Value);
    }

    public static void SetTarget(Character source, Character target)
    {
        source.Target = target;
    }

    public static void ClearTarget(Character source)
    {
        source.Target = null;
    }

    public static void UpdateHP(Character cc)
    {
        if (cc.IsMain)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_AVATAR_HP);
        }
        if (cc.Type == EActorType.PARTNER && cc.Host == GTWorld.Main)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_PARTNER_HP);
        }
        if (cc.IsBoss())
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_BOSS_HP);
        }
    }
}
