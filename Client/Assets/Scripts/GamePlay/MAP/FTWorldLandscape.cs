using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTWorldLandscape : MonoBehaviour
    {
        public void AddGroup<T>() where T : FTContainer
        {
            T g = new GameObject(typeof(T).Name).AddComponent<T>();
            g.transform.parent = transform;
            g.transform.localPosition = Vector3.zero;
            g.transform.localEulerAngles = Vector3.zero;
            g.transform.localScale = Vector3.one;
        }

        public T    GetGroup<T>()
        {
            return GetComponentInChildren<T>();
        }

        public void AddGroups()
        {
            AddGroup<FTGroupBuilding>();
            AddGroup<FTGroupEffect>();
            AddGroup<FTGroupGrass>();
            AddGroup<FTGroupStone>();
            AddGroup<FTGroupTree>();
        }
    }
}