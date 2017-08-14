using UnityEngine;
using System.Collections;
using System;
using CFG;

namespace MAP
{
    public class FTBarrier : FTComponent
    {
        [FT] public float L = 14;
        [FT] public float H = 10;
        [FT] public float W = 1;

        public override void Startup()
        {

        }

        public override void Trigger()
        {
            float length = L < 1 ? 14 : L;
            int count = Mathf.CeilToInt(L / 14);
            Vector3 size = Vector3.zero;
            size.x = count * 14;
            size.y = H;
            size.z = W;
            Transform body = transform.Find("Body");
            if (body == null)
            {
                body = new GameObject("Body").transform;
                body.parent = transform;
                body.transform.localPosition = Vector3.zero;
                body.localEulerAngles = Vector3.zero;
                body.hideFlags = HideFlags.HideInHierarchy;
            }
            else
            {
                NGUITools.DestroyChildren(body);
            }
            float halfCount = count * 0.5f;
            for (int i = 0; i < count; i++)
            {
                GameObject go = GTResourceManager.Instance.Instantiate(GTPrefabKey.PRE_BARRIER);
                if (go == null)
                {
                    return;
                }
                go.name = i.ToString();
                Transform trans = go.transform;
                Vector3 localPosition = Vector3.right * (i - halfCount + 0.5f) * 14;
                localPosition.z = size.z * 0.5f;
                trans.localPosition = localPosition;
                trans.SetParent(body, false);
            }
            BoxCollider bc = gameObject.GET<BoxCollider>();
            bc.size = size;
            bc.center = new Vector3(0, size.y * 0.5f - 1f, size.z * 0.5f);
            bc.hideFlags = HideFlags.HideInInspector;
        }

        public override void Release()
        {

        }

        public override DCFG Export()
        {
            MapBarrier data = new MapBarrier();
            data.ID         = this.ID;
            data.L          = this.L;
            data.H          = this.H;
            data.W          = this.W;
            data.Pos        = this.Pos;
            data.Face       = this.Face;
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapBarrier data = cfg as MapBarrier;
            this.ID         = data.ID;
            this.L          = data.L;
            this.H          = data.H;
            this.W          = data.W;
            this.Pos        = data.Pos;
            this.Face       = data.Face;
        }

        public override void OnDrawGizmos()
        {
            transform.localScale = Vector3.one;
            FTDraw.DrawGizmosCube(transform.position + new Vector3(0, H * 0.5f, 0), transform.rotation, transform.localScale, Color.red, new Vector3(L, H, W));
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            this.L  = UnityEditor.EditorGUILayout.FloatField("L", this.L);
            this.H  = UnityEditor.EditorGUILayout.FloatField("H", this.H);
            this.W  = UnityEditor.EditorGUILayout.FloatField("W", this.W);
            GUILayout.Space(5);
#endif
        }
    }
}