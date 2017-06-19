using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public static class GTShader
{
    static Dictionary<string, Shader> mShaders = new Dictionary<string, Shader>();

    public static Shader ChangeShader(this Material mat, string shaderName)
    {
        if (mat == null || string.IsNullOrEmpty(shaderName))
        {
            return null;
        }
        Shader shader = null;
        if(mShaders.TryGetValue(shaderName, out shader)==false)
        {
            shader = Shader.Find(shaderName);
            if (shader != null)
            {
                mShaders[shaderName] = shader;
            }
        }
        if (shader != null)
        {
            mat.shader = shader;
        }
        return shader;
    }

    public static Shader ChangeShader(this Material mat, Shader shader)
    {
        if (mat == null || shader == null)
        {
            return null;
        }
        mat.shader = shader;
        return shader;
    }
}
