using UnityEngine;
using System.Collections;
using UnityEditor;
using System.Xml;
using System.IO;
using System.Collections.Generic;
using System;

public class ResourceTools
{
    private static string OUTPUTPATH = Application.dataPath.Replace("/Assets", string.Empty) + "/GameCache/AssetBundles";
    private static Dictionary<string, GTResourceUnit> Units = new Dictionary<string, GTResourceUnit>();


    [MenuItem("工具/新版配置文件")]
    public static void BuildConfig()
    {
        DelBundles();
        DelBundleNames();
        SetBundleConfig();
        SetBundleNames();
    }

    [MenuItem("工具/新版打包")]
    public static void BuildBundle()
    {
        BuildBundles();
    }

    static void DelBundles()
    {
        if (Directory.Exists(OUTPUTPATH))
        {
            Directory.Delete(OUTPUTPATH, true);
        }
        Directory.CreateDirectory(OUTPUTPATH);
    }

    static void DelBundleNames()
    {
        var length = AssetDatabase.GetAllAssetBundleNames().Length;
        var oldAssetBundleNames = new string[length];
        for (var i = 0; i < length; i++)
        {
            oldAssetBundleNames[i] = AssetDatabase.GetAllAssetBundleNames()[i];
        }
        for (var j = 0; j < oldAssetBundleNames.Length; j++)
        {
            AssetDatabase.RemoveAssetBundleName(oldAssetBundleNames[j], true);
        }
    }

    static void SetBundleConfig()
    {
        List<GTResourceUnit> list = new List<GTResourceUnit>();
        UnityEngine.Object[] assets = Selection.GetFiltered(typeof(UnityEngine.Object), SelectionMode.DeepAssets);
        for (int i = 0; i < assets.Length; i++)
        {
            UnityEngine.Object obj = assets[i];
            string assetPath = AssetDatabase.GetAssetPath(obj);
            string extenName = System.IO.Path.GetExtension(assetPath).ToLower();
            if (string.IsNullOrEmpty(extenName) || extenName == ".meta")
            {
                continue;
            }
            GTResourceUnit bundle = new GTResourceUnit();
            switch(extenName)
            {
                case ".prefab":
                    {
                        bundle.AssetName = obj.name;
                        bundle.AssetBundleName = GetParentPathName(assetPath) + ".pre.assetbundle";
                        bundle.Path = assetPath;
                        bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                    }
                    break;
                case ".mp3":
                    {
                        bundle.AssetName = obj.name;
                        bundle.AssetBundleName = obj.name + extenName + ".assetbundle";
                        bundle.Path = assetPath;
                        bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                    }
                    break;
                case ".png":
                    {
                        if (assetPath.Contains("Image"))
                        {
                            bundle.AssetName = obj.name;
                            bundle.AssetBundleName = GetParentPathName(assetPath) + ".atlas.assetbundle";
                            bundle.Path = assetPath;
                            bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                        }
                        if (assetPath.Contains("T_Background"))
                        {
                            bundle.AssetName = obj.name;
                            bundle.AssetBundleName = obj.name + ".tex.assetbundle";
                            bundle.Path = assetPath;
                            bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                        }
                    }
                    break;
                case ".fbx":
                    {
                        bundle.AssetName = obj.name;
                        bundle.AssetBundleName = obj.name + extenName + ".assetbundle";
                        bundle.Path = assetPath;
                        bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                    }
                    break;

            }
            if(string.IsNullOrEmpty(bundle.AssetName))
            {
                continue;
            }
            bundle.AssetBundleName = bundle.AssetBundleName.ToLower();
            list.Add(bundle);
        }

        list.Sort((a1, a2) => { return a1.AssetName.CompareTo(a2.AssetName); });
        Units.Clear();
        foreach (var current in list)
        {
            Units[current.AssetName] = current;
        }
        
        XmlDocument doc = new XmlDocument();
        XmlNode root = doc.CreateElement("root");
        doc.AppendChild(root);
        foreach(var current in list)
        {
            GTResourceUnit bundle = current;
            XmlElement child = doc.CreateElement("row");
            root.AppendChild(child);
            child.SetAttribute("AssetName",       bundle.AssetName);
            child.SetAttribute("AssetBundleName", bundle.AssetBundleName);
            child.SetAttribute("Path",            bundle.Path);
            child.SetAttribute("GUID",            bundle.GUID);
        }

        string fileName = Application.streamingAssetsPath + "/Asset.xml";
        FileStream fs = null;
        if (!File.Exists(fileName))
        {
            fs = File.Create(fileName);
        }
        doc.Save(fileName);
        if (fs != null)
        {
            fs.Flush();
            fs.Dispose();
            fs.Close();
        }
    }

    static void SetBundleNames()
    {
        foreach(var current in Units)
        {
            GTResourceUnit bundle = current.Value;
            var assetImporter = AssetImporter.GetAtPath(bundle.Path);
            if (assetImporter != null)
            {
                assetImporter.assetBundleName = bundle.AssetBundleName.ToLower();
            }
        }
    }

    static void BuildBundles()
    {
        var options = BuildAssetBundleOptions.UncompressedAssetBundle;
        var shouldCheckODR = EditorUserBuildSettings.activeBuildTarget == BuildTarget.iOS;
#if UNITY_TVOS
            shouldCheckODR |= EditorUserBuildSettings.activeBuildTarget == BuildTarget.tvOS;
#endif
        if (shouldCheckODR)
        {
#if ENABLE_IOS_ON_DEMAND_RESOURCES
            if (PlayerSettings.iOS.useOnDemandResources)
                options |= BuildAssetBundleOptions.UncompressedAssetBundle;
#endif
#if ENABLE_IOS_APP_SLICING
            options |= BuildAssetBundleOptions.UncompressedAssetBundle;
#endif
        }
        BuildPipeline.BuildAssetBundles(OUTPUTPATH, BuildAssetBundleOptions.UncompressedAssetBundle,
            EditorUserBuildSettings.activeBuildTarget);
    }

    static string GetParentPathName(string path)
    {
        string folder = path.Replace("/" + Path.GetFileName(path), string.Empty);
        int index = folder.LastIndexOf("/");
        string parentName = string.Empty;
        try
        {
            if (index == folder.Length - 1 || index < 0)
            {
                parentName = folder.Trim('/');
            }
            else
            {
                parentName = folder.Substring(index, folder.Length - index);
                parentName = parentName.Trim('/');
            }
        }
        catch
        {
            Debug.LogError(folder);
        }
        return parentName;
    }
}
