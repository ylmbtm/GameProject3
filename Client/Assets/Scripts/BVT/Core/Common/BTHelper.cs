using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using BVT;
using System.Linq;

namespace BVT
{
#if UNITY_EDITOR
    public class BTHelper
    {
        public static Color     LIGHT_ORANGE = new Color(1, 0.9f, 0.4f);
		public static Color     LIGHT_BLUE   = new Color(0.8f,0.8f,1);
		public static Color     LIGHT_RED    = new Color(1,0.5f,0.5f, 0.8f);
        public static Texture2D TEX          = new Texture2D(1, 1);
        static GUISkin          mBVTSkin;

        public static GUISkin BVTSkin
        {
            get
            {
                if (mBVTSkin == null)
                {
                    mBVTSkin = UnityEditor.AssetDatabase.LoadAssetAtPath<GUISkin>("Assets/Scripts/BVT/Editor/Resource/Skin/BVTSkin.guiskin");
                }
                return mBVTSkin;
            }
        }

        public static void DrawCoolLabel(string text)
        {
            GUI.skin.label.richText = true;
            GUI.color = LIGHT_ORANGE;
            GUILayout.Label("<b><size=16>" + text + "</size></b>");
            GUI.color = Color.white;
        }

        public static void DrawCoolTitle(string text)
        {
            GUILayout.Space(5);
            GUI.skin.label.richText = true;
            GUI.color = LIGHT_ORANGE;
            GUILayout.Label("<b><size=16>" + text + "</size></b>");
            GUI.color = Color.white;
            GUILayout.Space(2);
        }

        public static void DrawSeparator()
        {
            GUI.backgroundColor = Color.black;
            GUILayout.Box("", GUILayout.MaxWidth(Screen.width), GUILayout.Height(2));
            GUI.backgroundColor = Color.white;
        }

        public static void DrawBoldSeparator()
        {
            Rect lastRect = GUILayoutUtility.GetLastRect();
            GUILayout.Space(14);
            GUI.color = new Color(0, 0, 0, 0.25f);
            GUI.DrawTexture(new Rect(0, lastRect.yMax + 6, Screen.width, 4), TEX);
            GUI.DrawTexture(new Rect(0, lastRect.yMax + 6, Screen.width, 1), TEX);
            GUI.DrawTexture(new Rect(0, lastRect.yMax + 9, Screen.width, 1), TEX);
            GUI.color = Color.white;
        }

        public static void DrawEndOfInspector()
        {
            Rect lastRect = GUILayoutUtility.GetLastRect();
            GUILayout.Space(8);
            GUI.color = new Color(0, 0, 0, 0.4f);
            GUI.DrawTexture(new Rect(0, lastRect.yMax + 6, Screen.width, 4), TEX);
            GUI.DrawTexture(new Rect(0, lastRect.yMax + 4, Screen.width, 1), TEX);
            GUI.color = Color.white;
        }

        public static void DrawTitledSeparator(string title, bool startOfInspector)
        {
            if (!startOfInspector)
                DrawBoldSeparator();
            else
                UnityEditor.EditorGUILayout.Space();
            DrawCoolLabel(title + " ▼");
            DrawSeparator();
        }

        public static void ShowComponentsSelectionMenu(Type baseType, Action<Type> callback)
        {
            UnityEditor.GenericMenu.MenuFunction2 select = (selectType) => { callback((Type)selectType); };
            UnityEditor.GenericMenu menu = new UnityEditor.GenericMenu();
            List<NodeAttribute> scriptList = new List<NodeAttribute>();
            Type [] subTypes = System.Reflection.Assembly.GetExecutingAssembly().GetTypes();
            for (int i = 0; i < subTypes.Length; i++)
            {
                Type type = subTypes[i];
                if (IsSubClassOf(type, baseType) && type.IsDefined(typeof(NodeAttribute), false))
                {
                    NodeAttribute[] attrs = (NodeAttribute[])type.GetCustomAttributes(typeof(NodeAttribute), false);
                    if (attrs != null && attrs.Length > 0)
                    {
                        NodeAttribute attr = attrs[0];
                        attr.NodeType = type;
                        scriptList.Add(attr);
                    }
                }
            }
            for (int i = 0; i < scriptList.Count; i++)
            {
                string label = scriptList[i].Label;
                string type  = scriptList[i].Type;
                menu.AddItem(new GUIContent(type + "/" + label), false, select, scriptList[i].NodeType);
            }
            menu.ShowAsContext();
        }

        public static bool IsSubClassOf(Type type, Type baseType)
        {
            var b = type.BaseType;
            while (b != null)
            {
                if (b.Equals(baseType))
                {
                    return true;
                }
                b = b.BaseType;
            }
            return false;
        }

        public static TAtr GetAttribute<TAtr>(Type type) where TAtr : Attribute
        {
            TAtr[] attrs = (TAtr[])type.GetCustomAttributes(typeof(TAtr), false);
            if (attrs != null && attrs.Length > 0)
            {
                TAtr attr = attrs[0];
                return attr;
            }
            return null;
        }

        public static T CreateAsset<T>(bool displayFilePanel) where T : ScriptableObject
        {
            return (T)CreateAsset(typeof(T), displayFilePanel);
        }

        public static T CreatePrefab<T>(bool displayFilePanel) where T : MonoBehaviour
        {
            T asset = null;
            Type type = typeof(T);
            var path = UnityEditor.EditorUtility.SaveFilePanelInProject(
                        "Create Asset of type " + type.ToString(),
                           type.Name, "prefab", "");
            asset = new GameObject(type.Name).AddComponent<T>();
            UnityEngine.Object prefab = UnityEditor.PrefabUtility.CreateEmptyPrefab(path);
            UnityEditor.PrefabUtility.ReplacePrefab(asset.gameObject, prefab, UnityEditor.ReplacePrefabOptions.ConnectToPrefab);
            UnityEditor.AssetDatabase.SaveAssets();
            return asset;
        }

        public static ScriptableObject CreateAsset(System.Type type, bool displayFilePanel)
        {
            ScriptableObject asset = null;
            var path = UnityEditor.EditorUtility.SaveFilePanelInProject(
                        "Create Asset of type " + type.ToString(),
                           type.Name + ".asset",
                        "asset", "");
            asset = CreateAsset(type, path);
            return asset;
        }

        public static ScriptableObject CreateAsset(System.Type type)
        {
            var asset = CreateAsset(type, GetAssetUniquePath(type.Name + ".asset"));
            return asset;
        }

        public static ScriptableObject CreateAsset(System.Type type,string path)
        {
            if(string.IsNullOrEmpty(path))
            {
                return null;
            }
            ScriptableObject data = null;
            data = ScriptableObject.CreateInstance(type);
            UnityEditor.AssetDatabase.CreateAsset(data, path);
            UnityEditor.AssetDatabase.SaveAssets();
            return data;
        }

        public static string GetAssetUniquePath(string fileName)
        {
            var path = UnityEditor.AssetDatabase.GetAssetPath(UnityEditor.Selection.activeObject);
            if (path == "")
            {
                path = "Assets";
            }
            if (System.IO.Path.GetExtension(path) != "")
                path = path.Replace(System.IO.Path.GetFileName(UnityEditor.AssetDatabase.GetAssetPath(UnityEditor.Selection.activeObject)), "");
            return UnityEditor.AssetDatabase.GenerateUniqueAssetPath(path + "/" + fileName);
        }

        public static ScriptableObject AddScriptableComponent(GameObject target, System.Type type)
        {
            return null;
        }
    }
#endif
}

