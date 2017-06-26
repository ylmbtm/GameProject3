using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FFloat : FValueObj<float>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            GUI.backgroundColor = new Color(0.7f, 0.7f, 1);
            V = UnityEditor.EditorGUILayout.FloatField(V, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}
