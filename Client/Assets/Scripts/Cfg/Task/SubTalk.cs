using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    [Serializable]
    public class SubTalk: SubTaskBase
    {
        public List<TaskDialog> Dialogs = new List<TaskDialog>();
        public TaskNpc          Npc     = new TaskNpc();

        public SubTalk()
        {
            Func = ETaskSubFuncType.TYPE_TALK;
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            foreach (var current in GetChilds(os))
            {
                switch (current.Name)
                {
                    case "Npc":
                        this.Npc     = ReadObj<TaskNpc>(current);
                        break;
                    case "Dialogs":
                        this.Dialogs = ReadList<TaskDialog>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Npc",     Npc);
            DCFG.Write(doc, os, "Dialogs", Dialogs);
        }
    }
}

