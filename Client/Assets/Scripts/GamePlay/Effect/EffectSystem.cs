using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class EffectSystem
{
    public Dictionary<ulong, EffectData> Effects = new Dictionary<ulong, EffectData>();

    public GameObject LoadEffectObj(int id)
    {
        DEffect cfg = ReadCfgEffect.GetDataById(id);
        GameObject go = GTPoolManager.Instance.GetObject(cfg.Path);
        if (go == null)
        {
            Debug.LogError(id);
        }
        else
        {
            NGUITools.SetLayer(go, GTLayer.LAYER_DEFAULT);
        }
        return go;
    }

    public GameObject LoadEffectByPath(string path)
    {
        GameObject go = GTPoolManager.Instance.GetObject(path);
        if (go != null)
        {
            NGUITools.SetLayer(go, GTLayer.LAYER_DEFAULT);
        }
        return go;
    }

    public EffectData LoadEffect(int id, ulong guid, GTTransform data, Transform parent, bool retain = true)
    {
        EffectData tab = new EffectData(id, guid, data, parent, retain);
        Effects[tab.GUID] = tab;
        return tab;
    }

    public EffectData LoadEffect(int id, ulong guid, bool retain = true)
    {
        EffectData tab = new EffectData(id, guid, GTTransform.Default, null, retain);
        Effects[tab.GUID] = tab;
        return tab;
    }

    public void Release()
    {
        foreach (var current in Effects)
        {
            current.Value.Destroy();
        }
        Effects.Clear();
    }
}
