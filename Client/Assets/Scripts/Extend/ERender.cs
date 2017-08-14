using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ERender
{
    static Dictionary<int, ERender> Renders = new Dictionary<int, ERender>();
    public Camera                   RenderCamera  { get; private set; }
    public RenderTexture            RenderTexture { get; private set; }
    public int                      Layer         { get; private set; }
    public UITexture                ViewTexture   { get; private set; }

    public ERender(Camera cam, UITexture uiTexture, int layer)
    {
        this.RenderCamera = cam;
        this.Layer = layer;
        this.ViewTexture = uiTexture;
        this.RenderCamera.gameObject.layer = layer;
        this.RenderCamera.cullingMask = 1 << layer;
        this.RenderCamera.clearFlags = CameraClearFlags.SolidColor;
        this.RenderCamera.orthographic = false;
        this.RenderCamera.clearFlags = CameraClearFlags.SolidColor;
        this.RenderTexture = new RenderTexture(1024, 1024, 24);
        this.RenderCamera.targetTexture = this.RenderTexture;
        this.ViewTexture.mainTexture = this.RenderTexture;
        Color color = this.RenderCamera.backgroundColor;
        color.a = 0;
        this.RenderCamera.backgroundColor = color;
    }

    public static ERender AddRender(UITexture uiTexture)
    {
        if (uiTexture == null) return null;
        ERender render = null;
        for (int i= GTLayer.LAYER_RENDER_START;i<32;i++)
        {
            if (Renders.ContainsKey(i)) continue;
            Camera cam = GTCameraManager.Instance.CreateCamera("RenderCamera_" + i.ToString());
            cam.transform.localPosition = new Vector3(0, 0, 10000);
            render = new ERender(cam,uiTexture, i);
            Renders.Add(i, render);
            break;
            
        }
        return render;
    }

    public static ERender DelRender(ERender render)
    {
        if (render != null && render.RenderCamera != null)
        {
            render.Release();
        }
        return null;
    }

    public GameObject AddModel(string path)
    {
        if (string.IsNullOrEmpty(path)) return null;
        GameObject model = GTResourceManager.Instance.Load<GameObject>(path, true);
        AddModel(model);
        return model;
    }

    public GameObject AddModel(GameObject model)
    {
        if (RenderCamera == null) return null;
        if (model != null)
        {
            model.transform.parent = this.RenderCamera.transform;
            NGUITools.SetLayer(model, Layer);
        }
        return model;
    }

    public void DelModel(string path)
    {
        if (RenderCamera == null) return;
        if (string.IsNullOrEmpty(path)) return;
        Transform child = RenderCamera.transform.Find(path);
        if(child!=null)
        {
            Object.Destroy(child.gameObject);
        }
    }

    public void DelAllModels()
    {
        if (RenderCamera == null) return;
        while (RenderCamera.transform.childCount > 0)
        {
            Transform child = RenderCamera.transform.GetChild(0);
            child.parent = null;
            GameObject.DestroyImmediate(child.gameObject);
        }
    }

    public void Release()
    {
        DelAllModels();
        Renders.Remove(Layer);
        RenderTexture.Release();
        Object.Destroy(RenderTexture);
        Object.Destroy(RenderCamera.gameObject);
        RenderTexture = null;
        RenderCamera = null;
    }

}
