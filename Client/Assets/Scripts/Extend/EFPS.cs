using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class EFPS : MonoBehaviour
{
    private float mUpdateTime = 0.5f;
    private float mStartTimer;
    private float mFPS;
    private int   mFrames;
    private GUIStyle mStyle;

    void Start()
    {
        mStyle = new GUIStyle();
        Application.targetFrameRate = 300;
        mStartTimer = Time.realtimeSinceStartup;
        mFrames = 0;
        mStyle.fontSize = 20;
        mStyle.normal.textColor = new Color(0, 255, 0, 255);
    }

    void OnGUI()
    {
        GUI.Label(new Rect(20, 20, 200, 200), "FPS:" + mFPS.ToString("f2"), mStyle);
    }


    void Update()
    {
        mFrames++;
        if (Time.realtimeSinceStartup > mStartTimer+mUpdateTime)
        {
            mFPS = mFrames / (Time.realtimeSinceStartup - mStartTimer);
            mFrames = 0;
            mStartTimer = Time.realtimeSinceStartup;
        }
    }
}