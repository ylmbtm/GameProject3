using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace BVT
{
    public class FVector3 : FValueObj<Vector3>
    {
#if UNITY_EDITOR
        public override void DrawField()
        {
            V = UnityEditor.EditorGUILayout.Vector3Field("", V, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
        }
#endif
    }
}
