using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class EffectSystem
{
    public Dictionary<int, EffectUnit> Effects = new Dictionary<int, EffectUnit>();

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
            NGUITools.SetLayer(go, GTLayer.LAYER_EFFECT);
        }
        return go;
    }

    public EffectUnit LoadEffect(int id, int guid, KTransform data, Transform parent, bool retain = true)
    {
        EffectUnit tab = new EffectUnit(id, guid, data, parent, retain);
        Effects[tab.GUID] = tab;
        return tab;
    }

    public EffectUnit LoadEffect(int id, int guid, bool retain =true)
    {
        EffectUnit tab = new EffectUnit(id, guid, KTransform.Default, null, retain);
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
