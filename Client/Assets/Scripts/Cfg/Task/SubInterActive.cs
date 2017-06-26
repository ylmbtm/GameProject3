using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    [Serializable]
    public class SubInterActive : SubTaskBase
    {
        public string  Cmd      = string.Empty;
        public string  AnimName = string.Empty;
        public TaskNpc Npc      = new TaskNpc();


        public SubInterActive()
        {
            Func = ETaskSubFuncType.TYPE_INTERACTIVE;
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.Cmd      = os.GetString("Cmd");
            this.AnimName = os.GetString("AnimName");
            foreach (var current in GetChilds(os))
            {
                switch (current.Name)
                {
                    case "Npc":
                        this.Npc = ReadObj<TaskNpc>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Cmd",      Cmd);
            DCFG.Write(doc, os, "AnimName", AnimName);
            DCFG.Write(doc, os, "Npc",      Npc);
        }
    }
}

