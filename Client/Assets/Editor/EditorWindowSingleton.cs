using UnityEngine;
using System.Collections;
using UnityEditor;

namespace EDT
{
    public class EditorWindowSingleton<T> : EditorWindow where T : EditorWindow
    {
        static T instance = null;

        public static T GetInstance()
        {
            if (instance == null)
            {
                instance = EditorWindow.GetWindow<T>() as T;
            }
            return instance;
        }
    }
}

