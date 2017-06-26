using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FTransform : FValueObj<Transform>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            GUI.backgroundColor = new Color(0.5f, 0.5f, 0.5f);
            V = (Transform)UnityEditor.EditorGUILayout.ObjectField("", V, typeof(Transform), true, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}

