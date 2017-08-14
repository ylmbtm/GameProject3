using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace ACT
{
    public class ActSystem : IGameLoop
    {
        private List<ActItem>   mRunItems     = new List<ActItem>();
        private List<ActItem>   mDelItems     = new List<ActItem>();
        private List<MoverBase> mMovers       = new List<MoverBase>();
        private List<MoverBase> mDeleteMovers = new List<MoverBase>();

        public void Run(ActItem item)
        {
            lock (mRunItems)
            {
                mRunItems.Add(item);
            }
        }

        public void Execute()
        {
            for (int i = 0; i < mRunItems.Count; i++)
            {
                ActItem item = mRunItems[i];
                item.Loop();
                if (item.Status == EActStatus.INITIAL || item.Status == EActStatus.SUCCESS)
                {
                    mDelItems.Add(item);
                }
            }
            for(int i=0;i<mDelItems.Count;i++)
            {
                ActItem item = mDelItems[i];
                mRunItems.Remove(item);
            }
            mDelItems.Clear();
            for (int i = 0; i < mMovers.Count; i++)
            {
                MoverBase mover = mMovers[i];
                if (mover.IsDead())
                {
                    mDeleteMovers.Add(mover);
                }
                else
                {
                    mover.Execute();
                }
            }
            for (int i = 0; i < mDeleteMovers.Count; i++)
            {
                mMovers.Remove(mDeleteMovers[i]);
            }
        }

        public void Release()
        {
            mDelItems.Clear();
            mRunItems.Clear();
            mMovers.Clear();
            mDeleteMovers.Clear();
        }

        public void MakeMove(ActMissile obj, Character target, HitCallbackFly onTriggerEnter)
        {
            MoverBase mover = null;
            switch (obj.MoveType)
            {
                case EMoveType.TYPE_FIXDIRECTION:
                    mover = new MoverFixDirection(obj, target, onTriggerEnter);
                    break;
                case EMoveType.TYPE_FIXPOSITION:
                    mover = new MoverFixPosition(obj, target, onTriggerEnter);
                    break;
                case EMoveType.TYPE_CHASE:
                    mover = new MoverChase(obj, target, onTriggerEnter);
                    break;
                case EMoveType.TYPE_CHASEPARABOL:
                    mover = new MoverChaseParabol(obj, target, onTriggerEnter);
                    break;
            }
            if (mover != null)
            {
                mMovers.Add(mover);
            }
        }

        public void MakeMove(ActFlyWeaponBind obj, Character target, HitCallbackFly onTriggerEnter)
        {
            MoverFlyWeaponBind mover = new MoverFlyWeaponBind(obj, target, onTriggerEnter);
            mMovers.Add(mover);
        }

        public void MakeMove(ActAnnular obj, Character target, HitCallbackFly onTriggerEnter, Vector3 rotatePoint, Transform rotateTrans)
        {
            MoverCircle mover = new MoverCircle(obj, target, onTriggerEnter, rotatePoint, rotateTrans);
            mMovers.Add(mover);
        }

        public void MakeMove(ActBeam obj, Character src, Character dst, HitCallbackBeam onTriggerEnter)
        {
            MoverBeam mover = new MoverBeam(obj, src, dst, onTriggerEnter);
            mMovers.Add(mover);
        }

        public void MakeMove(ActLighting obj, Character src, Character dst, HitCallbackLighting onTriggerEnter)
        {
            MoverLighting mover = new MoverLighting(obj, src, dst, onTriggerEnter);
            mMovers.Add(mover);
        }
    }
}

