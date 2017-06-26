using UnityEngine;
using System.Collections;

namespace BVT
{
#if UNITY_EDITOR
    public static class BTUndo
    {
        public static void CheckUndo(Object target, string name)
        {
            if(Application.isPlaying || target ==null)
            {
                return;
            }
            Event e = Event.current;
            if (((e.type == EventType.MouseDown || e.type == EventType.MouseUp) && e.button == 0) || (e.type == EventType.KeyUp))
            {
                UnityEditor.Undo.RecordObject(target, name);
            }
        }

        public static void CheckDirty(Object target)
        {
            if (Application.isPlaying || target == null)
            {
                return;
            }
            if(GUI.changed)
            {
                UnityEditor.EditorUtility.SetDirty(target);
            }
        }
    }
#endif
}

