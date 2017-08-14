using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;


public class ResourceAtlasTools : Editor
{
    [MenuItem("图集处理/高清晰")]
    static void A()  
    {    
        ResourceAtlasCreate_RGBA32 ra = new ResourceAtlasCreate_RGBA32();
        ra.CreateUIAtlasPrefab(Selection.activeObject);
    }


    [MenuItem("图集处理/中清晰")]
    static void B_2()
    {
        ResourceAtlasCreate_RGBA16 ra = new ResourceAtlasCreate_RGBA16();
        ra.CreateUIAtlasPrefab(Selection.activeObject);
    }

    [MenuItem("图集处理/低清晰")]
    static void C()
    {
        switch (EditorUserBuildSettings.activeBuildTarget)
        {
            case BuildTarget.Android:
                {
                    ResourceAtlasCreate_ETC1_Alpha ra = new ResourceAtlasCreate_ETC1_Alpha();
                    ra.CreateUIAtlasPrefab(Selection.activeObject);
                }
                break;
            case BuildTarget.iOS:
                {
                    ResourceAtlasCreate_PVRTC4 ra = new ResourceAtlasCreate_PVRTC4();
                    ra.CreateUIAtlasPrefab(Selection.activeObject);
                }
                break;
            case BuildTarget.StandaloneWindows64:
            case BuildTarget.StandaloneWindows:
                {
                    ResourceAtlasCreate_RGBA32 ra = new ResourceAtlasCreate_RGBA32();
                    ra.CreateUIAtlasPrefab(Selection.activeObject);
                }
                break;
        }
    }
}
