using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FGameObject : FValueObj<GameObject>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            V = (GameObject)UnityEditor.EditorGUILayout.ObjectField("", V, typeof(GameObject), true, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}
