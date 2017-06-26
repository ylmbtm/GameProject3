using UnityEngine;
using System.Collections;
using UnityEditor;
using System.Xml;
using System.IO;
using ACT;
using System.Reflection;

namespace ACT.EDT
{
    [CustomEditor(typeof(DctSkillContainer))]
    public class DctActSkillContainerInspector : Editor
    {
        public DctSkillContainer Director
        {
            get { return target as DctSkillContainer; }
        }

        public override void OnInspectorGUI()
        {
            SerializedProperty containerProperty = serializedObject.FindProperty("Container");
            EditorGUILayout.PropertyField(containerProperty, new GUIContent("Container"), true);
            base.serializedObject.ApplyModifiedProperties();

            EditorGUILayout.BeginHorizontal();
            if (GUILayout.Button("Save", GUILayout.Width(80), GUILayout.Height(40), GUILayout.ExpandWidth(true)))
            {
                XmlDocument xmlDoc = new XmlDocument();
                string file = Application.dataPath + "/Resources/Text/Skill/" + Director.Container.ActorID.ToString() + ".xml";
                FileStream fs = File.Create(file);
                XmlElement root = xmlDoc.CreateElement("Root");
                xmlDoc.AppendChild(root);
                Director.Container.SaveDoc(xmlDoc, root);
                fs.Close();
                fs.Dispose();
                xmlDoc.Save(file);
            }

            GUI.color = Color.cyan;
            if (GUILayout.Button("Load", GUILayout.Width(80), GUILayout.Height(40), GUILayout.ExpandWidth(true)))
            {
                Director.Container.LoadDoc();
            }
            EditorGUILayout.EndHorizontal();
        }
    }
}

