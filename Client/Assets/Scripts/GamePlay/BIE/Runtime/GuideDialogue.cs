using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class GuideDialogue : GuideBase
    {
        public int StDialogueID;
        public int EdDialogueID;

        public override void Enter()
        {
            base.Enter();
            UIDialogue dialogueWindow = (UIDialogue)GTWindowManager.Instance.OpenWindow(EWindowID.UIDialogue);
            dialogueWindow.ShowDialogue(StDialogueID, EdDialogueID, IsShowSkip, Finish);
        }

        public override void Finish()
        {
            base.Finish();
            GTWindowManager.Instance.HideWindow(EWindowID.UIDialogue);
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.StDialogueID = os.GetInt32("StDialogueID");
            this.EdDialogueID = os.GetInt32("EdDialogueID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "StDialogueID", StDialogueID);
            DCFG.Write(doc, os, "EdDialogueID", EdDialogueID);
        }
    }
}