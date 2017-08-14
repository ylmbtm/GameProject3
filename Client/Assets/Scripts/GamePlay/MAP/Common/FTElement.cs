using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTElement : MonoBehaviour
    {
        private FTWorldMap m_FTWorldMap;

        [FT] public Vector3    Pos
        {
            get { return transform.position; }
            set { transform.position = value; }
        }

        [FT] public Vector3    EulerAngles
        {
            get { return transform.eulerAngles; }
            set { transform.eulerAngles = value; }
        }

        [FT] public Vector3    Scale
        {
            get { return transform.localScale; }
            set { transform.localScale = value; }
        }

        [FT] public float      Face
        {
            get { return transform.eulerAngles.y; }
            set { transform.eulerAngles = new Vector3(0, value, 0); }
        }

        [FT] public FTWorldMap Map
        {
            get
            {
                if (m_FTWorldMap == null)
                {
                    m_FTWorldMap = GetComponentInParent<FTWorldMap>();
                }
                return m_FTWorldMap;
            }
        }

        public virtual DCFG Export()
        {
            return null;
        }

        public virtual void Import(DCFG cfg)
        {

        }

        public virtual void OnDrawGizmos()
        {

        }

        public virtual void OnDrawInspector()
        {

        }

        public virtual void MoveToGround()
        {
            Ray ray1 = new Ray(transform.position, Vector3.down);
            RaycastHit hit;
            if (Physics.Raycast(ray1, out hit, Mathf.Infinity, LayerMask.GetMask("Default")))
            {
                transform.position = hit.point;
                return;
            }

            Ray ray2 = new Ray(transform.position, Vector3.up);
            if (Physics.Raycast(ray2, out hit, Mathf.Infinity, LayerMask.GetMask("Default")))
            {
                transform.position = hit.point;
                return;
            }
        }
    }
}