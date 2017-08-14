using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

namespace BIE
{
    [System.Serializable]
    public class GuideBase: DCFG
    {
        public UInt16                   Id;
        public string                   TypeName                 = string.Empty;
        public bool                     IsLocked                 = true;
        public bool                     IsPause                  = false;
        public bool                     IsShowSkip               = false;
        public bool                     IsSavePoint              = false;
        public EGuideConditionRelation  TriggerConditionRelation = EGuideConditionRelation.AND;
        public List<CheckCondition>     TriggerConditions        = new List<CheckCondition>();

        public EGuideState              State
        {
            get; set;
        }

        public GuideSystem              Container
        {
            get; set;
        }

        public virtual bool Check()
        {
            switch(TriggerConditionRelation)
            {
                case EGuideConditionRelation.AND:
                    for (int i = 0; i < TriggerConditions.Count; i++)
                    {
                        if(TriggerConditions[i].Check() == false)
                        {
                            return false;
                        }
                    }
                    break;
                case EGuideConditionRelation.OR:
                    for (int i = 0; i < TriggerConditions.Count; i++)
                    {
                        if (TriggerConditions[i].Check() == true)
                        {
                            return true;
                        }
                    }
                    break;
            }
            return true;
        }

        public virtual void Enter()
        {

        }

        public virtual void Execute()
        {

        }
        
        public virtual void Stop()
        {
            this.State = EGuideState.TYPE_FINISH;
        }

        public virtual void Finish()
        {
            this.State = EGuideState.TYPE_FINISH;
        }

        public virtual void Reset()
        {
            this.State = EGuideState.TYPE_NONE;
        }

        public override void Read(XmlElement os)
        {
            this.Id                       = os.GetUInt16("Id");
            this.TypeName                 = os.GetString("TypeName");
            this.IsLocked                 = os.GetBool("IsLocked");
            this.IsPause                  = os.GetBool("IsPause");
            this.IsShowSkip               = os.GetBool("IsShowSkip");
            this.IsSavePoint              = os.GetBool("IsSavePoint");
            this.TriggerConditionRelation = (EGuideConditionRelation)os.GetInt32("TriggerConditionRelation");
            foreach(var current in GetChilds(os))
            {
                Type type = System.Type.GetType("BIE." + current.Name);
                CheckCondition cc = (CheckCondition)System.Activator.CreateInstance(type);
                cc.Read(current);
                TriggerConditions.Add(cc);
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {

        }
    }
}