using UnityEngine;
using System.Collections;
using CFG;
using System.Collections.Generic;

namespace MAP
{
    public class FTArea : FTComponent
    {
        [FT] public string        Name       = string.Empty;
        [FT] public EAreaShape    Shape      = EAreaShape.TYPE_CIRCLE;
        [FT] public bool          AllowRide  = false;
        [FT] public bool          AllowPK    = false;
        [FT] public bool          AllowTrade = false;
        [FT] public List<FTEvent> Events     = new List<FTEvent>();

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
            MapArea data    = new MapArea();
            data.ID         = this.ID;
            data.Name       = this.Name;
            data.Pos        = this.Pos;
            data.Shape      = (int)this.Shape;
            data.Scale      = this.Scale;
            data.AllowRide  = this.AllowRide;
            data.AllowPK    = this.AllowPK;
            data.AllowTrade = this.AllowTrade;
            for (int i = 0; i < Events.Count; i++)
            {
                data.Events.Add(Events[i].ID);
            }
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapArea data            = cfg as MapArea;
            this.ID                 = data.ID;
            this.Name               = data.Name;
            this.Pos                = data.Pos;
            this.Scale              = data.Scale;
            this.Shape              = (EAreaShape)data.Shape;
            this.AllowRide          = data.AllowRide;
            this.AllowPK            = data.AllowPK;
            this.AllowTrade         = data.AllowTrade;
            FTGroupEvent group = Map.GetGroup<FTGroupEvent>();
            for (int i = 0; i < data.Events.Count; i++)
            {
                FTEvent e = group.GetElement(data.Events[i]);
                if (e != null)
                {
                    Events.Add(e);
                }
            }
        }

        public override void OnDrawGizmos()
        {
 
            float x = transform.localScale.x;
            float z = transform.localScale.z;
            float y = transform.localScale.y;
            switch (Shape)
            {
                case EAreaShape.TYPE_CIRCLE:
                    float r = Mathf.Clamp(x, 0.1f, 2000);
                    transform.localScale = new Vector3(r, y, r);
                    FTDraw.DrawGizmosCylinder(transform.position, transform.rotation, transform.localScale, new Color(0.00f, 1.00f, 1.00f, 0.5f));
                    break;
                case EAreaShape.TYPE_RECT:
                    float w = Mathf.Clamp(x, 0.1f, 2000);
                    float l = Mathf.Clamp(z, 0.1f, 2000);
                    transform.localScale = new Vector3(w, y, l);
                    FTDraw.DrawGizmosCube(transform.position, transform.rotation, transform.localScale, new Color(0.00f, 1.00f, 1.00f, 0.5f), Vector3.one);
                    break;
            }
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID         = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            this.Name       = UnityEditor.EditorGUILayout.TextField("Name", this.Name);
            this.Shape      = (EAreaShape)UnityEditor.EditorGUILayout.EnumPopup("Shape", this.Shape);
            this.AllowRide  = UnityEditor.EditorGUILayout.Toggle("AllowRide", this.AllowRide);
            this.AllowPK    = UnityEditor.EditorGUILayout.Toggle("AllowPK", this.AllowPK);
            this.AllowTrade = UnityEditor.EditorGUILayout.Toggle("AllowTrade", this.AllowTrade);
            GUILayout.Space(5);
#endif
        }
    }
}