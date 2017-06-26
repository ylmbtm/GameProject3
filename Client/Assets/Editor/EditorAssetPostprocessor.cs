using UnityEngine;
using System.Collections;
using UnityEditor;

public class EditorAssetPostprocessor : AssetPostprocessor
{
    void OnPreprocessTexture()
    {
        var texture = assetImporter as TextureImporter;
        if (assetPath.Contains("Lightmap-") == true)
        {
            texture.mipmapEnabled = false;
            TextureImporterPlatformSettings platformSettings = new TextureImporterPlatformSettings();
            platformSettings.overridden = true;
            platformSettings.maxTextureSize = 256;
            platformSettings.format = TextureImporterFormat.ETC2_RGBA8;
            texture.SetPlatformTextureSettings(platformSettings);
        }
    }
}
