using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class ECurve : MonoBehaviour
{
    [SerializeField]
    public AnimationCurve curve;
    static Dictionary<string, AnimationCurve> mCurves = new Dictionary<string, AnimationCurve>();

    static public AnimationCurve Get(string path)
    {
        AnimationCurve curve = null;
        mCurves.TryGetValue(path, out curve);
        if(curve==null)
        {
            GameObject go = GTResourceManager.Instance.Load<GameObject>(path);
            if (go.GetComponent<ECurve>())
            {
                ECurve cur = go.GetComponent<ECurve>();
                curve = cur.curve;
                mCurves.Add(path, curve);
            }
        }
        return curve;
    }
}