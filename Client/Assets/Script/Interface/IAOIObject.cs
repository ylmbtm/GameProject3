using UnityEngine;
using System.Collections;
using Protocol;

public interface IAOIObject
{
    void SyncAOI(XTransform data);
}