/*
http://www.cgsoso.com/forum-211-1.html

CG搜搜 Unity3d 每日Unity3d插件免费更新 更有VIP资源！

CGSOSO 主打游戏开发，影视设计等CG资源素材。

插件如若商用，请务必官网购买！

daily assets update for try.

U should buy the asset from home store if u use it in your project!
*/

using UnityEngine;
using System.Collections;

namespace HWRWeaponSystem
{
	public class CameraFollower : MonoBehaviour
	{

		public GameObject Target;
		public Vector3 Offset;
	
		void Start ()
		{
	
		}

		void Update ()
		{
		
			if (Target) {
				this.transform.position = Vector3.Lerp (this.transform.position, Target.transform.position + Offset, Time.deltaTime * 10);	
			}
	
		}
	}
}
