using UnityEngine;
using System.Collections;
using System.Xml;
using System.IO;
using System.Collections.Generic;
using System;
using UnityEditor;

namespace EDT
{
    public class EditorResourceBuild
    {
        static string OUTPUTPATH = Application.dataPath.Replace("/Assets", string.Empty) + "/AssetBundles";

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
                switch (extenName)
                {
                    case ".xml":
                    case ".txt":
                        {
                            bundle.AssetName = obj.name + extenName;
                            bundle.AssetBundleName = obj.name + extenName + ".assetbundle";
                            bundle.Path = assetPath;
                            bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                        }
                        break;
                    case ".prefab":
                        {
                            bundle.AssetName = obj.name + extenName;
                            bundle.AssetBundleName = GTTools.GetParentPathName(assetPath) + ".pre.assetbundle";
                            bundle.Path = assetPath;
                            bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                        }
                        break;
                    case ".mp3":
                        {
                            bundle.AssetName = obj.name + extenName;
                            bundle.AssetBundleName = obj.name + extenName + ".assetbundle";
                            bundle.Path = assetPath;
                            bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                        }
                        break;
                    case ".png":
                        {
                            if (assetPath.Contains("Image"))
                            {
                                bundle.AssetName = obj.name + extenName;
                                bundle.AssetBundleName = GTTools.GetParentPathName(assetPath) + ".atlas.assetbundle";
                                bundle.Path = assetPath;
                                bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                            }
                            if (assetPath.Contains("T_Background"))
                            {
                                bundle.AssetName = obj.name + extenName;
                                bundle.AssetBundleName = obj.name + ".tex.assetbundle";
                                bundle.GUID = AssetDatabase.AssetPathToGUID(bundle.Path);
                            }
                        }
                        break;

                }
                if (string.IsNullOrEmpty(bundle.AssetName))
                {
                    continue;
                }
                bundle.AssetBundleName = bundle.AssetBundleName.ToLower();
                list.Add(bundle);
            }

            list.Sort((a1, a2) => { return a1.AssetName.CompareTo(a2.AssetName); });
            GTResourceManager.Instance.Units.Clear();
            foreach (var current in list)
            {
                GTResourceManager.Instance.Units[current.Path] = current;
            }

            XmlDocument doc = new XmlDocument();
            XmlNode root = doc.CreateElement("root");
            doc.AppendChild(root);
            foreach (var current in list)
            {
                GTResourceUnit bundle = current;
                XmlElement child = doc.CreateElement("row");
                root.AppendChild(child);
                child.SetAttribute("AssetName", bundle.AssetName);
                child.SetAttribute("AssetBundleName", bundle.AssetBundleName);
                child.SetAttribute("Path", bundle.Path);
                child.SetAttribute("GUID", bundle.GUID);
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
            Dictionary<string, GTResourceUnit> units = GTResourceManager.Instance.Units;
            foreach (var current in units)
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
            BuildPipeline.BuildAssetBundles(OUTPUTPATH, BuildAssetBundleOptions.UncompressedAssetBundle,
                EditorUserBuildSettings.activeBuildTarget);
        }

        public static void Build()
        {
            DelBundles();
            DelBundleNames();
            SetBundleConfig();
            SetBundleNames();
            BuildBundles();
        }
    }
}
