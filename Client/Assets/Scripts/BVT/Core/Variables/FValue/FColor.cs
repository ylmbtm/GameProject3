using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FColor : FValueObj<Color>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            V = UnityEditor.EditorGUILayout.ColorField(V, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}
