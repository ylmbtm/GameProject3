using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    public class FInt : FValueObj<int>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            GUI.backgroundColor = new Color(0.7f, 1, 0.7f);
            V = UnityEditor.EditorGUILayout.IntField(V, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}

