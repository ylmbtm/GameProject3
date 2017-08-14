using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class GuideOperation : GuideBase
    {
        public string                TipSound      = string.Empty;
        public string                TipText       = string.Empty;
        public Vector2               TipPosition   = Vector2.zero;
        public EGuideGirlPos         TipGirlPos    = EGuideGirlPos.TYPE_NONE;
        public string                LockKey       = "BtnName";
        public EGuideBoardType       BoardType     = EGuideBoardType.TYPE_NONE;
        public EGuideUIOperationType OperationType = EGuideUIOperationType.TYPE_CLICK;
        public EGuideRowType         RowType       = EGuideRowType.TYPE_NONE;
        public Vector2               BoardSize     = new Vector2(100, 100);

        public AudioSource           Audio
        {
            get; private set;
        }

        public override bool Check()
        {
            if(Container.GetData(LockKey))
            {
                return true;
            }
            else
            {
                return base.Check();
            }
        }

        public override void Enter()
        {
            base.Enter();
            this.Audio = GTAudioManager.Instance.PlaySound(TipSound);
            UIGuide guideWindow = (UIGuide)GTWindowManager.Instance.OpenWindow(EWindowID.UIGuide);
            guideWindow.ShowGuideBase(this);
            guideWindow.ShowGuideTip(TipText, TipPosition, TipGirlPos);
            guideWindow.ShowGuideOperation(Container.GetData(LockKey), this);
        }

        public override void Stop()
        {
            base.Stop();
            if (this.Audio != null)
            {
                this.Audio.Stop();
                this.Audio = null;
            }
            GTWindowManager.Instance.HideWindow(EWindowID.UIGuide);
        }

        public override void Finish()
        {
            base.Finish();
            if (this.Audio != null)
            {
                this.Audio.Stop();
                this.Audio = null;
            }
            GTWindowManager.Instance.HideWindow(EWindowID.UIGuide);
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.TipSound        = os.GetString("TipSound");
            this.TipPosition     = os.GetVector2("TipPosition");
            this.TipText         = os.GetString("TipText");
            this.TipGirlPos      = (EGuideGirlPos)os.GetInt32("TipGirlPos");
            this.OperationType   = (EGuideUIOperationType)os.GetInt32("OperationType");
            this.BoardType       = (EGuideBoardType)os.GetInt32("BoardType");
            this.BoardSize       = os.GetVector2("BoardSize");
            this.LockKey         = os.GetString("LockKey");
            this.RowType         = (EGuideRowType)os.GetInt32("RowType");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);  
            DCFG.Write(doc, os, "TipSound",           TipSound);
            DCFG.Write(doc, os, "TipPosition",        TipPosition);
            DCFG.Write(doc, os, "TipText",            TipText);
            DCFG.Write(doc, os, "TipGirlPos",    (int)TipGirlPos);
            DCFG.Write(doc, os, "LockKey",            LockKey);
            DCFG.Write(doc, os, "OperationType", (int)OperationType);
            DCFG.Write(doc, os, "BoardType",     (int)BoardType);
            DCFG.Write(doc, os, "RowType",       (int)RowType);
            DCFG.Write(doc, os, "BoardSize",          BoardSize);

        }
    }
}