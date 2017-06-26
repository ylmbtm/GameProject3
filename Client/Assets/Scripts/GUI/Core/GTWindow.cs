
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using DG.Tweening;
using DG.Tweening.Core;

public abstract class GTWindow
{
    protected Transform   transform;
    protected bool        mVisable   = false;
    protected bool        mResident  = false;
    protected string      mResPath   = string.Empty;

    public EWindowID        ID              { get; set; }
    public EWindowType      Type            { get; protected set; }
    public EWindowHideType  HideType        { get; protected set; }
    public EWindowMaskType  MaskType        { get; protected set; }
    public EWindowOpenType  OpenType        { get; protected set; }
    public bool             HasParentWindow { get; private set; }


    protected abstract void OnAwake();
    protected abstract void OnEnable();
    protected abstract void OnClose();
    protected abstract void OnAddButtonListener();
    protected abstract void OnAddHandler();
    protected abstract void OnDelHandler();

    public bool IsVisable()
    {
        return mVisable;
    }

    public bool IsResident()
    {
        return mResident;
    }

    public Transform CacheTransform
    {
        get { return transform; }
    }

    public UIPanel   Root
    {
        get
        {
            if (transform == null)
                return null;
            else
                return transform.GetComponent<UIPanel>();
        }
    }

    public bool Load()
    {
        if (string.IsNullOrEmpty(mResPath))
        {
            Debug.LogError("资源名为空");
            return false;
        }
        string path = string.Format("Guis/{0}", mResPath);
        GameObject prefab = GTResourceManager.Instance.Load<GameObject>(path);
        if (prefab == null)
        {
            Debug.LogError(string.Format("加载Window资源失败:{0}", mResPath));
            return false;
        }
        GameObject go = GameObject.Instantiate(prefab);
        transform = go.transform;
        transform.gameObject.SetActive(false);
        return true;
    }

    public void Show()
    {
        if (transform == null)
        {
            if (Load())
            {
                OnAwake();
            }
        }
        OnAddButtonListener();
        if (transform)
        {
            transform.gameObject.SetActive(true);     
            OnAddHandler();
            OnEnable();
        }
        mVisable = true;
    }

    public void Hide()
    {
        GTWindowManager.Instance.CloseWindow(ID);
    }

    public void Close()
    {
        if (transform)
        {
            mVisable = false;
            OnDelHandler();
            if (mResident)
            {
                transform.gameObject.SetActive(false);
            }
            else
            {
                transform.gameObject.SetActive(false);
                OnClose();
                Destroy();
                transform = null;
            }
        }
        HasParentWindow = false;
    }

    public void Destroy()
    {
        if (transform)
        {           
            GTResourceManager.Instance.Destroy(transform.gameObject);
        }
        mVisable = false;
    }

    public void SetParent(GTWindow parent)
    {
        if (transform == null)
            return;
        if (parent == null || parent.transform == null)
            return;
        transform.parent = parent.transform;
        HasParentWindow = true;
    }
}