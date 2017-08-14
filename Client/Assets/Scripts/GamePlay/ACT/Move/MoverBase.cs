using UnityEngine;
using System.Collections;

namespace ACT
{
    public enum EMoveType
    {
        TYPE_FIXDIRECTION  = 0,
        TYPE_FIXPOSITION   = 1,
        TYPE_CHASE         = 2,
        TYPE_CHASEPARABOL  = 3,
    }

    public delegate void HitCallback(MoverBase mover, Collider other);
    public delegate void HitCallbackFly(MoverFly mover, Collider other);
    public delegate void HitCallbackBeam(MoverBeam mover);
    public delegate void HitCallbackLighting(MoverLighting mover);

    public class MoverBase
    {
        public Transform         CacheTransform { get; protected set; }
        protected EffectData     mUnit;
        protected bool           mIsLoaded;

        public MoverBase()
        {

        }

        public virtual void Execute()
        {
            if (mIsLoaded == false || mUnit == null)
            {
                return;
            }
        }

        public virtual void Release()
        {
            if (mUnit != null)
            {
                mUnit.Release();
                mUnit = null;
            }
        }

        public bool IsDead()
        {
            if (mIsLoaded)
            {
                return mUnit == null ? true : mUnit.IsDead;
            }
            else
            {
                return false;
            };
        }

        public AnimationCurve GetCurve(int id)
        {
            DCurve db = null;
            if (id > 0)
            {
                db = ReadCfgCurve.GetDataById(id);
            }
            if (db != null)
            {
                return ECurve.Get(db.Path);
            }
            return null;
        }
    }
}