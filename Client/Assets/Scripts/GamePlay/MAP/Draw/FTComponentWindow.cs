#if UNITY_EDITOR

using UnityEngine;
using System.Collections;
using UnityEditor;
using System;

namespace MAP
{
    public class FTComponentWindow : EditorWindow
    {
        private SceneView.OnSceneFunc m_SceneGUIDelegate = null;
        private IFTContainer          m_FTContainer      = null;

        void OnEnable()
        {
            m_SceneGUIDelegate = new SceneView.OnSceneFunc(OnSceneGUIDelegate);
            SceneView.onSceneGUIDelegate += m_SceneGUIDelegate;
        }

        void OnDisable()
        {
            SceneView.onSceneGUIDelegate -= m_SceneGUIDelegate;
            m_FTContainer = null;
        }

        void OnDestroy()
        {
            SceneView.onSceneGUIDelegate -= m_SceneGUIDelegate;
            m_FTContainer = null;
        }

        void OnSceneGUIDelegate(SceneView sceneView)
        {
            if(m_FTContainer == null)
            {
                Close();
                return;
            }
            HandleUtility.AddDefaultControl(GUIUtility.GetControlID(FocusType.Passive));
            Event e = Event.current;
            if (e.button == 0 && e.type == EventType.MouseDown)
            {
                if (e.modifiers == EventModifiers.Shift)
                {
                    Ray ray = HandleUtility.GUIPointToWorldRay(e.mousePosition);
                    RaycastHit hit;
                    if (Physics.Raycast(ray, out hit, Mathf.Infinity, LayerMask.GetMask("Default")))
                    {
                        m_FTContainer.AddElement(hit.point, Vector3.zero);
                    }
                }
            }
        }

        public void SetGroup(IFTContainer fTContainer)
        {
            this.m_FTContainer = fTContainer;
        }
    }
}
#endif