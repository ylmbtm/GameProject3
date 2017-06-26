using UnityEngine;
using System.Collections.Generic;
using BVT;

namespace BVT
{
    public class NodeBlackboard : MonoBehaviour, IBlackboard
    {
        [SerializeField][HideInInspector]
        public List<FValue>                    Data = new List<FValue>();

        private Transform                      mRoot;
        private GameObject                     mRootObj;

        public bool AddData(string key, object value)
        {
            if (string.IsNullOrEmpty(key))
            {
                return false;
            }
            if (value == null)
            {
                return false;
            }
            FValue v = null;
            if (value.GetType() == typeof(bool))
            {
                v = NodeFactory.CreateFValueObject<FBool>(transform, key, value);
            }
            if (value.GetType() == typeof(int))
            {
                v = NodeFactory.CreateFValueObject<FInt>(transform, key, value);
            }
            if (value.GetType() == typeof(Color))
            {
                v = NodeFactory.CreateFValueObject<FColor>(transform, key, value);
            }
            if (value.GetType() == typeof(float))
            {
                v = NodeFactory.CreateFValueObject<FFloat>(transform, key, value);
            }
            if (value.GetType() == typeof(GameObject))
            {
                v = NodeFactory.CreateFValueObject<FGameObject>(transform, key, value);
            }
            if (value.GetType() == typeof(List<GameObject>))
            {
                v = NodeFactory.CreateFValueObject<FGameObjectList>(transform, key, value);
            }
            if (value.GetType() == typeof(Component))
            {
                v = NodeFactory.CreateFValueObject<FComponent>(transform, key, value);
            }
            if (value.GetType() == typeof(string))
            {
                v = NodeFactory.CreateFValueObject<FString>(transform, key, value);
            }
            if (value.GetType() == typeof(Vector3))
            {
                v = NodeFactory.CreateFValueObject<FVector3>(transform, key, value);
            }
            if (value.GetType() == typeof(Vector2))
            {
                v = NodeFactory.CreateFValueObject<FVector2>(transform, key, value);
            }
            if(value.GetType() == typeof(Transform))
            {
                v = NodeFactory.CreateFValueObject<FTransform>(transform, key, value);
            }
            if (v != null)
            {
                Data.Add(v);
                return true;
            }
            else
            {
                return false;
            }
        }

        public void DelData(string key)
        {
            for(int i=0;i<Data.Count;i++)
            {
                if(Data[i].GetKey()==key)
                {
                    FValue v = Data[i];
                    Data.RemoveAt(i);
                    UnityEngine.Object.DestroyImmediate(v);
                    break;
                }
            }
        }

        public TVal GetData<TVal>(string key) where TVal : FValue
        {
            for (int i = 0; i < Data.Count; i++)
            {
                if (Data[i].GetKey() == key)
                {
                    FValue s = Data[i];
                    if (s != null)
                    {
                        if (s is TVal)
                        {
                            return (TVal)(s != null ? s : null);
                        }
                    }
                    break;
                }
            }
            return default(TVal);
        }

        public void GetData<TVal>(ref TVal v) where TVal : FValue, new()
        {
            if (v != null)
            {
                if (!string.IsNullOrEmpty(v.GetKey()))
                {
                    FValue s = GetData<FValue>(v.GetKey());
                    v = s != null ? (TVal)s : v;
                }
            }
            else
            {
                v = NodeFactory.CreateFValueObject<TVal>(transform, string.Empty);
            }
        }

        public void SetData(string key, object value)
        {
            for (int i = 0; i < Data.Count; i++)
            {
                if (Data[i].GetKey() == key)
                {
                    Data[i].SetValue(value);
                }
            }
        }

        public void SetRoot(Transform root)
        {
            this.mRoot = root;
            this.mRootObj = this.mRoot == null ? null : this.mRoot.gameObject;
        }

        public bool ContainsKey(string key)
        {
            for (int i = 0; i < Data.Count; i++)
            {
                if (Data[i].GetKey() == key)
                {
                    return true;
                }
            }
            return false;
        }

        public void ClearNullData()
        {
            List<int> list = new List<int>();
            for (int i = 0; i < Data.Count; i++)
            {
                if (Data[i] == null)
                {
                    list.Add(i);
                }
            }
            for (int i = 0; i < list.Count; i++)
            {
                Data.RemoveAt(list[i]);
            }
        }

#if UNITY_EDITOR
        public void DrawGUI()
        {
            BTUndo.CheckUndo(this, "NodeBlackboard Inspector");
            GUIStyle titleStyle = new GUIStyle("label");
            titleStyle.alignment = TextAnchor.MiddleCenter;
            GUILayout.Label("Tree Blackboard", titleStyle);
            GUI.backgroundColor = new Color(0.8f, 0.8f, 1);
            if (GUILayout.Button("AddData", GUILayout.ExpandWidth(true), GUILayout.Height(30)))
            {
                var menu = new UnityEditor.GenericMenu();
                menu.AddItem(new GUIContent("bool"),             false, ContextNew, false);
                menu.AddItem(new GUIContent("float"),            false, ContextNew, 0.2f);
                menu.AddItem(new GUIContent("int"),              false, ContextNew, 1);
                menu.AddItem(new GUIContent("string"),           false, ContextNew, "some string");
                menu.AddItem(new GUIContent("Vector2"),          false, ContextNew, Vector2.zero);
                menu.AddItem(new GUIContent("Vector3"),          false, ContextNew, Vector3.zero);
                menu.AddItem(new GUIContent("Color"),            false, ContextNew, Color.white);
                menu.AddItem(new GUIContent("GameObject"),       false, ContextNew, mRootObj);
                menu.AddItem(new GUIContent("List<GameObject>"), false, ContextNew, new List<GameObject>());
                menu.AddItem(new GUIContent("Component"),        false, ContextNew, mRootObj.transform);
                menu.AddItem(new GUIContent("Transform"),        false, ContextNew, mRootObj.transform);
                menu.ShowAsContext();
            }
            GUILayout.BeginHorizontal();
            GUI.backgroundColor = Color.yellow;
            GUILayout.Label("Key",   GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
            GUILayout.Label("Value", GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
            GUI.backgroundColor = Color.white;
            GUILayout.EndHorizontal();

            for (int i = 0; i < Data.Count; i++)
            {
                FValue v = Data[i];
                if (v != null)
                {
                    GUILayout.BeginHorizontal();
                    v.DrawGUI();
                    GUI.backgroundColor = new Color(1f, 0.6f, 0.6f);
                    if (GUILayout.Button("X", GUILayout.MaxWidth(20)))
                    {
                        DelData(v.GetKey());
                    }
                    GUI.backgroundColor = new Color(0.7f, 0.7f, 0.7f);
                    GUILayout.EndHorizontal();
                }
            }

            GUI.backgroundColor = new Color(0.8f, 0.8f, 1);
            if (GUILayout.Button("ClearNullData", GUILayout.ExpandWidth(true), GUILayout.Height(30)))
            {
                ClearNullData();
            }
        }

        public void ContextNew(object value)
        {
            string v = value.GetType().Name;
            while (ContainsKey(v))
            {
                v = value.GetType().Name + UnityEngine.Random.Range(0, 1000);
            }
            AddData(v, value);
        }
#endif
    }
}