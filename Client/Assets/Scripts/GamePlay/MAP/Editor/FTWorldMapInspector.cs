using UnityEngine;
using System.Collections;
using UnityEditor;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    [CustomEditor(typeof(FTWorldMap), true)]
    public class FTWorldMapInspector : Editor
    {
        public override void OnInspectorGUI()
        {
            GUILayout.Space(10);
            FTWorldMap worldMap = target as FTWorldMap;
            base.OnInspectorGUI();
            GUILayout.Space(10);
            GUI.color = Color.red;
            GUILayout.BeginHorizontal();
            if (GUILayout.Button("导入", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                worldMap.Import();
            }

            GUI.color = Color.green;
            if (GUILayout.Button("导出", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                worldMap.Export();
            }
            GUILayout.EndHorizontal();
            GUILayout.Space(10);
        }
    }
}