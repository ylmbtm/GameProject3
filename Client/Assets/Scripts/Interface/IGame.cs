using UnityEngine;
using System.Collections;

public interface IGame
{
    void Startup();
    void Execute();
    void Release();
}
