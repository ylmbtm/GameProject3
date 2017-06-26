using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using ACT;
using System.Xml;
using System;
using System.Reflection;

namespace ACT
{
    [Serializable]
    public class ActSkillSystem
    {
        [SerializeField]
        public int            ActorID;
        [SerializeField]
        public List<ActSkill> Skills = new List<ActSkill>();

        public void SaveDoc(XmlDocument doc, XmlElement root)
        {
            root.SetAttribute("ActorID", this.ActorID.ToString());
            for (int i = 0; i < Skills.Count; i++)
            {
                XmlElement child = doc.CreateElement("Skill");
                root.AppendChild(child);
                Skills[i].SaveDoc(doc, child);
            }
        }

        public void LoadDoc()
        {
            TextAsset asset = GTResourceManager.Instance.Load<TextAsset>("Text/Skill/" + ActorID);
            if (asset == null)
            {
                return;
            }
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(asset.text);
            XmlElement root = doc.FirstChild as XmlElement;
            ActorID = root.GetAttribute("ActorID").ToInt32();
            XmlElement child = root.FirstChild as XmlElement;
            while (child != null)
            {
                ActSkill skill = new ActSkill();
                skill.LoadDoc(child);
                Skills.Add(skill);
                child = child.NextSibling as XmlElement;
            }
        }
    }
}

