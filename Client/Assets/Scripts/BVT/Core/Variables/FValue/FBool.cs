using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FBool : FValueObj<bool>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            V = UnityEditor.EditorGUILayout.Toggle(V, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}
