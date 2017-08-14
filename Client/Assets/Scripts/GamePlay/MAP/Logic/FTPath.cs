using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    public class FTPath : FTComponent
    {
        [FT] public EPathNodeType    Type         = EPathNodeType.Linear;
        [FT] public List<FTPathNode> PathNodes    = new List<FTPathNode>();
        [FT] public bool             PositionVary = true;
        [FT] public bool             RotationVary = true;

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
            MapPath data      = new MapPath();
            data.ID           = this.ID;
            data.Type         = (int)this.Type;
            data.PositionVary = this.PositionVary;
            data.RotationVary = this.RotationVary;
            for (int i = 0; i < PathNodes.Count; i++)
            {
                MapPathNode d = (MapPathNode)PathNodes[i].Export();
                data.PathNodes.Add(d);
            }
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapPath data      = cfg as MapPath;
            this.ID           = data.ID;
            this.Type         = (EPathNodeType)data.Type;
            this.PositionVary = data.PositionVary;
            this.RotationVary = data.RotationVary;
            for (int i = 0; i < data.PathNodes.Count; i++)
            {
                MapPathNode d = data.PathNodes[i];
                FTPathNode p = new GameObject().AddComponent<FTPathNode>();
                p.transform.parent = transform;
                p.Import(d);
            }
        }

        public          void UpdateActor(Transform actor, float time)
        {
            Vector3    p = Vector3.zero;
            Quaternion q = Quaternion.identity;
            Vector3    s = Vector3.one;
            switch(Type)
            {
                case EPathNodeType.Linear:
                    FTPathHelper.PathLinearInterp(time, PathNodes, out p, out q, out s);
                    break;
                case EPathNodeType.Bezier:
                    FTPathHelper.PathBezierInterp(time, PathNodes, out p, out q, out s);
                    break;
            }
            if (this.PositionVary)
            {
                actor.position = p;
            }
            if (this.RotationVary)
            {
                actor.rotation = q;
            }
        }

        public override void OnDrawGizmos()
        {
            transform.localScale = Vector3.one;
            FTDraw.DrawPathNodes(Type, PathNodes);
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID           = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            this.Type         = (EPathNodeType)UnityEditor.EditorGUILayout.EnumPopup("Type", this.Type);
            this.PositionVary = UnityEditor.EditorGUILayout.Toggle("PositionVary", this.PositionVary);
            this.RotationVary = UnityEditor.EditorGUILayout.Toggle("RotationVary", this.RotationVary);
            if (GUILayout.Button("添加路点"))
            {
                AddElement();
            }

            int index = -1;
            for (int i = 0; i < PathNodes.Count; i++)
            {
                FTPathNode node = PathNodes[i];
                if (node == null)
                {
                    continue;
                }
                GUILayout.BeginHorizontal();
                GUILayout.Label(node.name);
                GUILayout.Label(node.Pos.ToString());
                GUILayout.Label(node.Face.ToString());
                GUI.color = Color.cyan;
                if (GUILayout.Button("I"))
                {
                    InsElement(i);
                }
                GUI.color = Color.red;
                if (GUILayout.Button("X"))
                {
                    index = i;
                }
                GUI.color = Color.white;
                GUILayout.EndHorizontal();
            }
            if (index >= 0)
            {
                DelElement(index);
            }
            GUILayout.Space(5);
#endif
        }

        public FTPathNode AddElement()
        {
            FTPathNode t = new GameObject(typeof(FTPathNode).Name).AddComponent<FTPathNode>();
            t.transform.parent = transform;
            t.Init();
            if (PathNodes.Count >= 2)
            {
                FTPathNode p1 = PathNodes[PathNodes.Count - 1];
                FTPathNode p2 = PathNodes[PathNodes.Count - 2];
                t.Pos = (p1.Pos - p2.Pos) + p1.Pos;
            }
            PathNodes.Add(t);
            return t;
        }

        public FTPathNode DelElement(int index)
        {
            FTPathNode t = PathNodes[index];
            GameObject.DestroyImmediate(t.gameObject);
            PathNodes.RemoveAt(index);
            return null;
        }

        public FTPathNode InsElement(int index)
        {
            FTPathNode t = new GameObject(typeof(FTPathNode).Name).AddComponent<FTPathNode>();
            t.transform.parent = transform;
            t.Init();
            FTPathNode p1 = null;
            FTPathNode p2 = null;
            if (index > 0)
            {
                p1 = PathNodes[index - 1];
            }
            if (index <= PathNodes.Count - 1)
            {
                p2 = PathNodes[index];
            }
            if (p1 != null && p2 != null)
            {
                t.Pos = (p1.Pos + p2.Pos) * 0.5f;
            }
            PathNodes.Insert(index, t);
            return t;
        }

        public void       AddElement(Vector3 pos, Vector3 eulerAngles)
        {
            FTPathNode p = AddElement();
            p.transform.position = pos;
            p.transform.eulerAngles = eulerAngles;
        }
    }
}