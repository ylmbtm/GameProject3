using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

namespace BIE
{
    public class GuidePath : GuideBase
    {
        public string        TipSound         = string.Empty;
        public string        TipText          = string.Empty;
        public Vector3       TipPosition      = Vector3.zero;
        public EGuideGirlPos TipGirlPos       = EGuideGirlPos.TYPE_NONE;
        public Vector3       TargetScale      = new Vector3(5, 5, 5);
        public Vector3       TargetPos        = new Vector3(0, -2.16f, 0);
        public string        TargetEffectPath = string.Empty;

        private Character         m_Character;
        private Vector3           m_DirectionToTarget;
        private ETriggerObject    m_TargetTriggerObject;
        private GameObject        m_TargetArrow;
        private List<GameObject>  m_ArrowGameObjectList = new List<GameObject>();
        private float             m_BetweenLength       = 2f;
        private float             m_PerLength           = 0.5f;
        private float             m_Offset              = 1.2f;

        public override void Enter()
        {
            base.Enter();
            m_Character = GTWorld.Main;
            GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
            go.GetComponent<Collider>().isTrigger = true;
            m_TargetTriggerObject = go.AddComponent<ETriggerObject>();
            m_TargetTriggerObject.transform.localScale = TargetScale;
            m_TargetTriggerObject.transform.localPosition = TargetPos;
            m_TargetTriggerObject.GetComponent<MeshRenderer>().enabled = false;
            m_TargetTriggerObject.onTriggerEnter = OnTriggerEnter;
            m_TargetArrow = GTResourceManager.Instance.Load<GameObject>("Effect/other_xinshou_yidong", true);
            m_TargetArrow.transform.localPosition = TargetPos;
            for (int i = 0; i < 20; i++)
            {
                GameObject g = GTResourceManager.Instance.Load<GameObject>("Effect/jiantourow", true);
                m_ArrowGameObjectList.Add(g);
            }

            UIGuide guideWindow = (UIGuide)GTWindowManager.Instance.OpenWindow(EWindowID.UIGuide);
            guideWindow.ShowGuideBase(this);
            guideWindow.ShowGuideTip(TipText, TipPosition, TipGirlPos);
            GTAudioManager.Instance.PlayEffectAudio(TipSound);
        }

        public override void Execute()
        {
            base.Execute();
            this.UpdateArrowList();
        }

        public override void Finish()
        {
            base.Finish();
            m_TargetTriggerObject.gameObject.SetActive(false);
            m_TargetArrow.gameObject.SetActive(false);
            for (int i = 0; i < m_ArrowGameObjectList.Count; i++)
            {
                GameObject.DestroyImmediate(m_ArrowGameObjectList[i]);
            }
            m_ArrowGameObjectList.Clear();
            GTWindowManager.Instance.HideWindow(EWindowID.UIGuide);
        }

        public override void Stop()
        {
            base.Stop();
            m_TargetTriggerObject.gameObject.SetActive(false);
            m_TargetArrow.gameObject.SetActive(false);
            GTWindowManager.Instance.HideWindow(EWindowID.UIGuide);
        }

        public override void Reset()
        {
            m_ArrowGameObjectList.Clear();
            base.Reset();
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.TipSound          = os.GetString("TipSound");
            this.TipPosition       = os.GetVector3("TipPosition");
            this.TipText           = os.GetString("TipText");
            this.TipGirlPos        = (EGuideGirlPos)os.GetInt32("TipGirlPos");
            this.TargetScale       = os.GetVector3("TargetScale");
            this.TargetPos         = os.GetVector3("TargetPos");
            this.TargetEffectPath  = os.GetString("TargetEffectPath");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "TipSound",         TipSound);
            DCFG.Write(doc, os, "TipPosition",      TipPosition);
            DCFG.Write(doc, os, "TipText",          TipText);
            DCFG.Write(doc, os, "TipGirlPos",  (int)TipGirlPos);
            DCFG.Write(doc, os, "TargetScale",      TargetScale);
            DCFG.Write(doc, os, "TargetPos",        TargetPos);
            DCFG.Write(doc, os, "TargetEffectPath", TargetEffectPath);
        }

        void OnTriggerEnter(Collider obj)
        {
            if (obj.gameObject == null)
            {
                return;
            }
            CharacterView view = obj.gameObject.GetComponent<CharacterView>();
            if (view == null)
            {
                return;
            }
            if(m_Character == view.Owner)
            {
                GTAsync.Instance.Run(Finish);
            }
        }

        void UpdateArrowList()
        {
            m_DirectionToTarget = TargetPos - m_Character.Pos;
            m_DirectionToTarget = m_DirectionToTarget.normalized;
            m_DirectionToTarget.y = 0;
            for (int i = 0; i < m_ArrowGameObjectList.Count; i++)
            {
                GameObject obj = m_ArrowGameObjectList[i];
                Vector3 pos = TargetPos - (i * (m_BetweenLength + m_PerLength) + m_Offset) * m_DirectionToTarget;
                pos.y = m_Character.Pos.y;
                obj.transform.position = pos;
                obj.transform.rotation = Quaternion.LookRotation(m_DirectionToTarget);
                float dir = Vector3.Distance(pos, m_Character.Pos);
                if (dir > (m_Offset + m_BetweenLength) && Vector3.Dot(pos - m_Character.Pos, m_DirectionToTarget) > 0)
                {
                    obj.SetActive(true);
                }
                else
                {
                    obj.SetActive(false);
                }
            }
        }
    }  
}