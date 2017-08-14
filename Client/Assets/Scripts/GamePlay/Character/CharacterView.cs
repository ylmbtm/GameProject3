using UnityEngine;
using System.Collections;

public class CharacterView : MonoBehaviour
{
    [SerializeField] public Character     Owner;
    [SerializeField] public CharacterAttr Attr;

    void Update()
    {
        if  (Owner == null) return;
        this.Owner.Execute();
    }

    void OnDestroy()
    {
        if (Owner == null) return;
        this.Owner.Destroy();
    }

    void OnBecameVisible()
    {

    }

    void OnBecameInvisible()
    {

    }
}