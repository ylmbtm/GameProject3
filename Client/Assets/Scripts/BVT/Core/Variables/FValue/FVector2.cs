using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FVector2 : FValueObj<Vector2>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {

            V = UnityEditor.EditorGUILayout.Vector2Field("", V, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}
