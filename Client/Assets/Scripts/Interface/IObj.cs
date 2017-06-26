using UnityEngine;
using System.Collections;

public interface IObj
{
    int        ID             { get; set; }
    int        GUID           { get; set; }
    Vector3    Pos            { get; set; }
    Vector3    Euler          { get; set; }
    Vector3    Scale          { get; set; }
}
