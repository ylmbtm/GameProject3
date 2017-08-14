using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    public class FTAreaMonster : FTComponent, IFTContainer
    {
        [FT] public Int32         MonsterID    = 0;
        [FT] public float         RebornCD     = 20;
        [FT] public Int32         RebornType   = 0;
        [FT] public Int32         PatrolType   = 0;
        [FT] public Int32         PatrolTypeID = 0;
        [FT] public List<FTPoint> Points       = new List<FTPoint>();

        public override void Startup()
        {

        }

        public override void Trigger()
        {

        }

        public override void Release()
        {

        }

        public override DCFG Export()
        {
            MapAreaMonster data = new MapAreaMonster();
            data.ID             = this.ID;
            data.Pos            = transform.position;
            data.MonsterID      = this.MonsterID;
            data.RebornCD       = this.RebornCD;
            data.RebornType     = this.RebornType;
            data.PatrolType     = this.PatrolType;
            data.PatrolTypeID   = this.PatrolTypeID;
            for (int i = 0; i < Points.Count; i++)
            {
                MapPoint d = (MapPoint)Points[i].Export();
                data.Points.Add(d);
            }
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapAreaMonster data = cfg as MapAreaMonster;
            this.ID             = data.ID;
            this.Pos            = data.Pos;
            this.MonsterID      = data.MonsterID;
            this.RebornCD       = data.RebornCD;
            this.RebornType     = data.RebornType;
            this.PatrolType     = data.PatrolType;
            this.PatrolTypeID   = data.PatrolTypeID;
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
                    FTDraw.DrawGizmosCapsule(p.transform.position + new Vector3(0, 0.5f, 0), p.transform.rotation, Vector3.one * 0.5f, Color.red);
                }
            }
            FTDraw.DrawCircle(transform.position, 10, new Color(1, 0, 0, 0.1f));
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID           = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            this.MonsterID    = UnityEditor.EditorGUILayout.IntField("MonsterID", this.MonsterID);
            this.RebornCD     = UnityEditor.EditorGUILayout.FloatField("RebornCD", this.RebornCD);
            this.RebornType   = UnityEditor.EditorGUILayout.IntField("RebornType", this.RebornType);
            this.PatrolType   = UnityEditor.EditorGUILayout.IntField("PatrolType", this.PatrolType);
            this.PatrolTypeID = UnityEditor.EditorGUILayout.IntField("PatrolTypeID", this.PatrolTypeID);

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