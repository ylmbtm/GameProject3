using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class UILoading : GTWindow
{
    public string [] mBackTextures = new string[1]
    {
        "Textures/Background/Background_1",
    };
    
    public UILoading()
    {
        Type = EWindowType.LOADED;
        mResident = false;
        mResPath = "Loading/UILoading";
    }

    private UISlider   mProgressBar;
    private UILabel    mProgressText;
    private UITexture  mBackground;

    protected override void OnAwake()
    {
        mProgressBar = transform.transform.Find("ProgressBar").GetComponent<UISlider>();
        mProgressText = transform.Find("ProgressText").GetComponent<UILabel>();
        mBackground = transform.Find("Background").GetComponent<UITexture>();
    }

    protected override void OnAddButtonListener()
    {
        
    }

    protected override void OnAddHandler()
    {
        
    }

    protected override void OnEnable()
    {
        int ran = UnityEngine.Random.Range(0, mBackTextures.Length);
        mBackground.width = 1280;
        mBackground.mainTexture = GTResourceManager.Instance.Load<Texture2D>(mBackTextures[ran]);
        UpdateProgress(0);
    }

    protected override void OnDelHandler()
    {
        
    }

    protected override void OnClose()
    {
        
    }

    public void UpdateProgress(float progress)
    {
        mProgressBar.value = progress;
        mProgressText.text = GTTools.ToPercent(progress);
    }
}
