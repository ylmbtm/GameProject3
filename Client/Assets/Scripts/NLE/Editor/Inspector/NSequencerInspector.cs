using UnityEngine;
using System.Collections;
using UnityEditor;
using NLE;
using System.Reflection;

namespace NLE.EDT
{
    [CustomEditor(typeof(NSequencer),true)]
    public class NSequencerInspector : Editor
    {
        public NSequencer Sequencer
        {
            get { return target as NSequencer; }
        }

        public override void OnInspectorGUI()
        {
            if (GUILayout.Button("Open Director", GUILayout.Width(200), GUILayout.Height(50), GUILayout.ExpandWidth(true)))
            {
                NSequencerWindow window = EditorWindow.GetWindow<NSequencerWindow>();
                window.OnFocusSequencer(target as NSequencer);
            }
            FieldInfo[] fields = Sequencer.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo fieldInfo = fields[i];
                if (fieldInfo.IsDefined(typeof(SerializeField), true))
                {
                    SerializedProperty p = serializedObject.FindProperty(fieldInfo.Name);
                    EditorGUILayout.PropertyField(p, new GUIContent(fieldInfo.Name));
                }
            }
            base.serializedObject.ApplyModifiedProperties();
        }
    }
}