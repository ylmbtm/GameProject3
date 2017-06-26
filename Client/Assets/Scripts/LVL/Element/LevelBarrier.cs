using UnityEngine;
using System.Collections;
using CFG;

namespace LVL
{
    [LevelElementVariable(CfgType = typeof(MapBarrier))]
    public class LevelBarrier : LevelElement
    {
        [LevelVariable]
        public float Width = 14;
         
        public override void Build()
        {
            float width = Width < 1 ? 14 : Width;
            int count = Mathf.CeilToInt(Width / 14);

            Vector3 size = Vector3.zero;
            size.x = count * 14;
            size.y = 4;
            size.z = 1.5f;

            Transform body = transform.Find("Body");
            if (body == null)
            {
                body = new GameObject("Body").transform;
                body.parent = transform;
                body.transform.localPosition = Vector3.zero;
                body.localEulerAngles = Vector3.zero;
            }
            else
            {
                NGUITools.DestroyChildren(body);
            }
            float halfCount = count * 0.5f;
            for (int i = 0; i < count; i++)
            {
                GameObject unit = GTResourceManager.Instance.Instantiate(GTPrefabKey.PRE_BARRIER);
                if (unit == null)
                {
                    return;
                }
                unit.name = i.ToString();
                Transform trans = unit.transform;
                Vector3 localPosition = Vector3.right * (i - halfCount + 0.5f) * 14;
                localPosition.z = size.z * 0.5f;
                trans.localPosition = localPosition;
                trans.SetParent(body, false);
            }
            BoxCollider bc = gameObject.GET<BoxCollider>();
            bc.size = size;
            bc.center = new Vector3(0, size.y * 0.5f, size.z * 0.5f);
            NGUITools.SetLayer(gameObject, GTLayer.LAYER_BARRER);
        }
    }
}

