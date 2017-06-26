using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FString : FValueObj<string>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            GUI.backgroundColor = new Color(0.5f, 0.5f, 0.5f);
            V = UnityEditor.EditorGUILayout.TextField(V, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}