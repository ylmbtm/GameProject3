using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    [Serializable]
    public class TaskDialog : DCFG
    {
        public int                NpcID;
        public ETaskDialogRole    Role         = ETaskDialogRole.TYPE_PLAYER;
        public ETaskDialogPos     Pos          = ETaskDialogPos.TYPE_LF;
        public ETaskDialogAction  Action       = ETaskDialogAction.TYPE_NEXT;
        public ETaskDialogContent ContentShow  = ETaskDialogContent.TYPE_NORMAL;
        public string             Content      = string.Empty;
        public string             NpcAnim      = string.Empty;
        public int                VoiceID;
        public float              Delay;

    
        public override void Read(XmlElement os)
        {
            this.NpcID          = os.GetInt32("NpcID");
            this.Role           = (ETaskDialogRole)os.GetInt32("Role");
            this.Pos            = (ETaskDialogPos)os.GetInt32("Pos");
            this.Action         = (ETaskDialogAction)os.GetInt32("Action");
            this.ContentShow    = (ETaskDialogContent)os.GetInt32("ContentShow");
            this.Content        = os.GetString("Content");
            this.NpcAnim        = os.GetString("NpcAnim");
            this.VoiceID        = os.GetInt32("VoiceID");
            this.Delay          = os.GetFloat("Delay");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Role",        (int)Role);
            DCFG.Write(doc, os, "NpcID",       this.NpcID);
            DCFG.Write(doc, os, "Pos",         (int)Pos);
            DCFG.Write(doc, os, "Action",      (int)Action);
            DCFG.Write(doc, os, "ContentShow", (int)ContentShow);
            DCFG.Write(doc, os, "Content",     this.Content);
            DCFG.Write(doc, os, "VoiceID",     this.VoiceID);
            DCFG.Write(doc, os, "Delay",       this.Delay);
            DCFG.Write(doc, os, "NpcAnim",     this.NpcAnim);
        }
    }
}

