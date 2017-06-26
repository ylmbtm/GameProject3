using BVT;
using UnityEditor;
using UnityEngine;
using System.Collections.Generic;

namespace BVT
{
    [CustomEditor(typeof(NodeTree), true)]
    public class BVTGraphTreeInspector : Editor
    {
        public NodeTree          Tree { get { return target as NodeTree; } }

        private void OnEnable()
        {
            List<FValue> list = new List<FValue>();
            Tree.GetComponents<FValue>(list);
            for (int i = 0; i < list.Count; i++)
            {
                list[i].hideFlags = HideFlags.HideInInspector;
            }
            Tree.Blackboard.hideFlags = HideFlags.HideInInspector;
            Tree.NodesRoot.hideFlags = HideFlags.HideInHierarchy;
        }

        private bool IsPrefab()
        {
            return PrefabUtility.GetPrefabType(Tree) == PrefabType.Prefab;
        }

        public override void OnInspectorGUI()
        {
            if (IsPrefab())
            {
                return;
            }
            if (Tree.IsRunning)
            {
                return;
            }
            BTUndo.CheckUndo(this, "NodeTree Inspector");
            Tree.ID = EditorGUILayout.IntField("Tree ID", Tree.ID);
            Tree.Name = EditorGUILayout.TextField("Tree Name", Tree.Name);
            Tree.Loop = EditorGUILayout.Toggle("Tree Loop", Tree.Loop);
            EditorGUILayout.LabelField("Tree Desc");
            Tree.Desc = EditorGUILayout.TextArea(Tree.Desc, GUILayout.Height(50));


            GUI.backgroundColor = new Color(0.8f, 0.8f, 1);
            EditorGUILayout.BeginHorizontal();
            if (GUILayout.Button("Open Editor", GUILayout.Width(100), GUILayout.Height(30)))
            {
                BTTreeWindow.OpenWindow(Tree);
            }
            if (GUILayout.Button("Shut", GUILayout.MaxWidth(60), GUILayout.Height(30)))
            {
                BTTreeWindow.ShutWindow();
            }

            if (GUILayout.Button("Save", GUILayout.MaxWidth(60), GUILayout.Height(30)))
            {

            }
            if (GUILayout.Button("Load", GUILayout.MaxWidth(60), GUILayout.Height(30)))
            {

            }
            EditorGUILayout.EndHorizontal();
            GUI.backgroundColor = Color.black;
            GUILayout.Space(10);

            Tree.Blackboard.SetRoot(Tree.transform);
            Tree.Blackboard.DrawGUI();
            base.serializedObject.ApplyModifiedProperties();
        }
    }
}