using UnityEngine;
using System.Collections;
using UnityEditor;
using BVT;

namespace EDT
{
    [CustomEditor(typeof(CharacterView), true)]
    public class EditorCharacter : Editor
    {
        public SerializedProperty characterAttrProperty;

        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();
            CharacterView view = target as CharacterView;
            Character cc = view.Owner;
            if (cc == null)
            {
                return;
            }
            EditorGUILayout.TextField("ID",      cc.ID.ToString());
            EditorGUILayout.TextField("Type",    cc.Type.ToString());
            EditorGUILayout.TextField("GUID",    cc.GUID.ToString());
            EditorGUILayout.TextField("Camp",    cc.Camp.ToString());
            EditorGUILayout.TextField("FSM",     cc.FSM.ToString());
            EditorGUILayout.TextField("AIState", cc.AIState.ToString());

            EditorGUILayout.ObjectField("Target", cc.Target == null ? null : cc.Target.View, typeof(CharacterView), true);
            EditorGUILayout.ObjectField("Obj", cc.Obj, typeof(CharacterView), true);
            EditorGUILayout.ObjectField("ObjTrans", cc.Obj, typeof(CharacterView), true);
            EditorGUILayout.ObjectField("AI", cc.AI, typeof(NodeTree), true);

            if (cc.CurrAttr != null) view.Attr = cc.CurrAttr;
        }
    }
}

