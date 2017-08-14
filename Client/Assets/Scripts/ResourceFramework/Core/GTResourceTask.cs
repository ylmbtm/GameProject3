
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class GTResourceTask
{
    public int                   LoadedDepCount = 0;
    public string                AssetName = null;
    public GTResourceBundle        Bundle = null;
    public GTResourceTask          Parent = null;
    public System.Action<Object> AssetCallback = null;

}