using System;
using System.Collections.Generic;
using UnityEngine;

public class GTPoolManager : GTMonoSingleton<GTPoolManager>
{
    public class CPoolInfo
    {
        public Queue<GameObject> quene = new Queue<GameObject>();
    }

    private Dictionary<string, CPoolInfo> mPoolDict = new Dictionary<string, CPoolInfo>();
    private List<GameObject>              mDestroyPoolGameObjects = new List<GameObject>();//删除队列

    public GameObject GetObject(string path)
    {
        if (string.IsNullOrEmpty(path))
        {
            return null;
        }
        CPoolInfo poolInfo = null;
        GameObject obj = null;
        mPoolDict.TryGetValue(path, out poolInfo);
        if (poolInfo != null)
        {
            if (poolInfo.quene.Count > 0)
            {
                obj = poolInfo.quene.Dequeue();
            }
        }
        if (obj == null)
        {
            obj = GTResourceManager.Instance.Load<GameObject>(path, true);
        }
        if (obj != null)
        {
            obj.transform.parent = null;
            obj.SetActive(true);
        }
        return obj;
    }

    public void       ReleaseGo(string path, GameObject go, bool retain = true)
    {
        if (string.IsNullOrEmpty(path) || go == null)
        {
            return;
        }

        if (retain == false)
        {
            GameObject.Destroy(go);
            return;
        }

        go.transform.parent = transform;
        CPoolInfo poolInfo = null;
        if (!mPoolDict.TryGetValue(path, out poolInfo))
        {
            poolInfo = new CPoolInfo();
            mPoolDict.Add(path, poolInfo);
        }
        go.SetActive(false);
        go.transform.localPosition = new Vector3(100000, 0, 0);
        poolInfo.quene.Enqueue(go);
    }

    public void       Release()
    {
        mPoolDict.Clear();
        mDestroyPoolGameObjects.Clear();
    } 
}