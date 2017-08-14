using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.IO;

public class GTResourceManager : GTSingleton<GTResourceManager>
{
    public Dictionary<string, GTResourceUnit>    Units          = new Dictionary<string, GTResourceUnit>();   //以AssetName为Key，保存Asset
    public Dictionary<string, GTResourceBundle>  Bundles        = new Dictionary<string, GTResourceBundle>(); //保存所有的AssetBundle
    public ResourceType                          Type           = ResourceType.TYPE_BUNDLE;                 //资源加载方式
    
    private bool                                 IsRead
    {
        get; set;
    }

    public string BundlePPath
    {
        get
        {
            return GetBundlePersistentPath();
        }
    }

    public string BundleSPath
    {
        get
        {
            return GetBundleStreamingPath();
        }
    }

    public string WWWPath
    {
        get
        {
            return GetWWWPath();
        }

    }

    public string AssetConfigPath
    {
        get
        {
            return GetAssetConfigPath();
        }
    }

    public string GetBundlePersistentPath()
    {
        switch (Application.platform)
        {
            case RuntimePlatform.Android:
            case RuntimePlatform.IPhonePlayer:
                return Application.persistentDataPath;
            case RuntimePlatform.WindowsEditor:
            case RuntimePlatform.WindowsPlayer:
                return Application.dataPath.Replace("/Assets", string.Empty) + "/AssetBundles";
            default:
                return Application.dataPath.Replace("/Assets", string.Empty) + "/AssetBundles";
        }
    }

    public string GetBundleStreamingPath()
    {

        switch (Application.platform)
        {
            case RuntimePlatform.Android:
            case RuntimePlatform.IPhonePlayer:
                return Application.streamingAssetsPath;
            case RuntimePlatform.WindowsEditor:
            case RuntimePlatform.WindowsPlayer:
                return Application.dataPath.Replace("/Assets", string.Empty) + "/AssetBundles";
            default:
                return Application.dataPath.Replace("/Assets", string.Empty) + "/AssetBundles";
        }

    }

    public string GetWWWPath()
    {
        switch (Application.platform)
        {
            case RuntimePlatform.WindowsEditor:
            case RuntimePlatform.OSXEditor:
            case RuntimePlatform.WindowsPlayer:
            case RuntimePlatform.IPhonePlayer:
            case RuntimePlatform.Android:
                return "file:///";
            default:
                return string.Empty;
        }
    }

    public string GetAssetConfigPath()
    {
        switch (Application.platform)
        {
            case RuntimePlatform.Android:
            case RuntimePlatform.IPhonePlayer:
                return Application.persistentDataPath;
            case RuntimePlatform.WindowsEditor:
            case RuntimePlatform.WindowsPlayer:
                return Application.streamingAssetsPath;
            default:
                return Application.streamingAssetsPath;
        }
    }

    public string GetExtPath()
    {
        string path = string.Empty;
        switch (Application.platform)
        {
            case RuntimePlatform.WindowsEditor:
                path = Application.streamingAssetsPath;
                break;
            case RuntimePlatform.Android:
                path = Application.persistentDataPath;
                break;
            case RuntimePlatform.IPhonePlayer:
                path = Application.persistentDataPath;
                break;
            default:
                path = Application.streamingAssetsPath;
                break;

        }
        return path;
    }

    public string GetDataPath()
    {
        return Application.streamingAssetsPath;
    }

    public void LoadConfig()
    {
#if UNITY_EDITOR
#else
        Type = ResourceType.TYPE_BUNDLE;
#endif
        if(IsRead)
        {
            return;
        }
        IsRead = true;
        string fsPath = AssetConfigPath + "/Asset.xml";
        StreamReader fs = new StreamReader(fsPath);
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(fs.ReadToEnd());
        XmlNodeList list = doc.SelectSingleNode("root").ChildNodes;
        foreach (var current in list)
        {
            XmlElement element = current as XmlElement;
            if (element == null)
            {
                continue;
            }
            GTResourceUnit u = new GTResourceUnit();
            for (int i = 0; i < element.Attributes.Count; i++)
            {
                XmlAttribute attr = element.Attributes[i];
                switch (attr.Name)
                {
                    case "AssetBundleName":
                        u.AssetBundleName = attr.Value;
                        break;
                    case "AssetName":
                        u.AssetName = attr.Value;
                        break;
                    case "Path":
                        u.Path = attr.Value;
                        break;
                    case "GUID":
                        u.GUID = attr.Value;
                        break;
                }
            }
            Units.Add(u.AssetName, u);
            GTResourceBundle bundle = null;
            Bundles.TryGetValue(u.AssetBundleName, out bundle);
            if (bundle == null)
            {
                bundle = new GTResourceBundle();
                bundle.AssetBundleName = u.AssetBundleName;
                Bundles.Add(u.AssetBundleName, bundle);
            }
        }
        fs.Dispose();
        fs.Close();
    }

    void LoadBundle(string assetName, System.Action<Object> callback)
    {
        GTResourceUnit unit = null;
        Units.TryGetValue(assetName, out unit);
        if (unit == null)
        {
            Debug.LogError("配置表中不存在：" + assetName);
            return;
        }
        GTResourceBundle bundle = null;
        Bundles.TryGetValue(unit.AssetBundleName, out bundle);
        if (bundle == null)
        {
            Debug.LogError("不存在这个Bundle：" + unit.AssetBundleName);
            return;
        }
        AddLoadBundleTask(unit.AssetName, bundle, null, callback);
    }

    void LoadSource(string assetName, System.Action<Object> callback) 
    {
#if UNITY_EDITOR
        if (string.IsNullOrEmpty(assetName))
        {
            return;
        }
        GTResourceUnit unit = null;
        Units.TryGetValue(assetName, out unit);
        if (unit == null)
        {
            Debug.LogError("配置表中不存在：" + assetName);
            return;
        }
        Object asset = UnityEditor.AssetDatabase.LoadAssetAtPath<Object>(unit.Path);
        if (asset == null)
        {
            Debug.LogError("不存在这个资源：" + assetName);
            return;
        }
        if (callback != null)
        {
            callback.Invoke(asset);
        }
#endif
    }

    void AddLoadBundleTask(string assetName, GTResourceBundle bundle, GTResourceTask parentTask, System.Action<Object> callback) 
    {
        GTResourceTask task = new GTResourceTask();
        task.Bundle = bundle;
        task.LoadedDepCount = 0;
        task.Parent = parentTask;
        task.AssetName = assetName;
        task.AssetCallback = callback;
        GTCoroutinueManager.Instance.StartCoroutine(LoadAsyncBundle(task));
    }

    public IEnumerator LoadMainAssetBundleManifest()
    {
        yield return LoadAsyncManifest(null);
    }

    IEnumerator LoadAsyncManifest(System.Action callback)
    {
        string url = string.Format("{0}{1}/{2}", WWWPath, BundlePPath, "AssetBundles");
        WWW www = new WWW(url);
        yield return www;
        if (!string.IsNullOrEmpty(www.error))
        {
            Debug.LogError(www.error);
            yield break;
        }
        AssetBundle ab = www.assetBundle;
        AssetBundleManifest maniFest = (AssetBundleManifest)ab.LoadAsset("AssetBundleManifest");
        ab.Unload(false);
        foreach (var current in maniFest.GetAllAssetBundles())
        {
            string[] deps = maniFest.GetAllDependencies(current);
            if (deps == null || deps.Length == 0)
            {
                continue;
            }
            GTResourceBundle bundle = null;
            Bundles.TryGetValue(current, out bundle);
            if (bundle == null)
            {
                continue;
            }
            bundle.Deps.AddRange(deps);
        }
        if (callback != null)
        {
            callback.Invoke();
        }
        www.Dispose();
        yield return null;
    }

    IEnumerator LoadAsyncBundle(GTResourceTask task) 
    {
        GTResourceBundle taskBundle = task.Bundle;
        AssetBundle ab = null;

        if (taskBundle.State >= GTResourceBundle.TYPE_LOADING)
        {
            while (taskBundle.State != GTResourceBundle.TYPE_LOADED)
            {
                yield return null;
            }
            ab = taskBundle.AB;
        }
        else
        {
            taskBundle.State = GTResourceBundle.TYPE_LOADING;
            string url = string.Format("{0}{1}/{2}", WWWPath, BundlePPath, taskBundle.AssetBundleName);
            WWW www = new WWW(url);
            yield return www;
            if (!string.IsNullOrEmpty(www.error))
            {
                Debug.LogError(www.error);
                yield break;
            }
            ab = www.assetBundle;
            taskBundle.State = GTResourceBundle.TYPE_LOADED;
            taskBundle.AB = ab;
            www.Dispose();
        }

        for (int i = 0; i < taskBundle.Deps.Count; i++)
        {
            string depAbName = taskBundle.Deps[i];
            GTResourceBundle dpBundle = null;
            Bundles.TryGetValue(depAbName, out dpBundle);
            if (dpBundle == null)
            {
                continue;
            }
            AddLoadBundleTask(null, dpBundle, task, null);
        }
        if (task.Parent != null)
        {
            task.Parent.LoadedDepCount++;
        }
        while (task.LoadedDepCount < taskBundle.Deps.Count)
        {
            yield return null;
        }
        AssetBundleRequest req = null;
        if (task.AssetName != null)
        {
            req = ab.LoadAssetAsync<Object>(task.AssetName);
            while (!req.isDone)
            {
                yield return null;
            }
        }
        if (req != null)
        {
            if (req.asset == null)
            {
                Debug.LogError(ab);
                yield break;
            }
            if (task.AssetCallback != null)
            {
                task.AssetCallback.Invoke(req.asset);
            }
        }
        yield return 0;
    }

    public void Load(string assetName, System.Action<Object> callback)
    {
        switch(Type)
        {
            case ResourceType.TYPE_BUNDLE:
                LoadBundle(assetName, callback);
                break;
            case ResourceType.TYPE_SOURCE:
                LoadSource(assetName, callback);
                break;
        }
    }

    public void UnloadAssetBundle(string assetBundleName, bool all = false)
    {
        GTResourceBundle rb = null;
        Bundles.TryGetValue(assetBundleName, out rb);
        if (rb != null)
        {
            if (rb.State == GTResourceBundle.TYPE_LOADED && rb.AB != null)
            {
                Bundles.Remove(assetBundleName);
                rb.AB.Unload(all);
            }
        }
    }

    public void UnloadAssetBundleByAssetName(string assetName, bool all = false)
    {
        GTResourceUnit unit = null;
        Units.TryGetValue(assetName, out unit);
        if (unit != null)
        {
            UnloadAssetBundle(unit.AssetBundleName);
        }
    }

    public T Load<T>(string path, bool instance = false) where T : UnityEngine.Object
    {
        T asset = Resources.Load<T>(path) as T;
        if (asset != null && instance)
        {
            return UnityEngine.Object.Instantiate(asset);
        }
        return asset;
    }

    public GameObject Instantiate(string path, Vector3 position, Quaternion rotation)
    {
        GameObject asset = Load<GameObject>(path);
        GameObject go = null;
        if (asset != null)
        {
            go = (GameObject)GameObject.Instantiate(asset, position, rotation);
        }
        return go;
    }

    public GameObject Instantiate(string path)
    {
        GameObject asset = Load<GameObject>(path);
        GameObject go = null;
        if (asset != null)
        {
            go = (GameObject)GameObject.Instantiate(asset);
        }
        return go;
    }

    public void Destroy(GameObject go)
    {
        GameObject.Destroy(go);
    }

    public void DestroyObj(UnityEngine.Object obj)
    {
        if (obj != null)
        {
            UnityEngine.Object.Destroy(obj);
        }
    }

    public void LoadFromStreamingAssets(string sPath, string pPath)
    {
        string targetPath = Application.persistentDataPath + sPath;
        string sourcePath = Application.streamingAssetsPath + pPath;
        if (Application.platform == RuntimePlatform.Android)
        {
            if (File.Exists(targetPath))
            {
                File.Delete(targetPath);
            }
            WWW www = new WWW(sourcePath);
            bool boo = true;
            while (boo)
            {
                if (www.isDone)
                {
                    File.WriteAllBytes(targetPath, www.bytes);
                    boo = false;
                }
            }
        }
    }
}
