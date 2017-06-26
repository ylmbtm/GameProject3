using UnityEngine;
using System.Collections;
using System;

public class UIGuide : GTWindow
{

    private Transform mClick;
    private Transform mDrag;
    private Transform mTip;
    private UILabel   mTipContent;
    private Transform mPivot;

    private Transform mSrcTarget;
    private Transform mNewTarget;
    private Callback  mCallback;

    public UIGuide()
    {
        mResident = true;
        mResPath = "Guide/UIGuide";
        Type = EWindowType.MASKED;
    }

    protected override void OnAwake()
    {
        mPivot = transform.Find("Pivot");
        mClick = transform.Find("Pivot/Click");
        mTip = transform.Find("Pivot/Tip");
        mTipContent = transform.Find("Pivot/Tip/Content").GetComponent<UILabel>();
        mDrag = transform.Find("Pivot/Drag");
    }

    protected override void OnAddButtonListener()
    {
        
    }

    protected override void OnAddHandler()
    {
        GTUpdate.Instance.AddListener(OnGuideUpdate);
    }

    protected override void OnEnable()
    {
        
    }

    protected override void OnDelHandler()
    {
        GTUpdate.Instance.DelListener(OnGuideUpdate);
    }

    protected override void OnClose()
    {
        
    }

    private void OnGuideUpdate()
    {
        if (mNewTarget == null || mSrcTarget == null)
        {
            return;
        }
        mNewTarget.transform.position = mSrcTarget.position;
        mClick.transform.position = mNewTarget.position;
    }

    private void OnExitGuide()
    {
        if (mNewTarget != null)
        {
            GameObject.DestroyImmediate(mNewTarget.gameObject);
            mNewTarget = null;
        }
    }

    private void OnHandPress(GameObject go, bool state)
    {
        if(mCallback!=null)
        {
            mCallback();
        }
        if (mSrcTarget == null)
        {
            return;
        }
        UIEventListener.BoolDelegate pOnPress = UIEventListener.Get(mSrcTarget.gameObject).onPress;
        if (pOnPress != null)
        {
            pOnPress(mSrcTarget.gameObject, state);
        };
    }

    private void OnHandDoubleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mCallback != null)
        {
            mCallback();
        }
        if (mSrcTarget == null)
        {
            return;
        }
        UIEventListener.VoidDelegate pOnDoubleClick = UIEventListener.Get(mSrcTarget.gameObject).onDoubleClick;
        if (pOnDoubleClick != null)
        {
            pOnDoubleClick(mSrcTarget.gameObject);
        };
    }

    private void OnHandClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mCallback != null)
        {
            mCallback();
        }
        if (mSrcTarget == null)
        {
            return;
        }
        UIEventListener.VoidDelegate pOnClick = UIEventListener.Get(mSrcTarget.gameObject).onClick;
        if (pOnClick != null)
        {
            pOnClick(mSrcTarget.gameObject);
        };
    }

    private void OnHandDrag(GameObject go,Vector2 delta)
    {
        if (mCallback != null)
        {
            mCallback();
        }
        if (mSrcTarget == null)
        {
            return;
        }
        UIEventListener.VectorDelegate pOnDrag = UIEventListener.Get(mSrcTarget.gameObject).onDrag;
        if (pOnDrag != null)
        {
            pOnDrag(mSrcTarget.gameObject,delta);
        };
    }

}
