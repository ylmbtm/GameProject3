using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace MAP
{
    public class FTPathHelper
    {
        public static void PathBezierInterp(float time, List<FTPathNode> paths, out Vector3 p, out Quaternion r, out Vector3 s)
        {
            if (time > paths[paths.Count - 1].Time)
            {
                p = paths[paths.Count - 1].transform.position;
                r = paths[paths.Count - 1].transform.rotation;
                s = paths[paths.Count - 1].transform.localScale;
            }
            else
            {
                if (time < paths[0].Time)
                {
                    p = paths[0].transform.position;
                    r = paths[0].transform.rotation;
                    s = paths[0].transform.localScale;
                }
                else
                {
                    int i;
                    for (i = 0; i < paths.Count - 1; i++)
                    {
                        if (time < paths[i].Time)
                        {
                            break;
                        }
                    }
                    float num = (time - paths[i - 1].Time) / (paths[i].Time - paths[i - 1].Time);
                    float num2 = num * num;
                    float d = num2 * num;
                    float num3 = 1f - num;
                    float num4 = num3 * num3;
                    float d2 = num4 * num3;
                    Vector3 p0 = paths[i - 1].transform.position;
                    Vector3 p1 = paths[i - 1].TangentOut;
                    Vector3 p2 = paths[i].TangentIn;
                    Vector3 p3 = paths[i].transform.position;
                    p = p0 * d2 + p1 * (3f * num * num4) + p2 * (3f * num2 * num3) + p3 * d;
                    r = Quaternion.Slerp(paths[i - 1].transform.rotation, paths[i].transform.rotation, num);
                    s = Vector3.Lerp(paths[i - 1].transform.localScale, paths[i].transform.localScale, num);
                }
            }
        }

        public static void PathLinearInterp(float time, List<FTPathNode> paths, out Vector3 p, out Quaternion r, out Vector3 s)
        {
            if (time > paths[paths.Count - 1].Time)
            {
                p = paths[paths.Count - 1].transform.position;
                r = paths[paths.Count - 1].transform.rotation;
                s = paths[paths.Count - 1].transform.localScale;
            }
            else
            {
                if (time < paths[0].Time)
                {
                    p = paths[0].transform.position;
                    r = paths[0].transform.rotation;
                    s = paths[0].transform.localScale;
                }
                else
                {
                    int i;
                    for (i = 0; i < paths.Count - 1; i++)
                    {
                        if (time < paths[i].Time)
                        {
                            break;
                        }
                    }
                    float t = (time - paths[i - 1].Time) / (paths[i].Time - paths[i - 1].Time);
                    p = Vector3.Lerp(paths[i - 1].transform.position, paths[i].transform.position, t);
                    r = Quaternion.Slerp(paths[i - 1].transform.rotation, paths[i].transform.rotation, t);
                    s = Vector3.Lerp(paths[i - 1].transform.localScale, paths[i].transform.localScale, t);
                }
            }
        }
    }
}