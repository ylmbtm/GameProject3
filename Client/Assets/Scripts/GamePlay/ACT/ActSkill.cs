using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Reflection;
using System;

namespace ACT
{
    [Serializable]
    public class ActSkill : ActTree
    {
        [SerializeField]
        public int                  ID;
        [SerializeField]
        public int                  Lv           = 1;
        [SerializeField]
        public string               Name         = string.Empty;
        [SerializeField]
        public float                CastDist = 5;
        [SerializeField]
        public float                CD           = 2;
        [SerializeField]
        public ESkillCostType       CostType     = ESkillCostType.NO;
        [SerializeField]
        public int                  CostNum      = 0;
        [SerializeField]
        public ESkillType           Type         = ESkillType.Postive;
        [SerializeField]
        public ESkillPos            Pos          = ESkillPos.Skill_0;
        [SerializeField]
        public ESkillMoveType       MoveType     = ESkillMoveType.TYPE_CANNOT_MOVE;

        public Character            Caster   { get; set; }
        public Character            Target   { get { return Caster.Target; } }
        public bool                 FirstUse { get; set; }
        public bool                 CanMove  { get; set; }


        public ActSkill()
        {
            this.EventType = EActEventType.Special;
            this.Skill = this;
            this.FirstUse = true;
        }

        public override void Begin()
        {
            this.Skill = this;
            this.StatTime = Time.time;
            this.Status = EActStatus.SELFEND;
            for (int i = 0; i < Children.Count; i++)
            {
                ActItem child = Children[i];
                child.ApplyCenter = Skill.Caster.CacheTransform;
            }
            this.FirstUse = false;
        }

        public bool IsCD()
        {
            if (this.Status == EActStatus.SELFEND)
            {
                return true;
            }
            if (this.FirstUse)
            {
                return false;
            }
            if (this.CD > 0 && (Time.time - this.StatTime < this.CD))
            {
                return true;
            }
            return false;
        }

        public float GetLeftTime()
        {
            return CD - (Time.time - this.StatTime);
        }

        public override void AddChild(ActItem act)
        {
            base.AddChild(act);
            act.Skill = this;
        }

        public override void DelChild(ActItem act)
        {
            base.DelChild(act);
        }
    }
}

