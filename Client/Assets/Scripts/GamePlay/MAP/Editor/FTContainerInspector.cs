using UnityEngine;
using System.Collections;
using UnityEditor;

namespace MAP
{
    [CustomEditor(typeof(FTContainer), true)]
    public class FTContainerInspector : Editor
    {
        public override void OnInspectorGUI()
        {
            FTContainer group = target as FTContainer;
            group.transform.localPosition = Vector3.zero;
            group.transform.localRotation = Quaternion.identity;
            group.transform.localScale = Vector3.one;
            group.OnDrawInspector();
        }
    }
}