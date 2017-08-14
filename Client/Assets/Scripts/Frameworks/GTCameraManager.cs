using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class GTCameraManager : GTMonoSingleton<GTCameraManager>
{
    private UIRoot        mRoot = null;
    private Transform     mAnchor = null;
    private Vector3       mRetailPos;
    private Vector3       mRetailEuler;
    private RenderTexture mMiniMapTexture;
    private GameObject    mFreeLook;

    public Camera           MainCamera    { get; private set; }
    public Camera           MinimapCamera { get; private set; }
    public Camera           NGUICamera    { get; private set; }
    public CameraFollow     CameraCtrl    { get; private set; }

    public const int DEPTH_CAM_MAIN       = 0;
    public const int DEPTH_CAM_2DUICAMERA = 6;
    public const int DEPTH_CAM_MINIMAP    = 4;

    public override void SetRoot(Transform parent)
    {
        base.SetRoot(parent);
        this.AddRoot();
    }

    public Camera CreateCamera(string name)
    {
        GameObject go = new GameObject(name);
        go.transform.parent = transform;
        go.transform.localPosition = Vector3.zero;
        go.transform.localRotation = Quaternion.identity;
        go.transform.localScale = Vector3.one;
        return go.AddComponent<Camera>();
    }

    public void CreateMainCamera(Transform trans)
    {
        if (mFreeLook == null)
        {
            mFreeLook = GTResourceManager.Instance.Load<GameObject>("Model/Other/FreeLook", true);
            MainCamera = mFreeLook.GetComponentInChildren<Camera>();
            GTTools.SetTag(MainCamera.gameObject, GTTools.Tags.MainCamera);
            mFreeLook.transform.parent = transform;
            mRetailPos = MainCamera.transform.localPosition;
            mRetailEuler = MainCamera.transform.localEulerAngles;
            MainCamera.fieldOfView = 60;
            MainCamera.renderingPath = RenderingPath.Forward;
            MainCamera.depth = DEPTH_CAM_MAIN;
        }
        this.CameraCtrl = mFreeLook.GET<CameraFollow>();
        this.CameraCtrl.SetTarget(trans);
    }

    public void CreateMiniCamera(Transform target)
    {
        MinimapCamera = CreateCamera("MiniMapCamera");
        MinimapCamera.transform.parent = target;
        MinimapCamera.transform.localPosition = new Vector3(0, 10, 0);
        MinimapCamera.transform.localEulerAngles = new Vector3(90, 0, 0);
        MinimapCamera.transform.localScale = Vector3.one;
        MinimapCamera.cullingMask = (1 << GTLayer.LAYER_DEFAULT) + (1 << GTLayer.LAYER_MINI);
        MinimapCamera.clearFlags = CameraClearFlags.SolidColor;
        MinimapCamera.orthographic = true;
        MinimapCamera.orthographicSize = 8;
        if (mMiniMapTexture == null)
        {
            mMiniMapTexture = new RenderTexture(128, 128, 24);
        }
        MinimapCamera.targetTexture = mMiniMapTexture;
    }

    public void AddUI(GameObject go)
    {
        if (go == null)
        {
            return;
        }
        go.transform.parent = mAnchor;
        go.transform.localPosition = Vector3.zero;
        go.transform.localEulerAngles = Vector3.zero;
        go.transform.localScale = Vector3.one;
        NGUITools.SetLayer(go, mRoot.gameObject.layer);
    }

    void AddRoot()
    {
        if (UIRoot.list.Count > 0)
        {
            Destroy(UIRoot.list[0].gameObject);
            UIRoot.list.Clear();
            UIPanel.list.Clear();
        }
        NGUITools.CreateUI(false);
        mRoot = UIRoot.list[0];
        NGUITools.SetLayer(mRoot.gameObject, GTLayer.LAYER_UI);

        mRoot.scalingStyle = UIRoot.Scaling.Flexible;
        mRoot.minimumHeight = 320;
        mRoot.maximumHeight = 4096;
        mRoot.transform.localPosition = new Vector3(0, 0, -10000);
        NGUICamera = UICamera.eventHandler.cachedCamera;
        NGUICamera.clearFlags = CameraClearFlags.Depth;
        NGUICamera.depth = DEPTH_CAM_2DUICAMERA;
        NGUICamera.nearClipPlane = -10;
        NGUICamera.farClipPlane = 1200;
        DontDestroyOnLoad(mRoot);

        List<Transform> childs = new List<Transform>();
        for (int i = 0; i < mRoot.transform.childCount; i++)
        {
            Transform cc = mRoot.transform.GetChild(i);
            if (cc != NGUICamera.transform)
            {
                childs.Add(cc);
            }
        }
        for (int i = 0; i < NGUICamera.transform.childCount; i++)
        {
            Transform cc = NGUICamera.transform.GetChild(i);
            childs.Add(cc);
        }
        while (childs.Count > 0)
        {
            Transform child = childs[0];
            childs.Remove(child.transform);
            GameObject.Destroy(child.gameObject);
        }

        mAnchor = mRoot.gameObject.AddChild().transform;
        mAnchor.gameObject.name = "Anchor";
        mAnchor.localPosition = new Vector3(0, 0, 0);
        mAnchor.localScale = Vector3.one;
        mAnchor.localRotation = Quaternion.identity;
    }
}