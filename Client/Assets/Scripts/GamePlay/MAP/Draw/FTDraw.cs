using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using MAP;
using System.Reflection;

namespace MAP
{
    public class FTDraw
    {
        static Mesh         m_Cylinder = null;
        static Mesh         m_Capsule = null;
        static GUIContent[] m_SelectLabelIcons;
        static GUIContent[] m_SelectLargeIcons;

        static Mesh Cylinder
        {
            get
            {
                if(m_Cylinder == null)
                {
                    GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
                    go.hideFlags = HideFlags.HideAndDontSave;
                    go.SetActive(false);
                    m_Cylinder = go.GetComponent<MeshFilter>().sharedMesh;
                }
                return m_Cylinder;
            }
        }

        static Mesh Capsule
        {
            get
            {
                if (m_Capsule == null)
                {
                    GameObject go = GameObject.CreatePrimitive(PrimitiveType.Capsule);
                    go.hideFlags = HideFlags.HideAndDontSave;
                    go.SetActive(false);
                    m_Capsule = go.GetComponent<MeshFilter>().sharedMesh;
                }
                return m_Capsule;
            }
        }

        public static void DrawGizmosCube(Transform transform, Color color, Vector3 size)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = transform.localToWorldMatrix;
            Gizmos.color = color;
            Gizmos.DrawCube(Vector3.zero, size);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawGizmosSphere(Transform transform, Color color, float size)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = transform.localToWorldMatrix;
            Gizmos.color = color;
            Gizmos.DrawSphere(Vector3.zero, size);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawGizmosCylinder(Transform transform, Color color)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = transform.localToWorldMatrix;
            Gizmos.color = color;
            Gizmos.DrawMesh(Cylinder);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawGizmosCapsule(Transform transform, Color color)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = transform.localToWorldMatrix;
            Gizmos.color = color;
            Gizmos.DrawMesh(Capsule);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawGizmosCube(Vector3 pos, Quaternion q,Vector3 s, Color color, Vector3 size)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = Matrix4x4.TRS(pos, q, s);
            Gizmos.color = color;
            Gizmos.DrawCube(Vector3.zero, size);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawGizmosSphere(Vector3 pos, Quaternion q, Vector3 s, Color color, float size)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = Matrix4x4.TRS(pos, q, s);
            Gizmos.color = color;
            Gizmos.DrawSphere(Vector3.zero, size);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawGizmosCylinder(Vector3 pos, Quaternion q, Vector3 s, Color color)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = Matrix4x4.TRS(pos, q, s);
            Gizmos.color = color;
            Gizmos.DrawMesh(Cylinder);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawGizmosCapsule(Vector3 pos, Quaternion q, Vector3 s, Color color)
        {
            Matrix4x4 defaultMatrix4x4 = Gizmos.matrix;
            Gizmos.matrix = Matrix4x4.TRS(pos, q, s);
            Gizmos.color = color;
            Gizmos.DrawMesh(Capsule);
            Gizmos.matrix = defaultMatrix4x4;
        }

        public static void DrawCircle(Vector3 pos, float radius, Color color)
        {
#if UNITY_EDITOR
            UnityEditor.Handles.color = color;
            UnityEditor.Handles.DrawSolidArc(pos, Vector3.up, Vector3.forward, 360, radius);
#endif
        }

        public static void DrawCylinder(Vector3 pos, Color color, float size)
        {
#if UNITY_EDITOR
            UnityEditor.Handles.color = color;
            UnityEditor.Handles.CylinderHandleCap(0, pos, Quaternion.Euler(-90, 0, 0), size, EventType.Repaint);
#endif
        }

        public static void DrawArc(Vector3 pos, Vector3 normal, Vector3 from, Color color, float angle, float radius)
        {
#if UNITY_EDITOR
            UnityEditor.Handles.color = color;
            UnityEditor.Handles.DrawSolidArc(pos, normal, from, angle, radius);
#endif
        }

        public static void DrawCone(Vector3 pos, Color color, float size)
        {
#if UNITY_EDITOR
            UnityEditor.Handles.color = color;
            UnityEditor.Handles.ConeHandleCap(0, pos, Quaternion.Euler(new Vector3(-90, 0, 0)), size, EventType.Repaint);
#endif
        }

        public static void DrawSphere(Vector3 pos, Color color, float size)
        {
#if UNITY_EDITOR
            UnityEditor.Handles.color = color;
            UnityEditor.Handles.SphereHandleCap(0, pos, Quaternion.Euler(new Vector3(-90, 0, 0)), size, EventType.Repaint);
#endif
        }

        public static void DrawCube(Vector3 pos, Color color, float size)
        {
#if UNITY_EDITOR
            UnityEditor.Handles.color = color;
            UnityEditor.Handles.CubeHandleCap(0, pos, Quaternion.Euler(new Vector3(-90, 0, 0)), size, EventType.Repaint);
#endif
        }

        public static void DrawPathNodes(EPathNodeType type, List<FTPathNode> pathNodes)
        {
#if UNITY_EDITOR
            if (pathNodes.Count < 2)
            {
                return;
            }
            UnityEditor.Handles.color = Color.green;
            for (int i = 0; i < pathNodes.Count; i++)
            {
                FTPathNode n1 = pathNodes[i];
                if (n1 == null)
                {
                    pathNodes.RemoveAt(i);
                    break;
                }
                n1.name = string.Format("P {0}", i);
                FTDraw.DrawGizmosSphere(n1.transform, Color.green, 0.5f);
                FTDraw.DrawLabelIcon(n1.gameObject, 6);
                if (i < pathNodes.Count - 1)
                {
                    FTPathNode n2 = pathNodes[i + 1];
                    if (n2 == null)
                    {
                        continue;
                    }
                    switch (type)
                    {
                        case EPathNodeType.Linear:
                            UnityEditor.Handles.DrawBezier(n1.transform.position, n2.transform.position, n1.transform.position, n2.transform.position, Color.green, null, 2);
                            break;
                        case EPathNodeType.Bezier:
                            UnityEditor.Handles.DrawBezier(n1.transform.position, n2.transform.position, n1.TangentOut, n2.TangentIn, Color.green, null, 2);
                            break;
                    }
                }
            }
#endif
        }

        public static void IconContents(out GUIContent[] guiContentArray, string baseName, string postFix, int startIndex,int count)
        {
#if UNITY_EDITOR
            guiContentArray = new GUIContent[count];
            for (int index = 0; index < count; ++index)
            {
                var mi = UnityEditor.EditorGUIUtility.IconContent(baseName + (object)(startIndex + index) + postFix);
                guiContentArray[index] = mi;
            }
#else
            guiContentArray = null;
#endif
        }

        public static void DrawIcon(GameObject obj, Texture2D icon)
        {
#if UNITY_EDITOR
            var ty = typeof(UnityEditor.EditorGUIUtility);
            var mi = ty.GetMethod("SetIconForObject", BindingFlags.NonPublic | BindingFlags.Static);
            mi.Invoke(null, new object[] { obj, icon });
#endif
        }

        public static void DrawLabelIcon(GameObject obj, int index)
        {
            if (m_SelectLabelIcons == null)
            {
                IconContents(out m_SelectLabelIcons, "sv_label_", string.Empty, 0, 8);
            }
            DrawIcon(obj, m_SelectLabelIcons[index].image as Texture2D);
        }

        public static void DrawLargeIcon(GameObject obj, int index)
        {
            if (m_SelectLargeIcons == null)
            {
                IconContents(out m_SelectLargeIcons, "sv_icon_dot", "_pix16_gizmo", 0, 16);
            }
            DrawIcon(obj, m_SelectLargeIcons[index].image as Texture2D);
        }
    }
}

