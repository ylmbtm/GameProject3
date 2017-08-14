using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;
using System.Reflection;

namespace ACT
{
    [Serializable]
    public class ActItem : ActNode
    {
        public    ActSkill           Skill           { get; set; }  
        public    ActItem            Parent          { get; set; }
        public    Transform          ApplyCenter     { get; set; }
        public    Vector3            ApplyHitPoint   { get; set; }
        public    Vector3            ApplyHitDir     { get; set; }

        public    EActEventType      EventType       { get; protected set; }
        public    EActStatus         Status          { get; protected set; }

        public    float              Duration        { get { return EdTime - StTime; } }
        public    float              PastTime        { get { return Time.time - this.StatTime; } }
        public    float              StatTime        { get; protected set; }


        [SerializeField]
        public float                 StTime;
        [SerializeField]
        public float                 EdTime;
        [SerializeField][ActFieldAttribute]
        public List<ActItem>         Children     = new List<ActItem>();

        protected HashSet<int>        mFinishList = new HashSet<int>();
        protected List<Character>     mTargetList = new List<Character>();

        protected virtual void ExecuteChildren()
        {
            LoopChildren(Children);
        }

        protected virtual void Execute()
        {
            this.Status = EActStatus.RUNNING;
        }

        protected virtual bool Trigger()
        {
            this.Status = EActStatus.TRIGGER;
            return true;
        }

        protected virtual void End()
        {
            this.Status = EActStatus.SELFEND;
        }

        protected virtual void Exit()
        {
            this.Status = EActStatus.SUCCESS;
        }

        protected void LoopChildren(List<ActItem> list)
        {
            if (mFinishList.Count >= list.Count)
            {
                Exit();
                return;
            }
            if (list.Count > 0)
            {
                for (int i = 0; i < list.Count; i++)
                {
                    if (mFinishList.Contains(i))
                    {
                        continue;
                    }
                    ActItem item = list[i];
                    item.Loop();
                    if (item.Status == EActStatus.SUCCESS)
                    {
                        mFinishList.Add(i);
                    }
                }
            }
            else
            {
                Exit();
            }
        }

        public static ActItem Clone(ActItem src)
        {
            ActItem item = (ActItem)Activator.CreateInstance(src.GetType());
            FieldInfo[] fields = item.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo newField = fields[i];
                if (newField.IsDefined(typeof(SerializeField), true) &&
                    newField.IsDefined(typeof(ActFieldAttribute),true) == false )
                {
                    FieldInfo oldField = src.GetType().GetField(newField.Name);
                    if (oldField != null)
                    {
                        newField.SetValue(item, oldField.GetValue(src));
                    }
                }
            }
            for (int i = 0; i < src.Children.Count; i++)
            {
                ActItem child = Clone(src.Children[i]);
                item.AddChild(child);
            }
            return item;
        }

        public static T Clone<T>(ActItem src) where T : ActItem
        {
            T item = Activator.CreateInstance<T>();
            FieldInfo[] fields = item.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo newField = fields[i];
                if (newField.IsDefined(typeof(SerializeField), true) &&
                    newField.IsDefined(typeof(ActFieldAttribute), true) == false)
                {
                    FieldInfo oldField = src.GetType().GetField(newField.Name);
                    if (oldField != null)
                    {
                        newField.SetValue(item, oldField.GetValue(src));
                    }
                }
            }
            for (int i = 0; i < src.Children.Count; i++)
            {
                ActItem child = Clone(src.Children[i]);
                item.AddChild(child);
            }
            return item;
        }

        public override void LoadDoc(XmlElement element)
        {
            base.LoadDoc(element);
            XmlElement child = element.FirstChild as XmlElement;
            while (child != null)
            {
                Type type = System.Type.GetType("ACT" + "." + child.Name);

                try
                {
                    ActItem act = (ActItem)System.Activator.CreateInstance(type);
                    act.LoadDoc(child);
                    AddChild(act);
                    child = child.NextSibling as XmlElement;
                }
                catch (Exception ex)
                {
                    Debug.LogError(child.Name);
                    child = child.NextSibling as XmlElement;
                }
            }
        }

        public override void SaveDoc(XmlDocument doc, XmlElement element)
        {
            base.SaveDoc(doc, element);
            for (int i = 0; i < Children.Count; i++)
            {
                ActItem act = Children[i];
                XmlElement child = doc.CreateElement(act.GetType().Name);
                element.AppendChild(child);
                act.SaveDoc(doc, child);
            }
        }

        public virtual void Begin()
        {
            this.Skill = this.Skill == null ? Parent.Skill : Skill;
            this.StatTime =  Time.time;
            this.Status = EActStatus.STARTUP;
        }

        public virtual void Loop()
        {
            if (Status == EActStatus.INITIAL)
            {
                Begin();
            }
            if (PastTime < StTime)
            {
                return;
            }
            if (Status == EActStatus.SELFEND)
            {
                ExecuteChildren();
            }
            else
            {
                switch (EventType)
                {
                    case EActEventType.Instant:
                        {
                            DoInstant();
                        }
                        break;
                    case EActEventType.Subtain:
                        {
                            DoSubtain();
                        }
                        break;
                }
            }
        }

        public virtual void Clear()
        {
            if (this.Status != EActStatus.SUCCESS)
            {
                Exit();
            }
            this.Status = EActStatus.INITIAL;
            this.Children.ForEach((item) => item.Clear());
            this.mFinishList.Clear();
            this.mTargetList.Clear();
        }

        public virtual void Stop()
        {
            this.Children.ForEach((item) => item.Stop());
            Clear();
        }

        protected virtual void DoInstant()
        {
            if (Status == EActStatus.STARTUP)
            {
                if(Trigger())
                {
                    Execute();
                    End();
                }
                else
                {
                    Exit();
                }
            }
        }

        protected virtual void DoSubtain()
        {
            if (Status == EActStatus.STARTUP)
            {
                if (!Trigger())
                {
                    Exit();
                }
            }
            Execute();
            if (PastTime >= EdTime && Status == EActStatus.RUNNING)
            {
                End();
            }
        }

        public virtual void AddChild(ActItem item)
        {
            Children.Add(item);
            item.Parent = this;
        }

        public virtual void DelChild(ActItem item)
        {
            Children.Remove(item);
        }

        public void AddInAttackList(Character cc)
        {
            mTargetList.Add(cc);
        }

        public void AddRangeInAttackList(List<Character> list)
        {
            mTargetList.AddRange(list);
        }

        public void ClearAttackList()
        {
            mTargetList.Clear();
        }
    }
}
