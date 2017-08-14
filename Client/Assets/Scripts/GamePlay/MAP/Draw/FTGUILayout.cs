using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace MAP
{
    public class FTGUILayout
    {
        public static void DrawEvents(FTWorldMap map, List<FTEvent> events)
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            GUI.color = Color.green;
            if (GUILayout.Button("添加事件", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                FTEditorHelper.ShowMenu(typeof(FTEvent), (obj) =>
                {
                    FTEvent e = map.GetGroup<FTGroupEvent>().AddEvent(obj as Type);
                    events.Add(e);
                });
            }
            for (int i = 0; i < events.Count; i++)
            {
                GUILayout.Label(events[i].name);
            }
            GUILayout.Space(10);
            GUI.color = Color.white;
#endif
        }
    }
}