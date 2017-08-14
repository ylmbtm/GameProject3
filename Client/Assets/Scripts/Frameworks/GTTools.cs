using System;
using System.Collections.Generic;
using UnityEngine;
using System.Text;
using System.Security.Cryptography;
using System.IO;
using System.Reflection;
using System.Collections;
using System.Linq;

public static class GTTools
{
    static StringBuilder m_StringBuilder = new StringBuilder();
    const  float         m_Eps = 1E-06f;
    static char[]        m_SeParator = new char[3] { '(', ',', ')' };
    static Color[]       m_RandomColors = new Color[6]
    {
        Color.blue,
        Color.yellow,
        Color.red,
        Color.cyan,
        Color.green,
        Color.magenta
    };

    public enum Tags
    {
        MainCamera,
        UI,
        Player,
        Effect,
        Trail,
    }

    public static string ToTimer(int value)
    {
        if (value > 9)
        {
            return value.ToString();
        }
        else
        {
            return string.Format("0{0}", value);
        }
    }

    public static string SecondsToTimer(int seconds)
    {
        if (seconds < 0)
        {
            return "00:00:00";
        }
        int h = seconds / 3600;
        int m = (seconds - h * 3600) / 60;
        int s = seconds % 60;
        return string.Format("{0}:{1}:{2}", ToTimer(h), ToTimer(m), ToTimer(s));
    }

    public static string ShowPrice(int price, int num)
    {
        if (price > num)
        {
            return string.Format("[ff0000]{0}[-]/{1}", price, num);
        }
        else
        {
            return string.Format("[00ff00]{0}[-]/{1}", price, num);
        }
    }

    public static string Format(string format, object arg0)
    {
        m_StringBuilder.Remove(0, m_StringBuilder.Length);
        m_StringBuilder.AppendFormat(format, arg0);
        return m_StringBuilder.ToString();
    }

    public static string Format(string format, object arg0, object arg1)
    {
        m_StringBuilder.Remove(0, m_StringBuilder.Length);
        m_StringBuilder.AppendFormat(format, arg0, arg1);
        return m_StringBuilder.ToString();
    }

    public static string Format(string format, object arg0, object arg1, object arg2)
    {
        m_StringBuilder.Remove(0, m_StringBuilder.Length);
        m_StringBuilder.AppendFormat(format, arg0, arg1, arg2);
        return m_StringBuilder.ToString();
    }

    public static string Format(string format, object arg0, object arg1, object arg2, object arg3)
    {
        m_StringBuilder.Remove(0, m_StringBuilder.Length);
        m_StringBuilder.AppendFormat(format, arg0, arg1, arg2, arg3);
        return m_StringBuilder.ToString();
    }

    public static string ToPercent(this float value, int num = 0)
    {
        string format = string.Format("{0}{1}{2}", "{0:P", num, "}");
        return string.Format(format, value);
    }

    public static string ToPercent(this int value, int num = 0)
    {
        string format = string.Format("{0}{1}{2}", "{0:P", num, "}");
        return string.Format(format, value);
    }

    public static string ToPoint(this float value, int num = 2)
    {
        string format = string.Format("{0}{1}{2}", "{0:F", num, "}");
        return string.Format(format, value);
    }

    public static string ToPoint(this int value, int num = 2)
    {
        string format = string.Format("{0}{1}{2}", "{0:F", num, "}");
        return string.Format(format, value);
    }

    public static string GetMd5Hash(string str)
    {
        MD5 md5Hasher = MD5.Create();
        byte[] data = md5Hasher.ComputeHash(Encoding.Default.GetBytes(str));
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < data.Length; i++)
        {
            sb.Append(data[i].ToString("x2"));
        }
        return sb.ToString();
    }

    public static string SaveVector3(Vector3 pos)
    {
        return Format("({0},{1},{2})", pos.x.ToString("0.00"), pos.y.ToString("0.00"), pos.z.ToString("0.00"));
    }

    public static string Join<T>(this string s, IEnumerable<T> collection)
    {
        IEnumerator<T> enumerator = collection.GetEnumerator();
        if (!enumerator.MoveNext())
        {
            return string.Empty;
        }
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.Append(enumerator.Current);
        while (enumerator.MoveNext())
        {
            stringBuilder.Append(s);
            stringBuilder.Append(enumerator.Current);
        }
        return stringBuilder.ToString();
    }

    public static string Join<T>(this string s, IEnumerable<T> collection, Func<T, string> func)
    {
        IEnumerator<T> enumerator = collection.GetEnumerator();
        if (!enumerator.MoveNext())
        {
            return string.Empty;
        }
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.Append(func(enumerator.Current));
        while (enumerator.MoveNext())
        {
            stringBuilder.Append(s);
            stringBuilder.Append(func(enumerator.Current));
        }
        return stringBuilder.ToString();
    }

    public static string JoinParams(this string s, params string[] args)
    {
        return s.Join(args);
    }

    public static string GetParentPathName(string path)
    {
        string folder = path.Replace("/" + Path.GetFileName(path), string.Empty);
        int index = folder.LastIndexOf("/");
        string parentName = string.Empty;
        try
        {
            if (index == folder.Length - 1 || index < 0)
            {
                parentName = folder.Trim('/');
            }
            else
            {
                parentName = folder.Substring(index, folder.Length - index);
                parentName = parentName.Trim('/');
            }
        }
        catch
        {
            Debug.LogError(folder);
        }
        return parentName;
    }

    public static int Copy(object destination, object source)
    {
        if (destination == null || source == null)
        {
            return 0;
        }
        return GTTools.Copy(destination, source, source.GetType());
    }

    public static int Copy(object destination, object source, Type type)
    {
        return GTTools.Copy(destination, source, type, null);
    }

    public static int Copy(object destination, object source, Type type, IEnumerable<string> excludeName)
    {
        if (destination == null || source == null)
        {
            return 0;
        }
        if (excludeName == null)
        {
            excludeName = new List<string>();
        }
        int num = 0;
        Type type2 = destination.GetType();
        FieldInfo[] fields = type.GetFields();
        for (int i = 0; i < fields.Length; i++)
        {
            FieldInfo fieldInfo = fields[i];
            if (!excludeName.Contains(fieldInfo.Name))
            {
                try
                {
                    FieldInfo field = type2.GetField(fieldInfo.Name);
                    if (field != null && field.FieldType == fieldInfo.FieldType)
                    {
                        field.SetValue(destination, fieldInfo.GetValue(source));
                        num++;
                    }
                }
                catch
                {
                }
            }
        }
        PropertyInfo[] properties = type.GetProperties();
        for (int j = 0; j < properties.Length; j++)
        {
            PropertyInfo propertyInfo = properties[j];
            if (!excludeName.Contains(propertyInfo.Name))
            {
                try
                {
                    PropertyInfo property = type2.GetProperty(propertyInfo.Name);
                    if (property != null && property.PropertyType == propertyInfo.PropertyType && property.CanWrite && propertyInfo.CanRead)
                    {
                        property.SetValue(destination, propertyInfo.GetValue(source, null), null);
                        num++;
                    }
                }
                catch
                {
                }
            }
        }
        return num;
    }

    public static Int16 ToInt16(this string value)
    {
        Int16 v = 0;
        if (Int16.TryParse(value, out v) == false)
        {
            Debug.LogError(string.Format("value = {0} can not convert to int16", value));
        }
        return v;
    }

    public static Int32 ToInt32(this string value)
    {
        int v = 0;
        if (int.TryParse(value, out v) == false)
        {
            Debug.LogError(string.Format("value = {0} can not convert to int", value));
        }
        return v;
    }

    public static Int64 ToInt64(this string value)
    {
        Int64 v = 0;
        if (Int64.TryParse(value, out v) == false)
        {
            Debug.LogError(string.Format("value = {0} can not convert to int64", value));
        }
        return v;
    }

    public static UInt16 ToUInt16(this string value)
    {
        UInt16 v = 0;
        if (UInt16.TryParse(value, out v) == false)
        {
            Debug.LogError(string.Format("value = {0} can not convert to uint16", value));
        }
        return v;
    }

    public static UInt32 ToUInt32(this string value)
    {
        UInt32 v = 0;
        if (UInt32.TryParse(value, out v) == false)
        {
            Debug.LogError(string.Format("value = {0} can not convert to uint32", value));
        }
        return v;
    }

    public static UInt64 ToUInt64(this string value)
    {
        UInt64 v = 0;
        if (UInt64.TryParse(value, out v) == false)
        {
            Debug.LogError(string.Format("value = {0} can not convert to uint64", value));
        }
        return v;
    }

    public static bool ToBool(this string value)
    {
        string s = value.ToLower();
        return s.Equals("true");
    }

    public static bool GetValueFromBitMark(uint value, int index)
    {
        return (value >> index) % 2 == 1;
    }

    public static bool GetValueFromBitMark(int value, int index)
    {
        return (value >> index) % 2 == 1;
    }

    public static bool CheckDistance(Transform src, Transform tar, float dis)
    {
        if (src == null || tar == null)
        {
            return false;
        }
        return GetHorizontalDistance(src.position, tar.position) < dis;
    }

    public static bool IsArrive(UnityEngine.AI.NavMeshAgent agent)
    {
        if (agent)
        {
            if (agent.enabled && agent.pathPending == false)
            {
                return agent.remainingDistance <= agent.stoppingDistance;
            }
        }
        return true;
    }

    public static bool IsTrigger(float ratio)
    {
        float r = UnityEngine.Random.Range(0, 1f);
        return r < ratio ? true : false;
    }

    public static bool IsTrigger(int ratio)
    {
        int r = UnityEngine.Random.Range(0, 100);
        return r < ratio ? true : false;
    }

    public static bool IsLineOfSight(Transform src, Transform tar, Vector3 direction)
    {
        RaycastHit hit;
        if (Physics.Linecast(tar.position, direction.normalized, out hit))
        {
            if (hit.transform.Equals(tar.position))
            {
                return true;
            }
        }
        return false;
    }

    public static bool IsCircleOfSight(Transform src, Transform tar, float radius)
    {
        float num = GetHorizontalDistance(src.position, tar.position);
        return num < radius;
    }

    public static bool SaveRenderTexture(this Camera cam, string filename)
    {
        RenderTexture rt = cam.targetTexture;
        if (rt == null)
        {
            rt = new RenderTexture(1024, 1024, 0);
            rt.antiAliasing = 8;
            rt.autoGenerateMips = false;
            rt.format = RenderTextureFormat.ARGB32;
            rt.filterMode = FilterMode.Trilinear;
            rt.anisoLevel = 4;
        }
        cam.targetTexture = rt;
        cam.Render();

        RenderTexture.active = rt;
        Texture2D tex = new Texture2D(rt.width, rt.height, TextureFormat.ARGB32, false);
        tex.ReadPixels(new Rect(0f, 0f, rt.width, rt.height), 0, 0);
        tex.Apply();

        cam.targetTexture = null;
        RenderTexture.active = null;
        NGUITools.DestroyImmediate(rt);
        try
        {
            byte[] bytes = tex.EncodeToPNG();
            System.IO.File.WriteAllBytes(filename, bytes);
        }
        catch (System.Exception ex)
        {
            Debug.LogError(ex.Message);
            return false;
        }
        finally
        {
            NGUITools.DestroyImmediate(tex);
        }
        return true;
    }

    public static bool IsNullOrBeDestroyed(this GameObject go)
    {
        return go == null || go.transform == null;
    }

    public static bool IsZero(this float delta)
    {
        return delta < 1E-06f && delta > -1E-06f;
    }

    public static bool IsEqual(this float lhs, float rhs)
    {
        float num = lhs - rhs;
        return num < 1E-06f && num > -1E-06f;
    }

    public static bool IsNumber(this string s)
    {
        if (s.Length == 0)
        {
            return false;
        }
        for (int i = 0; i < s.Length; i++)
        {
            if ((s[i] < '0' || s[i] > '9') && (s[i] != '.' || i == s.Length - 1 || s.IndexOf('.', i + 1) != -1) && (s[i] != '-' || i != 0))
            {
                return false;
            }
        }
        return true;
    }

    public static void Encode(object obj)
    {
        FieldInfo[] fields = obj.GetType().GetFields();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < fields.Length; i++)
        {
            FieldInfo field = fields[i];
            string fieldName = field.Name;
            object val = field.GetValue(obj);
            string fieldValue = (val == null) ? string.Empty : val.ToString();
            sb.AppendFormat("{0}={1},", fieldName, fieldValue);
        }
        Debug.Log(sb.ToString());
    }

    public static void Append(this UILabel lab,string arg)
    {
        if (lab == null) return;
        if(lab.text==string.Empty)
        {
            lab.text = arg;
        }
        else
        {
            lab.text = Format("{0}{1}{2}", lab.text, '\n',  arg);
        }       
    }

    public static void ResetLocalTransform(this Transform trans, Transform parent)
    {
        trans.parent = parent;
        trans.localPosition = Vector3.zero;
        trans.localRotation = Quaternion.identity;
        trans.localScale = Vector3.one;
    }

    public static void OpenOrCloseNavMeshLayerMask(GameObject go, int layer, bool open)
    {
        if (go == null) return;
        UnityEngine.AI.NavMeshAgent[] navs = go.GetComponentsInChildren<UnityEngine.AI.NavMeshAgent>();
        for (int i = 0; i < navs.Length; i++)
        {
            if (open)
            {
                navs[i].areaMask |= layer;
            }
            else
            {
                navs[i].areaMask &= ~layer;
            }
        }
    }

    public static void CreateFile(string path, string name, string info)
    {
        StreamWriter sw = null;
        FileInfo fileInfo = new FileInfo(path + "//" + name);
        sw = fileInfo.CreateText();
        sw.WriteLine(info);
        sw.Close();
        sw.Dispose();
    }

    public static void CreateFile(string path, string name, List<string> info)
    {
        StreamWriter sw = null;
        FileInfo fileInfo = new FileInfo(path + "//" + name);
        sw = fileInfo.CreateText();
        for (int i = 0; i < info.Count; i++)
        {
            sw.WriteLine(info[i]);
        }
        sw.Close();
        sw.Dispose();
    }

    public static void DeleteFile(string path, string name)
    {
        File.Delete(path + "//" + name);
    }

    public static void SetParticleSystemScale(this GameObject go, float scale)
    {
        foreach (Transform transform in go.transform)
        {
            transform.gameObject.SetParticleSystemScale(scale);
        }
        if (go.GetComponent<ParticleSystem>() != null)
        {
            go.GetComponent<ParticleSystem>().startSize *= scale;
            go.GetComponent<ParticleSystem>().startSpeed *= scale;
        }
    }

    public static void SetParticleSystemVisible(this GameObject go, bool bVisible)
    {
        foreach (Transform transform in go.transform)
        {
            transform.gameObject.SetParticleSystemVisible(bVisible);
        }
        if (go.GetComponent<ParticleSystem>() != null)
        {
            if (bVisible)
            {
                go.GetComponent<ParticleSystem>().Play();
            }
            else
            {
                go.GetComponent<ParticleSystem>().Stop();
            }
        }
    }

    public static void FindAllChildren(Transform trans, List<Transform> pList)
    {
        for (int i = 0; i < trans.childCount; i++)
        {
            pList.Add(trans.GetChild(i));
            FindAllChildren(trans.GetChild(i), pList);
        }
    }

    public static void ForeachChild(this Transform trans, Callback<Transform> callback, bool ignoreHide = false)
    {
        if (trans == null || callback == null)
        {
            return;
        }
        for (int i = 0; i < trans.childCount; i++)
        {
            callback(trans.GetChild(i));
        }
    }

    public static void Sample(this Animator animator, float time)
    {
        if (animator != null)
        {
            animator.playbackTime = time;
            animator.Update(0f);
        }
    }

    public static void SetTag(GameObject go, Tags tag)
    {
        if (go != null)
        {
            go.tag = tag.ToString();
        }
    }

    public static void GetDirs(string dirPath, ref List<string> list)
    {
        foreach (string path in Directory.GetFiles(dirPath))
        {
            list.Add(path);
        }
        if (Directory.GetDirectories(dirPath).Length == 0)
        {
            return;
        }
        foreach (string path in Directory.GetDirectories(dirPath))
        {
            GetDirs(path, ref list);
        }
    }

    public static long GetUtcTime()
    {
        long time = 0;
        DateTime dt = new DateTime(1970, 1, 1, 0, 0, 0);
        TimeSpan ts = DateTime.UtcNow - dt;
        time = ts.Ticks / TimeSpan.TicksPerSecond;
        return time;
    }

    public static long Sum(this IEnumerable<int> collection)
    {
        long num = 0L;
        foreach (int current in collection)
        {
            num += (long)current;
        }
        return num;
    }

    public static long Sum(this IEnumerable<long> collection)
    {
        long num = 0L;
        foreach (long current in collection)
        {
            num += current;
        }
        return num;
    }

    public static long Sum(this IEnumerable<short> collection)
    {
        long num = 0L;
        foreach (short current in collection)
        {
            num += (long)current;
        }
        return num;
    }

    public static uint ToInt32(this Color color)
    {
        uint num = (uint)(color.a * 255f);
        uint num2 = (uint)(color.r * 255f);
        uint num3 = (uint)(color.g * 255f);
        uint num4 = (uint)(color.b * 255f);
        return num << 24 | num2 << 16 | num3 << 8 | num4;
    }

    public static byte LByte8(ushort usValue)
    {
        return ((byte)(((ulong)(usValue)) & 0xff));
    }

    public static byte HByte8(ushort usValue)
    {
        return ((byte)((((ulong)(usValue)) >> 8) & 0xff));
    }

    public static Vector2 ToVector2(this string value)
    {
        if (string.IsNullOrEmpty(value))
        {
            return Vector2.zero;
        }
        string[] array = value.Split(m_SeParator);
        return new Vector2(array[1].ToFloat(), array[2].ToFloat());
    }

    public static Vector3 ToVector3(this string value, bool useSeparatorArray = false)
    {
        if (string.IsNullOrEmpty(value))
        {
            return Vector3.zero;
        }

        if (useSeparatorArray == false)
        {
            string[] array = value.Split(',');
            return new Vector3(array[0].ToFloat(), array[1].ToFloat(), array[2].ToFloat());
        }
        else
        {
            string[] array = value.Split(m_SeParator);
            return new Vector3(array[1].ToFloat(), array[2].ToFloat(), array[3].ToFloat());
        }
    }

    public static Vector3 NavSamplePosition(Vector3 srcPosition)
    {
        Vector3 dstPosition = srcPosition;
        UnityEngine.AI.NavMeshHit meshHit = new UnityEngine.AI.NavMeshHit();
        int layer = 1<<UnityEngine.AI.NavMesh.GetAreaFromName("Walkable");
        if(UnityEngine.AI.NavMesh.SamplePosition(srcPosition,out meshHit,100,layer))
        {
            dstPosition = meshHit.position;
        }
        return dstPosition;
    }

    public static Vector3 GroundPosition(Vector3 srcPosition, int terrainLayer)
    {
        Ray ray = new Ray(srcPosition + Vector3.up * 20, Vector3.down);
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit, 100, 1 << terrainLayer))
        {
            return hit.point;
        }
        return Vector3.zero;
    }

    public static Vector3 RandomOnCircle(float radius)
    {
        Quaternion qot = Quaternion.Euler(Vector3.up * UnityEngine.Random.Range(0, 360f));
        return qot * Vector3.forward * radius;
    }

    public static Vector3 RandomOnCircle(Vector3 center, float min, float max)
    {
        Vector3 randomPoint = UnityEngine.Random.insideUnitCircle * max;
        randomPoint += randomPoint.normalized;
        center += randomPoint;
        UnityEngine.AI.NavMeshHit meshHit = new UnityEngine.AI.NavMeshHit();
        int layer = 1 << UnityEngine.AI.NavMesh.GetAreaFromName("Walkable");
        if (UnityEngine.AI.NavMesh.SamplePosition(center, out meshHit, 100, layer))
        {
             return meshHit.position;
        }
        return center;
    }

    public static Vector3 GetRayPosition(Vector3 srcPosition, Vector3 dstPosition, float fDistance)
    {
        Vector3 dir = (dstPosition - srcPosition).normalized;
        return srcPosition + dir * fDistance;
    }

    public static Vector3 Random(Vector3 center, float strength)
    {
        float x = UnityEngine.Random.Range(-strength, strength);
        float y = UnityEngine.Random.Range(-strength, strength);
        float z = UnityEngine.Random.Range(-strength, strength);
        return center + new Vector3(x, y, z);
    }

    public static Vector3[] GetEvenPositionOnCircle(float radius, int num, Vector3 targetPos)
    {
        if (radius <= 0 || num <= 0)
        {
            return null;
        }
        Vector3[] array = new Vector3[num];
        for (int i = 0; i < num; i++)
        {
            float angle = ((float)i / num) * (float)Math.PI * 2;
            Vector3 pos = new Vector3((float)Math.Cos(angle), 0, (float)Math.Sin(angle)) * radius + targetPos;
            array[i] = pos;
        }
        return array;
    }

    public static Transform GetBone(Transform trans, string boneName)
    {
        if (string.IsNullOrEmpty(boneName))
        {
            return null;
        }
        Transform[] tran = trans.GetComponentsInChildren<Transform>(true);
        foreach (Transform t in tran)
        {
            if (t.name == boneName)
            {
                return t;
            }
        };
        return null;
    }

    public static ArrayList LoadFile(string path, string name)
    {
        StreamReader sr = null;
        try
        {
            sr = File.OpenText(path + "//" + name);
        }
        catch (Exception e)
        {
            return null;
        }
        string line;
        ArrayList list = new ArrayList();
        while ((line = sr.ReadLine()) != null)
        {
            list.Add(line);
        }
        sr.Close();
        sr.Dispose();
        return list;
    }

    public static GameObject Clone(this GameObject prefab)
    {
        if (null == prefab)
        {
            return null;
        }
        GameObject gameObject = UnityEngine.Object.Instantiate<GameObject>(prefab);
        Transform transform = prefab.transform;
        gameObject.transform.parent = transform.parent;
        gameObject.transform.localScale = transform.localScale;
        return gameObject;
    }

    public static GameObject ReplacedByPrefab(this GameObject go, GameObject prefab)
    {
        if (null == go || null == prefab)
        {
            return null;
        }
        GameObject gameObject = UnityEngine.Object.Instantiate<GameObject>(prefab);
        Transform transform = go.transform;
        gameObject.transform.parent = transform.parent;
        gameObject.transform.localScale = transform.localScale;
        UnityEngine.Object.Destroy(go);
        return gameObject;
    }

    public static GameObject GetRootObject(this GameObject go)
    {
        if (go == null)
        {
            return null;
        }
        Transform transform = go.transform;
        while (transform.parent != null)
        {
            transform = transform.parent;
        }
        return transform.gameObject;
    }

    public static Animator LoadAnimator(GameObject model,string ctrlPath)
    {
        if(model==null||string.IsNullOrEmpty(ctrlPath))
        {
            return null;
        }
        GameObject ctrl = GTResourceManager.Instance.Load<GameObject>(ctrlPath);
        if(ctrl==null)
        {
            return null;
        }
        RuntimeAnimatorController rc = null;
        Animator sourceAnimator = ctrl.GetComponent<Animator>();
        Animator animator = model.GET<Animator>();
        if (!sourceAnimator)
        {
            return animator;
        }
        rc = ctrl.GetComponent<Animator>().runtimeAnimatorController;
        animator.runtimeAnimatorController = rc;
        animator.avatar = sourceAnimator.avatar;
        return animator;
    }

    public static float Average(this IEnumerable<float> collection)
    {
        int num = 0;
        foreach (float num2 in collection)
        {
            num++;
        }
        if (num == 0)
        {
            return 0f;
        }
        return (float)(collection.Sum() / (double)num);
    }

    public static float Bake(this Animator animator, string action)
    {
        if (!string.IsNullOrEmpty(action))
        {
            animator.Rebind();
            animator.StopPlayback();
            animator.recorderStartTime = 0f;
            animator.Play(action);
            AnimatorStateInfo currentAnimatorStateInfo = animator.GetCurrentAnimatorStateInfo(0);
            animator.StartRecording(0);
            while (currentAnimatorStateInfo.normalizedTime < 1f)
            {
                animator.Update(0.0333333351f);
                currentAnimatorStateInfo = animator.GetCurrentAnimatorStateInfo(0);
                if (currentAnimatorStateInfo.length <= 0f)
                {
                    return 0f;
                }
            }
            animator.StopRecording();
            animator.StartPlayback();
            return currentAnimatorStateInfo.length;
        }
        return 0f;
    }

    public static float BakeCurrent(this Animator animator)
    {
        animator.Rebind();
        animator.StopPlayback();
        animator.recorderStartTime = 0f;
        AnimatorStateInfo currentAnimatorStateInfo = animator.GetCurrentAnimatorStateInfo(0);
        animator.StartRecording(0);
        while (currentAnimatorStateInfo.normalizedTime < 1f)
        {
            animator.Update(0.0333333351f);
            currentAnimatorStateInfo = animator.GetCurrentAnimatorStateInfo(0);
        }
        animator.StopRecording();
        animator.StartPlayback();
        return currentAnimatorStateInfo.length;
    }

    public static float GetCurrentStateLength(this Animator animator)
    {
        if (animator != null)
        {
            return animator.GetCurrentAnimatorStateInfo(0).length;
        }
        return 0f;
    }

    public static float GetHorizontalDistance(Vector3 a, Vector3 b)
    {
        Vector3 v1 = a;
        v1.y = 0;
        Vector3 v2 = b;
        v2.y = 0;
        return Vector3.Distance(v1, v2);
    }

    public static float ToFloat(this string value)
    {
        float v = 0;
        if (float.TryParse(value, out v) == false)
        {
            Debug.LogFormat("value:{0} can not convert to float", value);
        }
        return v;
    }

    public static Color ToColor3(this int color)
    {
        float r = (float)(color >> 16) / 255f;
        float g = (float)(color >> 8 & 255) / 255f;
        float b = (float)(color & 255) / 255f;
        return new Color(r, g, b);
    }

    public static Color ToColor4(this int color)
    {
        float a = (float)(color >> 24) / 255f;
        float r = (float)(color >> 16 & 255) / 255f;
        float g = (float)(color >> 8 & 255) / 255f;
        float b = (float)(color & 255) / 255f;
        return new Color(r, g, b, a);
    }

    public static Color GetRandomColor()
    {
        float r = UnityEngine.Random.Range(0, 1f);
        float g = UnityEngine.Random.Range(0, 1f);
        float b = UnityEngine.Random.Range(0, 1f);
        return new Color(r, g, b);
    }

    public static Color GetRandomColorFromColorList(int index)
    {
        int i = index % m_RandomColors.Length;
        return m_RandomColors[i];
    }

    public static RenderTexture Realloc(this RenderTexture texture)
    {
        return texture.Realloc(Screen.width, Screen.height);
    }

    public static RenderTexture Realloc(this RenderTexture texture, int width, int height)
    {
        if (null == texture || texture.width != width || texture.height != height)
        {
            UnityEngine.Object.DestroyImmediate(texture);
            texture = new RenderTexture(width, height, 16);
            texture.hideFlags = HideFlags.HideAndDontSave;
        }
        return texture;
    }

    public static List<Type> GetSubTypes(object obj)
    {
        Assembly assembly = Assembly.GetAssembly(obj.GetType());
        if (assembly == null)
        {
            return null;
        }
        Type[] types = assembly.GetTypes();
        if (types == null)
        {
            return null;
        }
        List<Type> holderTypes = new List<Type>();
        for (int i = 0; i < types.Length; i++)
        {
            if (types[i].IsSubclassOf(obj.GetType()))
            {

                holderTypes.Add(types[i]);
            }
        }
        return holderTypes;
    }

    public static Assembly[] GetAssemblies()
    {
        return AppDomain.CurrentDomain.GetAssemblies();
    }

    public static V          GetValue<T, V>(this Dictionary<T, V> dict, T key)
    {
        V v = default(V);
        dict.TryGetValue(key, out v);
        return v;
    }

    public static T          GET<T>(this GameObject go) where T : UnityEngine.Component
    {
        if (go == null) return null;
        T t = go.GetComponent<T>();
        if (t == null)
        {
            t = go.AddComponent<T>();
        }
        return t;
    }

    public static T[]        GetCustomAttributes<T>(Type type, bool inherited) where T : Attribute
    {
        return (T[])type.GetCustomAttributes(typeof(T), inherited);
    }

    public static TAtr       GetAttribute<TAtr>(Type type) where TAtr : Attribute
    {
        TAtr[] attrs = (TAtr[])type.GetCustomAttributes(typeof(TAtr), false);
        if (attrs != null && attrs.Length > 0)
        {
            TAtr attr = attrs[0];
            return attr;
        }
        return null;
    }

    public static List<Type> GetAllSubTypes(Type parentType)
    {
        List<Type> list = new List<Type>();
        foreach (Assembly a in System.AppDomain.CurrentDomain.GetAssemblies())
        {
            foreach (System.Type type in a.GetTypes())
            {
                if (type.IsSubclassOf(parentType))
                {
                    list.Add(type);
                }
            }
        }
        return list;
    }

    public static List<T>    RandomListFromBigList<T>(List<T> pList, int randomNum)
    {
        int num = randomNum >= pList.Count ? pList.Count - 1 : randomNum;
        System.Random random = new System.Random(unchecked((int)DateTime.Now.Ticks));
        List<T> list = new List<T>(num);
        for (int i = 0; i < num; i++)
        {
            int r = random.Next(1, pList.Count);
            T temp = pList[r];
            if (list.Contains(temp))
            {
                --i;
                continue;
            }
            else
            {
                list.Add(temp);
            }
        }
        return list;
    }

    public static void       GetAllComponents<T>(Transform trans, List<T> pList) where T : Component
    {
        if (trans == null) return;
        for (int i = 0; i < trans.childCount; i++)
        {
            T t = trans.GetChild(i).GetComponent<T>();
            if (t != null)
            {
                pList.Add(t);
            }
        }
    }

    public static List<T>    GetListFromEnumNames<T>(Type enumType) 
    {
        List<T> list = new List<T>();
        for (int i = 0; i < Enum.GetNames(enumType).Length; i++)
        {
            list.Add(default(T));
        }
        return list;
    }
}