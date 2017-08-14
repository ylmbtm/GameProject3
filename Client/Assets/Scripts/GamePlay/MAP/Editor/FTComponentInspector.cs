using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

namespace MAP
{
    [CustomEditor(typeof(FTComponent), true)]
    [CanEditMultipleObjects]
    public class FTComponentInspector : Editor
    {
        public override void OnInspectorGUI()
        {
            FTComponent c = target as FTComponent;
            c.OnDrawInspector();
        }
    }
}