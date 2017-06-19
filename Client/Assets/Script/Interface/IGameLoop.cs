using UnityEngine;
using System.Collections;

public interface IGameLoop
{
    void Execute();
    void Release();
}
