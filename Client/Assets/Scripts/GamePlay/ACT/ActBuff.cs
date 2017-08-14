using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System;

namespace ACT
{
    public class ActBuff : ActBuffItem
    {
        [SerializeField]
        public int                  ID;
        [SerializeField]
        public EBuffType            Type          = EBuffType.None;
        [SerializeField]
        public string               Name          = string.Empty;
        [SerializeField]
        public string               Icon          = string.Empty;
        [SerializeField]
        public string               Desc          = string.Empty;
        [SerializeField]
        public EBuffDestroyType     DestroyType   = EBuffDestroyType.BY_TIMEEND;
        [SerializeField]
        public EBuffOverlayType     OverlayType   = EBuffOverlayType.UnChange;
        [SerializeField]
        public int                  MaxOverlayNum = 1;

        public int                  CurOverlayNum { get; private set; }
        public EActStatus           Status        { get; private set; }
        public float                PastTime      { get { return Time.realtimeSinceStartup - this.StatTime; } }
        public float                StatTime      { get; private set; }
        public float                LeftTime      { get { return Duration > 0 ? Duration - (Time.realtimeSinceStartup - this.StatTime) : Duration; } }

        public ActBuff()
        {
            this.CurOverlayNum = 0;
            this.Buff = this;
            this.Status = EActStatus.INITIAL;
        }

        public override bool Check()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                if (Children[i].Check() == false)
                {
                    return false;
                }
            }
            return true;
        }

        public override void Enter()
        {
            this.StatTime = Time.realtimeSinceStartup;
            this.CurOverlayNum++;
            for (int i = 0; i < Children.Count; i++)
            {
                ActBuffItem child = Children[i];
                child.Enter();
            }
            this.Status = EActStatus.STARTUP;
        }

        public override void Execute()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                ActBuffItem child = Children[i];
                child.Execute();
            }
            this.Status = EActStatus.RUNNING;
            if(Carryer.IsDead())
            {
                Stop();
            }
        }

        public override void Stop()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                ActBuffItem child = Children[i];
                child.Stop();
            }
            this.Status = EActStatus.SUCCESS;
        }

        public override void Exit()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                ActBuffItem child = Children[i];
                child.Exit();
            }
            this.Status = EActStatus.SUCCESS;
        }

        public override void Refresh()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                ActBuffItem child = Children[i];
                child.Refresh();
            }
            this.StatTime = Time.realtimeSinceStartup;
        }

        public override void Overlay()
        {
            CurOverlayNum++;
            for (int i = 0; i < Children.Count; i++)
            {
                ActBuffItem child = Children[i];
                child.Overlay();
            }
        }

        public void Loop()
        {
            if (this.Status == EActStatus.INITIAL)
            {
                Enter();
            }
            if (this.Status == EActStatus.STARTUP)
            {
                Status = EActStatus.RUNNING;
            }
            if (Status == EActStatus.RUNNING)
            {
                if (PastTime >= Duration)
                {
                    Exit();
                }
                else
                {
                    Execute();
                }     
            }
        }

        public override void LoadDoc(XmlElement element)
        {
            base.LoadDoc(element);
            XmlElement child = element.FirstChild as XmlElement;
            while (child != null)
            {
                Type type = System.Type.GetType("ACT" + "." + child.Name);
                ActBuffItem act = (ActBuffItem)System.Activator.CreateInstance(type);
                act.LoadDoc(child);
                AddChild(act);
                child = child.NextSibling as XmlElement;
            }
        }

        public override void SaveDoc(XmlDocument doc, XmlElement element)
        {
            base.SaveDoc(doc, element);
            for (int i = 0; i < Children.Count; i++)
            {
                ActBuffItem act = Children[i];
                XmlElement child = doc.CreateElement(act.GetType().Name);
                element.AppendChild(child);
                act.SaveDoc(doc, child);
            }
        }
    }
}