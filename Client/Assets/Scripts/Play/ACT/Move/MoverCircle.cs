using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverCircle : MoverFly
    {
        private Transform  m_rotateTrans;
        private Vector3    m_rotatePoint;
        private ActAnnular m_Annular;

        public MoverCircle(ActAnnular obj, Character target, HitCallbackFly onTriggerEnter, Vector3 rotatePoint, Transform rotateTrans) :
            base(obj, target, onTriggerEnter)
        {
            this.m_rotatePoint = rotatePoint;
            this.m_rotateTrans = rotateTrans;
            this.m_Annular = obj;
        }

        public override void Execute()
        {
            base.Execute();
            if (m_rotateTrans == null)
            {
                Vector3 axis = new Vector3(0, this.m_rotatePoint.y, 0);
                if (m_Annular.NegativeDirection)
                {
                    this.CacheTransform.Rotate(axis, -Time.deltaTime * mMoveSpeed, Space.Self);
                }
                else
                {
                    this.CacheTransform.Rotate(axis, Time.deltaTime * mMoveSpeed, Space.Self);
                }
            }
            else
            {

                this.CacheTransform.parent = m_rotateTrans;
                Vector3 axis = this.m_rotateTrans.up;
                Vector3 point = new Vector3(this.m_rotateTrans.position.x, this.CacheTransform.position.y, this.m_rotateTrans.position.z);
                if(m_Annular.NegativeDirection)
                {
                    this.CacheTransform.RotateAround(point, Vector3.up, Time.deltaTime * mMoveSpeed);
                }
                else
                {
                    this.CacheTransform.RotateAround(point, Vector3.up, -Time.deltaTime * mMoveSpeed);
                }
            }
        }
    }
}