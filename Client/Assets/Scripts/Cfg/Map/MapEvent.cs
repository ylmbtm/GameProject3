using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapEvent : MapElement
    {
        public EMapTrigger             Type            = EMapTrigger.TYPE_NONE;
        public bool                    Active          = false;
        public ECR                     Relation1       = ECR.AND;
        public ECR                     Relation2       = ECR.AND;
        public List<MapEventCondition> Conditions1     = new List<MapEventCondition>();
        public List<MapEventCondition> Conditions2     = new List<MapEventCondition>();
        public int                     TriggerNum      = 1;
        public float                   TriggerInterval = 0;
        public float                   TriggerDelay    = 0;

        public override void Read(XmlElement os)
        {
            this.Id              = os.GetInt("Id");
            this.Type            = (EMapTrigger)os.GetInt("Type");
            this.Active          = os.GetBool("Active");
            this.Relation1       = (ECR)os.GetInt("Relation1");
            this.Relation2       = (ECR)os.GetInt("Relation2");
            this.TriggerNum      = os.GetInt("TriggerNum");
            this.TriggerInterval = os.GetFloat("TriggerInterval");
            this.TriggerDelay    = os.GetFloat("TriggerDelay");

            foreach (XmlElement current in GetChilds(os))
            {
                switch (current.Name)
                {
                    case "Conditions1":
                        this.Conditions1.AddRange(ReadList<MapEventCondition>(current));
                        break;
                    case "Conditions2":
                        this.Conditions2.AddRange(ReadList<MapEventCondition>(current));
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Id",                 this.Id);
            DCFG.Write(doc, os, "Type",          (int)this.Type);
            DCFG.Write(doc, os, "Active",             this.Active);
            DCFG.Write(doc, os, "Relation1",     (int)this.Relation1);
            DCFG.Write(doc, os, "Relation2",     (int)this.Relation2);
            DCFG.Write(doc, os, "TriggerNum",         this.TriggerNum);
            DCFG.Write(doc, os, "TriggerInterval",    this.TriggerInterval);
            DCFG.Write(doc, os, "TriggerDelay",       this.TriggerDelay);
            DCFG.Write(doc, os, "Conditions1",        this.Conditions1);
            DCFG.Write(doc, os, "Conditions2",        this.Conditions2);
        }
    }
}

