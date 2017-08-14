using UnityEngine;
using System.Collections;
using System;
using System.Xml;
using System.Collections.Generic;

namespace BIE
{
    public class GuideConfig : DCFG
    {
        public List<GuideBase> Items = new List<GuideBase>();

        public override void Read(XmlElement os)
        {
            foreach(var current in GetChilds(os))
            {
                string typeName = current.GetString("TypeName");
                Type type = System.Type.GetType("BIE." + typeName);
                GuideBase cc = (GuideBase)System.Activator.CreateInstance(type);
                cc.Read(current);
                Items.Add(cc);
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {

        }
    }
}