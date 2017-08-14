using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;

[Serializable]
public class CharacterAttr
{
    public int HP;
    public int MaxHP;
    public int MP;
    public int MaxMP;
    public int SP;
    public int MaxSP;

    public int AP;
    public int DF;
    public int HPRECOVER;
    public int MPRECOVER;
    public int SPRECOVER;
    public int CRIT;
    public int CRITDAMAGE;
    public int CRITDF;
    public int SUCK;
    public int DODGE;
    public int HIT;
    public int ABSORB;
    public int REFLEX;
    public int SPEED;


    public int IAP;
    public int FAP;
    public int BAP;
    public int LAP;
    public int IDF;
    public int FDF;
    public int BDF;
    public int LDF;

    public int INJURYFREE;
    public int ATKEWHP;
    public int ATKEWMP;
    public int ATKEWSP;
    public int ATKEWAP;

    public int GetAttr(EAttr id)
    {
        switch (id)
        {
            case EAttr.HP:
                return HP;
            case EAttr.MAXHP:
                return MaxHP;
            case EAttr.MP:
                return MP;
            case EAttr.MAXMP:
                return MaxMP;
            case EAttr.SP:
                return SP;
            case EAttr.MAXSP:
                return MaxSP;
            case EAttr.AP:
                return AP;
            case EAttr.DF:
                return DF;
            case EAttr.HPRECOVER:
                return HPRECOVER;
            case EAttr.MPRECOVER:
                return MPRECOVER;
            case EAttr.SPRECOVER:
                return SPRECOVER;
            case EAttr.CRIT:
                return CRIT;
            case EAttr.CRITDAMAGE:
                return CRITDAMAGE;
            case EAttr.SUCK:
                return SUCK;
            case EAttr.DODGE:
                return DODGE;
            case EAttr.HIT:
                return HIT;
            case EAttr.ABSORB:
                return ABSORB;
            case EAttr.REFLEX:
                return REFLEX;
            case EAttr.CRITDF:
                return CRITDF;
            case EAttr.SPEED:
                return SPEED;
            case EAttr.IAP:
                return IAP;
            case EAttr.FAP:
                return FAP;
            case EAttr.BAP:
                return BAP;
            case EAttr.LAP:
                return LAP;
            case EAttr.IDF:
                return IDF;
            case EAttr.FDF:
                return FDF;
            case EAttr.BDF:
                return BDF;
            case EAttr.LDF:
                return LDF;
            case EAttr.INJURY:
                return INJURYFREE;
            case EAttr.EAP:
                return ATKEWAP;
            case EAttr.EHP:
                return ATKEWHP;
            case EAttr.EMP:
                return ATKEWMP;
            case EAttr.ESP:
                return ATKEWSP;
            default:
                return -1;
        }
    }

    public int CopyFrom(Dictionary<EAttr, int> dict)
    {
        Dictionary<EAttr, int>.Enumerator em = dict.GetEnumerator();
        while (em.MoveNext())
        {
            Update(em.Current.Key, em.Current.Value);
        }
        this.MaxHP = dict[EAttr.HP];
        this.MaxMP = dict[EAttr.MP];
        this.MaxSP = dict[EAttr.SP];
        em.Dispose();
        return 0;
    }

    public int CopyFrom(CharacterAttr src)
    {
        this.HP                   = src.HP;
        this.MP                   = src.MP;
        this.SP                   = src.SP;
        this.MaxHP                = src.MaxHP;
        this.MaxMP                = src.MaxMP;
        this.MaxSP                = src.MaxSP;

        this.AP                   = src.AP;
        this.DF                   = src.DF;
        this.HPRECOVER            = src.HPRECOVER;
        this.MPRECOVER            = src.MPRECOVER;
        this.SPRECOVER            = src.SPRECOVER;
        this.CRIT                 = src.CRIT;
        this.CRITDAMAGE           = src.CRITDAMAGE;
        this.CRITDF               = src.CRITDF;
        this.SUCK                 = src.SUCK;
        this.DODGE                = src.DODGE;
        this.HIT                  = src.HIT;
        this.ABSORB               = src.ABSORB;
        this.REFLEX               = src.REFLEX;
        this.SPEED                = src.SPEED;

        this.IAP                  = src.IAP;
        this.FAP                  = src.FAP;
        this.BAP                  = src.BAP;
        this.LAP                  = src.LAP;
        this.IDF                  = src.IDF;
        this.FDF                  = src.FDF;
        this.BDF                  = src.BDF;
        this.LDF                  = src.LDF;

        this.INJURYFREE           = src.INJURYFREE;
        this.ATKEWHP              = src.ATKEWHP;
        this.ATKEWMP              = src.ATKEWMP;
        this.ATKEWSP              = src.ATKEWSP;
        this.ATKEWAP              = src.ATKEWAP;
        return 0;
    }

    public int CopyFrom(List<int> list)
    {
        for (int i = 0; i < list.Count; i++)
        {
            EAttr id = (EAttr)(i + 1);
            Update(id, list[i]);
        }
        return 0;
    }

    public int Update(EAttr id, int value)
    {
        switch (id)
        {
            case EAttr.HP:
                HP = value;
                break;
            case EAttr.MAXHP:
                MaxHP = value;
                break;
            case EAttr.MP:
                MP = value;
                break;
            case EAttr.MAXMP:
                MaxMP = value;
                break;
            case EAttr.SP:
                SP = value;
                break;
            case EAttr.MAXSP:
                MaxSP = value;
                break;
            case EAttr.AP:
                AP = value;
                break;
            case EAttr.DF:
                DF = value;
                break;
            case EAttr.HPRECOVER:
                HPRECOVER = value;
                break;
            case EAttr.MPRECOVER:
                MPRECOVER = value;
                break;
            case EAttr.SPRECOVER:
                SPRECOVER = value;
                break;
            case EAttr.CRIT:
                CRIT = value;
                break;
            case EAttr.CRITDAMAGE:
                CRITDAMAGE = value;
                break;
            case EAttr.SUCK:
                SUCK = value;
                break;
            case EAttr.DODGE:
                DODGE = value;
                break;
            case EAttr.HIT:
                HIT = value;
                break;
            case EAttr.ABSORB:
                ABSORB = value;
                break;
            case EAttr.REFLEX:
                REFLEX = value;
                break;
            case EAttr.CRITDF:
                CRITDF = value;
                break;
            case EAttr.SPEED:
                SPEED = value;
                break;
            case EAttr.IAP:
                IAP = value;
                break;
            case EAttr.FAP:
                FAP = value;
                break;
            case EAttr.BAP:
                BAP = value;
                break;
            case EAttr.LAP:
                LAP = value;
                break;
            case EAttr.IDF:
                IDF = value;
                break;
            case EAttr.FDF:
                FDF = value;
                break;
            case EAttr.BDF:
                BDF = value;
                break;
            case EAttr.LDF:
                LDF = value;
                break;
            case EAttr.INJURY:
                INJURYFREE = value;
                break;
            case EAttr.EAP:
                ATKEWAP = value;
                break;
            case EAttr.EHP:
                ATKEWHP = value;
                break;
            case EAttr.EMP:
                ATKEWMP = value;
                break;
            case EAttr.ESP:
                ATKEWSP = value;
                break;
            default:
                break;
        }
        return 0;
    }
}
