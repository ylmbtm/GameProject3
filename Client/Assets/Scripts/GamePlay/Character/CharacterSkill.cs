using UnityEngine;
using System.Collections;
using System;
using ACT;
using System.Collections.Generic;

public class CharacterSkill : ICharacterComponent
{
    private Character                       mCharacter;
    private ActSkillSystem                  mContainer;
    private ActSkill                        mCurrentSkill;
    private ActSkill                        mPassiveSkill;
    private List<ActSkill>                  mNormalAttacks = new List<ActSkill>();
    private Dictionary<ESkillPos, ActSkill> mSkillAttacks = new Dictionary<ESkillPos, ActSkill>();
    private Dictionary<int, ActBuff>        mBuffs = new Dictionary<int, ActBuff>();
    private List<int>                       mDeleteBuffs = new List<int>();
    private int                             mComboIndex = 0;


    public float WarnerDist = 20;
    public float AttackDist = 5;
    public float GoHomeDist = 30;

    public CharacterSkill(Character c)
    {
        this.mCharacter = c;
        this.mContainer = new ActSkillSystem();
        this.mContainer.ActorID = c.ID;
        this.mContainer.LoadDoc();
        for (int i = 0; i < mContainer.Skills.Count; i++)
        {
            ActSkill skill = mContainer.Skills[i];
            if (skill.Pos == ESkillPos.Skill_0)
            {
                mNormalAttacks.Add(skill);
                this.AttackDist = skill.CastDist;
            }
            else
            {
                mSkillAttacks[skill.Pos] = skill;
            }
        }
        if (c.IsMain)
        {
            WarnerDist = 100;
        }
    }

    public void     Execute()
    {
        if (mCurrentSkill != null)
        {
            mCurrentSkill.Loop();
        }
        if (mCurrentSkill != null && mCurrentSkill.Status == EActStatus.SUCCESS)
        {
            mCurrentSkill.Clear();
            mCurrentSkill = null;
        }
        Dictionary<int, ActBuff>.Enumerator em = mBuffs.GetEnumerator();
        while (em.MoveNext())
        {
            ActBuff buff = em.Current.Value;
            buff.Loop();
            if (buff.Status == EActStatus.SUCCESS)
            {
                mDeleteBuffs.Add(buff.ID);
            }
        }
        em.Dispose();
        for (int i = 0; i < mDeleteBuffs.Count; i++)
        {
            mBuffs.Remove(mDeleteBuffs[i]);
        }
        mDeleteBuffs.Clear();
    }

    public void     Release()
    {
        StopCurrentSkill();
        DelBuffAll();
        mBuffs.Clear();
    }

    public void     ExitCurrentSkill()
    {
        if (mCurrentSkill != null)
        {
            mCurrentSkill.Clear();
            mCurrentSkill = null;
        }
    }

    public void     StopCurrentSkill()
    {
        if (mCurrentSkill != null)
        {
            mCurrentSkill.Stop();
            mCurrentSkill = null;
        }
    }

    public void     UseSkillById(int id)
    {
        ActSkill skill = GetSkill(id);
        if (skill == null)
        {
            return;
        }
        if (skill.Pos == ESkillPos.Skill_0)
        {
            if (mComboIndex < mNormalAttacks.Count - 1)
            {
                mComboIndex++;
            }
            else
            {
                mComboIndex = 0;
            }
        }
        mCurrentSkill = skill;
        mCurrentSkill.Caster = mCharacter;
    }

    public void     StopCombo()
    {
        mComboIndex = 0;
    }

    public ActSkill GetSkill(int id)
    {
        for (int i = 0; i < mContainer.Skills.Count; i++)
        {
            if (mContainer.Skills[i].ID == id)
            {
                return mContainer.Skills[i];
            }
        }
        return null;
    }

    public ActSkill GetSkill(ESkillPos pos)
    {
        if (pos == ESkillPos.Skill_0)
        {
            return mNormalAttacks.Count > mComboIndex ? mNormalAttacks[mComboIndex] : null;
        }
        else
        {
            ActSkill skill = null;
            mSkillAttacks.TryGetValue(pos, out skill);
            return skill;
        }
    }

    public ActSkill GetCurrent()
    {
        return mCurrentSkill;
    }

    public ActSkill GetPassive()
    {
        return mPassiveSkill;
    }

    public ActSkill SelectSkillByEnemy(Character enemy)
    {
        foreach (var current in mSkillAttacks)
        {
            ActSkill s = current.Value;
            if (s.CastDist > 0 && s.IsCD() == false)
            {
                if (enemy != null)
                {
                    if(GTTools.GetHorizontalDistance(mCharacter.Pos, enemy.Pos) < s.CastDist)
                    {
                        return s;
                    }
                }
                else
                {
                    return s;
                }
            }
        }
        return GetSkill(ESkillPos.Skill_0);
    }

    public bool AddBuff(int id, Character carryer, Character caster)
    {
        ActBuff newBuff = ActBuffSystem.Create(id, carryer, caster);
        if (newBuff == null)
        {
            return false;
        }
        else
        {
            if(newBuff.Check())
            {
                ActBuff curBuff = null;
                mBuffs.TryGetValue(id, out curBuff);
                if (curBuff == null)
                {
                    mBuffs[newBuff.ID] = newBuff;
                }
                else
                {
                    switch(curBuff.OverlayType)
                    {
                        case EBuffOverlayType.UnChange:
                            break;
                        case EBuffOverlayType.Overlay:
                            if (curBuff.MaxOverlayNum > 0 && curBuff.CurOverlayNum < curBuff.MaxOverlayNum)
                            {
                                curBuff.Overlay();
                            }
                            break;
                        case EBuffOverlayType.Refresh:
                            {
                                curBuff.Refresh();
                            }
                            break;
                        case EBuffOverlayType.OverlayAndRefresh:
                            if (curBuff.MaxOverlayNum > 0 && curBuff.CurOverlayNum < curBuff.MaxOverlayNum)
                            {
                                curBuff.Overlay();
                            }
                            curBuff.Refresh();
                            break;
                        default:
                            break;
                    }
                }        
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    public bool DelBuff(int id)
    {
        mDeleteBuffs.Add(id);
        return false;
    }

    public void DelBuffByType(EBuffType type, int delBuffNum = 0)
    {
        int num = 0;
        List<ActBuff> list = new List<ActBuff>(mBuffs.Values);
        for (int i = 0; i < list.Count; i++)
        {
            ActBuff current = list[i];
            if (current.Type == type)
            {
                num++;
                current.Stop();
            }
            if (delBuffNum > 0 && num >= delBuffNum)
            {
                break;
            }
        }
    }

    public void DelBuffAll()
    {
        List<ActBuff> list = new List<ActBuff>(mBuffs.Values);
        for (int i = 0; i < list.Count; i++)
        {
            list[i].Stop();
        }
    }

    public Dictionary<int,ActBuff> GetBuffs()
    {
        return mBuffs;
    }

}