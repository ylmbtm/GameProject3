using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    public class NodeFactory
    {
        public static Node     CreateNode(NodeTree tree, System.Type graphType)
        {
            GameObject child = new GameObject(graphType.ToString(), graphType);
            return (Node)child.GetComponent(graphType);
        }

        public static T        CreateFValueObject<T>(Transform root, string key, object v) where T : FValue
        {
            T c = root.gameObject.AddComponent<T>();
            c.hideFlags = HideFlags.HideInInspector;
            c.SetKey(key);
            c.SetValue(v);
            return c;
        }

        public static T        CreateFValueObject<T>(Transform root, string key) where T : FValue
        {
            T c = root.gameObject.AddComponent<T>();
            c.SetKey(key);
            return c;
        }

        public static T        CreateShareObject<T>(NodeBlackboard blackboard) where T : ShareValue
        {
            T v = System.Activator.CreateInstance<T>();
            v.SetBlackBoard(blackboard);
            return v;
        }
    }
}
