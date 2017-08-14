using UnityEngine;
using System.Collections;
using UnityEditor;
using System;
using System.IO;
using System.Diagnostics;

class ExportFormate
{
    public static string BINARY = "0";
    public static string ASCII = "1";
}

public class ResourceAnimOptimize
{
    private static int number = 0;
    static string ExePath = "F:/图集压缩方案/Unity骨骼动画压缩工具使用介绍/FBX/FBXCompress.exe";
    static DateTime time;

    [MenuItem("优化/压缩动画文件")]
    static void Execute()
    {
        number = 0;
        time = DateTime.Now;
        foreach (UnityEngine.Object o in Selection.GetFiltered(typeof(AnimationClip), SelectionMode.DeepAssets))
        {
            number++;
            ForFun((GameObject.Instantiate(o) as AnimationClip), EditorUtility.GetAssetPath(o));
        }
        AssetDatabase.SaveAssets();
        Log("一共压缩了" + number + "个动画文件!");
        Log("耗时:" + ((DateTime.Now - time).TotalMilliseconds / 1000) + "秒.");
    }

    [MenuItem("优化/压缩FBX")]
    static void ExecuteFBX()
    {
        string RootPath = Application.dataPath;
        RootPath = RootPath.Substring(0, RootPath.LastIndexOf("/")) + "/";
        Directory.CreateDirectory("C:/FBXCompress/");
        StreamWriter writer = File.CreateText("C:/FBXCompress/FBX.txt");
        foreach (UnityEngine.Object o in Selection.GetFiltered(typeof(UnityEngine.Object), SelectionMode.DeepAssets))
        {
            string fbxPath = RootPath + AssetDatabase.GetAssetPath(o);
            if (fbxPath.Contains(".FBX") || fbxPath.Contains(".fbx"))
            {
                writer.WriteLine(fbxPath);
            }
        }
        writer.Flush();
        writer.Close();
        Process.Start(ExePath, ExportFormate.BINARY);
    }

    static void ForFun(AnimationClip clip, string clipName)
    {
        AnimationClipCurveData[] curves = AnimationUtility.GetAllCurves(clip);
        Keyframe key;
        Keyframe[] keyFrames;
        foreach (AnimationClipCurveData curveDate in curves)
        {
            keyFrames = curveDate.curve.keys;
            for (int i = 0; i < keyFrames.Length; i++)
            {
                key = keyFrames[i];
                key.value = float.Parse(key.value.ToString("f3"));
                key.inTangent = float.Parse(key.inTangent.ToString("f3"));
                key.outTangent = float.Parse(key.outTangent.ToString("f3"));
                keyFrames[i] = key;
            }
            curveDate.curve.keys = keyFrames;
            clip.SetCurve(curveDate.path, curveDate.type, curveDate.propertyName, curveDate.curve);
        }
        AssetDatabase.CreateAsset(clip, clipName);
    }

    static void Log(string content)
    {
        UnityEngine.Debug.Log(content);
    }
}
