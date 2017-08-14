using UnityEngine;
using System.Collections;
using System.Xml;
using System.Collections.Generic;
using System;
using Protocol;

namespace BIE
{
    public class GuideSystem
    {
        public const int GUIDE_KEY  = 1;

        public bool      UseGuide   = true;
        public bool      PauseGuide = false;
        public UInt16    CurId;
        public GuideBase CurGuide;

        private Dictionary<string, Transform> m_GuideEvents = new Dictionary<string, Transform>();
        private GuideConfig                   m_GuideConfig = new GuideConfig();

        public void Startup()
        {
            if (UseGuide == false)
            {
                return;
            }
            m_GuideConfig.Load("Text/Guide/Guide");
            XGuide data = DataDBSGuide.GetDataById(GUIDE_KEY);
            if (data != null)
            {
                this.CurId = (ushort)data.Id;
                this.CurId++;
                this.CurGuide = CreateGuide(this.CurId);
            }
            else
            {
                this.CurId = 1;
                this.CurGuide = CreateGuide(this.CurId);
            }
        }

        public void Execute()
        {
            if (CurGuide == null || PauseGuide == true || UseGuide == false)
            {
                return;
            }
            if (CurGuide.State == EGuideState.TYPE_NONE)
            {
                if (CurGuide.Check())
                {
                    CurGuide.State = EGuideState.TYPE_ENTER;
                    CurGuide.Enter();
                    CurGuide.State = EGuideState.TYPE_EXECUTE;
                };
            }
            if (CurGuide.State == EGuideState.TYPE_EXECUTE)
            {
                CurGuide.Execute();
            }
            if (CurGuide.State == EGuideState.TYPE_FINISH)
            {
                MoveNext();
            }
        }

        public void MoveNext()
        {
            if(this.CurGuide.IsSavePoint)
            {
                XGuide guide = new XGuide();
                guide.Id = this.CurGuide.Id;
                DataDBSGuide.Update(GUIDE_KEY, guide);
            }
            this.CurId++;
            this.CurGuide = CreateGuide(this.CurId);
        }

        public void ResetCurGuide()
        {
            if (CurGuide != null)
            {
                CurGuide.Reset();
            }
        }

        public GuideBase CreateGuide(UInt16 id)
        {
            GuideBase guide = null;
            if (id <= m_GuideConfig.Items.Count)
            {
                guide = m_GuideConfig.Items[id - 1];
            }
            if (guide != null)
            {
                guide.Container = this;
                return guide;
            }
            else
            {
                return null;
            }
        }

        public Transform GetData(string key)
        {
            if(string.IsNullOrEmpty(key))
            {
                return null;
            }
            else
            {
                Transform trans = null;
                m_GuideEvents.TryGetValue(key, out trans);
                return trans;
            }
        }

        public void AddGuideListener(string key, Transform trans)
        {
            if (string.IsNullOrEmpty(key))
            {
                return;
            }
            if (trans == null)
            {
                return;
            }
            m_GuideEvents[key] = trans;
        }

        public void DelGuideListener(string key)
        {
            if (string.IsNullOrEmpty(key))
            {
                return;
            }
            m_GuideEvents.Remove(key);
        }
    }
}