using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

public abstract class DObj<TKey>
{
    public abstract TKey GetKey();
    public abstract void Read(XmlElement element);
}