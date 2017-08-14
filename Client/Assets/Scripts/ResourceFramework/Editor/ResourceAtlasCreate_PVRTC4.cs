using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;

public class ResourceAtlasCreate_PVRTC4 : ResourceAtlasCreate
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
            SetTextureImporterSettings(path, TextureImporterFormat.PVRTC_RGB4, false);
            Material mat = new Material(Shader.Find("Unlit/Transparent Colored"));
            mat.name = obj.name;
            AssetDatabase.CreateAsset(mat, path.Replace(".png", ".mat"));
            mat.mainTexture = AssetDatabase.LoadAssetAtPath(path, typeof(Texture2D)) as Texture2D;
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
            AssetDatabase.Refresh();
            EditorUtility.ClearProgressBar();
        }
    }
}
