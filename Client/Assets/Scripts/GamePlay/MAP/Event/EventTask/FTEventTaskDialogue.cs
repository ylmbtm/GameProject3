using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventTaskDialogue : FTEventTask
    {
        public int  StDialogueID;
        public int  EdDialogueID;
        public bool IsShowSkip  = false;

        public override void Trigger()
        {
            base.Trigger();
            UIDialogue dialogueWindow = (UIDialogue)GTWindowManager.Instance.OpenWindow(EWindowID.UIDialogue);
            dialogueWindow.ShowDialogue(StDialogueID, EdDialogueID, IsShowSkip, Finish);
        }

        public override void Finish()
        {
            GTWindowManager.Instance.HideWindow(EWindowID.UIDialogue);
            base.Finish();
        }
    }
}