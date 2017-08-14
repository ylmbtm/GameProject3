#if UNITY_EDITOR
using UnityEngine;
using System.Collections;
using System;
using System.Linq;


namespace MAP
{

    public class FTEditorHelper
    {
        public static void ShowMenu(Type type, UnityEditor.GenericMenu.MenuFunction2 menuFunction2)
        {
            UnityEditor.GenericMenu menu = new UnityEditor.GenericMenu();
            var types = AppDomain.CurrentDomain.GetAssemblies().SelectMany(item => item.GetTypes())
                   .Where(item => item.IsSubclassOf(type)).ToList();
            for (int i = 0; i < types.Count; i++)
            {
                Type t = types[i];
                menu.AddItem(new GUIContent(t.Name), false, menuFunction2, t);
            }
            menu.ShowAsContext();
        }
    }

}
#endif