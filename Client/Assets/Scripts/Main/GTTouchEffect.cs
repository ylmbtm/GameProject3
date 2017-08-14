using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class GTTouchEffect : GTMonoSingleton<GTTouchEffect>
{
    private Camera           mTouchCam;
    private GameObject       mTouchEffect;
    private float            mDistanceToCam = 2f;
    private ParticleSystem   mParticle;
    private bool             mActive = false;
    private bool             mIsLoop = false;

    public override void SetRoot(Transform parent)
    {
        base.SetRoot(parent);
        this.mTouchEffect = GTWorld.Instance.Ect.LoadEffectObj(GTEffectKey.EFFECT_TOUCH);
        if (mTouchEffect == null)
        {
            return;
        }
        mTouchEffect.transform.parent = transform;
        mTouchEffect.layer = GTLayer.LAYER_TOUCHEFFECT;
        mParticle = mTouchEffect.GetComponent<ParticleSystem>();
        mTouchCam = GTCameraManager.Instance.CreateCamera("TouchCamera");
        mTouchCam.depth = 12;
        mTouchCam.fieldOfView = 40;
        mTouchCam.clearFlags = CameraClearFlags.Depth;
        mTouchCam.cullingMask = 1 << GTLayer.LAYER_TOUCHEFFECT;
        mTouchCam.farClipPlane = mDistanceToCam + 1;

        AddEventListeners();
    }

    void AddEventListeners()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_MOUSE_BUTTON_UP,   OnMouseButtonUp);
        GTEventCenter.AddHandler(GTEventID.TYPE_MOUSE_BUTTON_DOWN, OnMouseButtonDown);
    }

    void DelEventListeners()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_MOUSE_BUTTON_UP,   OnMouseButtonUp);
        GTEventCenter.DelHandler(GTEventID.TYPE_MOUSE_BUTTON_DOWN, OnMouseButtonDown);
    }

    private void SetEffectActive(bool active)
    {
        mActive = active;
    }

    private void DoActiveEvent()
    {
        if(mActive)
        {
            Vector3 pos = Input.mousePosition;
            pos = mTouchCam.ScreenToWorldPoint(new Vector3(pos.x, pos.y, mDistanceToCam));
            mTouchEffect.transform.position = pos;
        }
        if (mIsLoop != mActive)
        {
            mIsLoop = mActive;
            mParticle.loop = mIsLoop;
            if (mIsLoop)
            {
                mParticle.Play();
            }
        }
    }

    private void OnMouseButtonDown()
    {
        if (mTouchEffect == null || mParticle == null || mTouchCam == null)
        {
            return;
        }
        SetEffectActive(true);
        DoActiveEvent();
    }

    private void OnMouseButtonUp()
    {
        if (mTouchEffect == null || mParticle == null || mTouchCam == null)
        {
            return;
        }
        SetEffectActive(false);
        DoActiveEvent();
    }

    private void OnDestroy()
    {
        DelEventListeners();
    }
}
