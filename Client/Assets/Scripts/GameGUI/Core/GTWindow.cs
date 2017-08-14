using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using DG.Tweening;
using DG.Tweening.Core;


public abstract class GTWindow
{
    public Transform        transform       { get; set; }
    public bool             Resident        { get; set; }
    public string           Path            { get; set; }
    public EWindowID        ID              { get; set; }
    public EWindowType      Type            { get; set; }
    public EWindowHideType  HideType        { get; set; }
    public EWindowMaskType  MaskType        { get; set; }
    public EWindowOpenType  OpenType        { get; set; }
    public EWindowShowMode  ShowMode        { get; set; }
    public Action           OnMaskClick     { get; set; }


    public EWindowID        TargetID        { get; set; }

    protected abstract void OnAwake();
    protected abstract void OnEnable();
    protected abstract void OnClose();
    protected abstract void OnAddButtonListener();
    protected abstract void OnAddHandler();
    protected abstract void OnDelHandler();

    public bool             IsVisable()
    {
        return transform == null ? false : transform.gameObject.activeSelf;
    }

    public bool             IsResident()
    {
        return Resident;
    }

    public UIPanel          Panel
    {
        get
        {
            if (transform == null)
                return null;
            else
                return transform.GetComponent<UIPanel>();
        }
    }

 
    public void Show()
    {
        if (transform == null)
        {
            if (LoadAsync())
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
    }

    public void Hide()
    {
        GTWindowManager.Instance.HideWindow(ID);
    }

    public bool LoadAsync()
    {
        if (string.IsNullOrEmpty(Path))
        {
            Debug.LogError("资源名为空");
            return false;
        }
        string path = string.Format("Guis/{0}", Path);
        GameObject prefab = GTResourceManager.Instance.Load<GameObject>(path);
        if (prefab == null)
        {
            Debug.LogError(string.Format("加载Window资源失败:{0}", Path));
            return false;
        }
        GameObject go = GameObject.Instantiate(prefab);
        transform = go.transform;
        transform.gameObject.SetActive(false);
        return true;
    }

    public void HideAsync()
    {
        TargetID = EWindowID.UINone;
        if (transform)
        {
            OnDelHandler();
            if (Resident)
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
    }

    public void Destroy()
    {
        TargetID = EWindowID.UINone;
        if (transform)
        {           
            GTResourceManager.Instance.Destroy(transform.gameObject);
        }
    }

    public void SetActive(bool active)
    {
        if(transform == null)
        {
            return;
        }
        transform.gameObject.SetActive(active);
    }
}