using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    public class FTMutiPoint : FTComponent, IFTContainer
    {
        [FT] public List<FTPoint> Points = new List<FTPoint>();

        public override DCFG Export()
        {
            MapMutiPoint data = new MapMutiPoint();
            data.ID           = this.ID;
            for (int i = 0; i < Points.Count; i++)
            {
                MapPoint d = (MapPoint)Points[i].Export();
                data.Points.Add(d);
            }
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapMutiPoint data   = cfg as MapMutiPoint;
            this.ID             = data.ID;
            for (int i = 0; i < data.Points.Count; i++)
            {
                MapPoint d = data.Points[i];
                FTPoint p = new GameObject().AddComponent<FTPoint>();
                p.transform.parent = transform;
                p.Import(d);
                Points.Add(p);
            }
        }

        public override void OnDrawGizmos()
        {
            transform.localScale = Vector3.one;
            for (int i = 0; i < Points.Count; i++)
            {
                FTPoint p = Points[i];
                if (p == null)
                {
                    Points.RemoveAt(i);
                }
                else
                {
                    FTDraw.DrawGizmosSphere(p.transform, Color.red, 1);
                }
            }
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            GUILayout.Space(10);
            GUILayout.BeginHorizontal();
            GUI.color = Color.green;
            if (GUILayout.Button("添加元素", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                this.AddElement();
            }
            GUILayout.Space(10);
            GUI.color = Color.green;
            if (GUILayout.Button("快捷编辑", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                UnityEditor.EditorWindow.GetWindow<FTComponentWindow>().SetGroup(this);
                UnityEditor.EditorWindow.GetWindow<FTComponentWindow>().Show();
            }
            GUILayout.EndHorizontal();

            GUI.color = Color.red;
            GUILayout.Space(10);
            if (GUILayout.Button("所有目标贴近地面", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                List<FTPoint> list = Points;
                for (int i = 0; i < list.Count; i++)
                {
                    list[i].MoveToGround();
                }
            }
            GUILayout.Space(5);
#endif
        }

        public FTPoint       AddElement()
        {
            FTPoint t = new GameObject(typeof(FTPoint).Name).AddComponent<FTPoint>();
            t.transform.parent = transform;
            Points.Add(t);
            return t;
        }

        public          void AddElement(Vector3 pos, Vector3 eulerAngles)
        {
            FTPoint p = AddElement();
            p.transform.position = pos;
            p.transform.eulerAngles = eulerAngles;
        }
    }
}