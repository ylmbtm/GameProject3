using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;

public class ResourceAtlasCreate_ETC1_Alpha : ResourceAtlasCreate
{
    public override void CreateUIAtlasPrefab(Object obj)
    {
        string path = AssetDatabase.GetAssetPath(obj);
        if (string.IsNullOrEmpty(path) || !IsTextureFile(path))
        {
            Debug.LogError("未选中对象或者选择的对象不是图片");
            return;
        }
        if (Path.GetExtension(path) == ".png" && !path.Contains("_Alpha") && !path.Contains("_RGB"))
        {
            ShowProgress(path, 0.5f);
            Material mat = new Material(Shader.Find("UI/UI_ETC"));
            mat.name = obj.name;
            AssetDatabase.CreateAsset(mat, path.Replace(".png", ".mat"));
            SetTextureReadable(path);

            SeperateRGBandAlphaChannel(path);

            SetTextureImporterSettings(path, TextureImporterFormat.ETC_RGB4, false);
            Texture2D _mainTex = AssetDatabase.LoadAssetAtPath(path.Replace(".png", "_RGB.png"), typeof(Texture2D)) as Texture2D;
            Texture2D _alphaTex = AssetDatabase.LoadAssetAtPath(path.Replace(".png", "_Alpha.png"), typeof(Texture2D)) as Texture2D;
            mat.SetTexture("_MainTex", _mainTex);
            mat.SetTexture("_MainTex_A", _alphaTex);
            GameObject go = null;
            UIAtlas uiAtlas = null;
            if ((go = AssetDatabase.LoadAssetAtPath(path.Replace(".png", ".prefab"), typeof(GameObject)) as GameObject) != null)
            {
                uiAtlas = SetAtlasInfo(go, path, mat);
            }
            else
            {
                go = new GameObject(obj.name);
                go.AddComponent<UIAtlas>();
                uiAtlas = SetAtlasInfo(go, path, mat);
                CreatePrefab(go, obj.name, path);
            }
            AssetDatabase.SaveAssets();
            EditorUtility.ClearProgressBar();
        }
    }

    /// <summary>
    /// 分离RGB和ALPHA
    /// </summary>
    /// <param name="texturePath"></param>
    public void SeperateRGBandAlphaChannel(string texturePath)
    {
        float sizeScale = 1f;
        Texture2D sourcetex = AssetDatabase.LoadAssetAtPath(texturePath, typeof(Texture2D)) as Texture2D;
        if (!sourcetex)
        {
            Debug.LogError("Load Texture Failed : " + texturePath);
            return;
        }
        Texture2D rgbTex = new Texture2D(sourcetex.width, sourcetex.height, TextureFormat.RGB24, true);
        Texture2D alphaTex = new Texture2D((int)(sourcetex.width * sizeScale), (int)(sourcetex.height * sizeScale), TextureFormat.RGB24, true);
        for (int i = 0; i < sourcetex.width; ++i)
        {
            for (int j = 0; j < sourcetex.height; ++j)
            {
                Color color = sourcetex.GetPixel(i, j);
                Color rgbColor = color;
                Color alphaColor = color;
                alphaColor.r = color.a;
                alphaColor.g = color.a;
                alphaColor.b = color.a;
                rgbTex.SetPixel(i, j, rgbColor);
                alphaTex.SetPixel((int)(i * sizeScale), (int)(j * sizeScale), alphaColor);
            }
        }
        rgbTex.Apply();
        alphaTex.Apply();
        byte[] bytes = rgbTex.EncodeToPNG();
        WritePNG(texturePath.Replace(".png", "_RGB.png"), bytes);
        bytes = alphaTex.EncodeToPNG();
        WritePNG(texturePath.Replace(".png", "_Alpha.png"), bytes);
        Debug.Log("Succeed to seperate RGB and Alpha channel for texture : " + texturePath);
    }

    /// <summary>
    /// 写入图片
    /// </summary>
    /// <param name="path"></param>
    /// <param name="bytes"></param>
    public void WritePNG(string path, byte[] bytes)
    {
        File.WriteAllBytes(path, bytes);
        AssetDatabase.Refresh();
        SetTextureImporterSettings(path, TextureImporterFormat.ETC_RGB4, false);
    }

}
