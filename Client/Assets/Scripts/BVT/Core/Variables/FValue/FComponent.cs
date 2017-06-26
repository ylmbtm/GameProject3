using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FComponent : FValueObj<Component>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            V = (Component)UnityEditor.EditorGUILayout.ObjectField("", V, typeof(Component), true, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}
