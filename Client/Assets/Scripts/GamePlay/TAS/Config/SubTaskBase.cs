using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    public class SubTaskBase : DCFG
    {
        public string           Desc = string.Empty;
        public ETaskSubFuncType Func = ETaskSubFuncType.TYPE_TALK;

        public override void Read(XmlElement os)
        {
            this.Func = (ETaskSubFuncType)os.GetInt32("Func");
            this.Desc = os.GetString("Desc");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Func", (int)Func);
            DCFG.Write(doc, os, "Desc",      Desc);
        }
    }
}