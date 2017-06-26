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
	public class ObjectPool : MonoBehaviour
	{
		[HideInInspector]
		public bool Active;
		[HideInInspector]
		public GameObject Prefab;
		public float LifeTime;
		private Vector3 positionTemp;
		private Quaternion rotationTemp;
		private Vector3 scaleTemp;
		private Rigidbody rigidBody;
		private LineRenderer lineRenderer;
		private TrailRenderer[] trailRenderers;
		private float[] trailTemp;

		void Awake ()
		{
			scaleTemp = this.transform.localScale;
			positionTemp = this.transform.position;
			rotationTemp = this.transform.rotation;
			rigidBody = this.GetComponent<Rigidbody> ();
			lineRenderer = this.GetComponent<LineRenderer> ();

			trailRenderers = (TrailRenderer[])GetComponentsInChildren<TrailRenderer> ();
			trailTemp = new float[trailRenderers.Length];
			for (int i = 0; i < trailRenderers.Length; i++) {
				trailTemp [i] = trailRenderers [i].time;
			}
		}

		void TrailEnable (bool enabled)
		{
			
		}

		void Start ()
		{
		
		}

		void OnEnable ()
		{
			if (LifeTime > 0) {
				StartCoroutine (setDestrying (LifeTime));
			}
		}

		public virtual void OnSpawn (Vector3 position, Vector3 scale, Quaternion rotation, GameObject prefab, float lifeTime)
		{
			if (lifeTime != -1)
				LifeTime = lifeTime;
		
			if (GetComponent<Renderer> ())
				GetComponent<Renderer> ().enabled = true;

			Prefab = prefab;
			this.transform.position = position;
			this.transform.rotation = rotation;
			this.transform.localScale = scale;
			scaleTemp = this.transform.localScale;
			positionTemp = this.transform.position;
			rotationTemp = this.transform.rotation;
		
			if (rigidBody) {
				rigidBody.velocity = Vector3.zero;
				rigidBody.angularVelocity = Vector3.zero;
			}
			if (lineRenderer) {
				lineRenderer.SetPosition (0, this.transform.position);
				lineRenderer.SetPosition (1, this.transform.position);
			}
			if (GetComponent<ParticleSystem> ()) {
				GetComponent<ParticleSystem> ().Play ();
			}

			for (int i = 0; i < trailRenderers.Length; i++) {
				trailRenderers [i].time = trailTemp [i];
			}
		
			Active = true;
		
			this.gameObject.SetActive (true);
		
			if (LifeTime > 0) {
				StartCoroutine (setDestrying (LifeTime));
			}

		}

	

		public IEnumerator setDestrying (float time)
		{
			yield return new WaitForSeconds (time);
			OnDestroyed ();
		}

		public void SetDestroy (float time)
		{
			StartCoroutine (setDestrying (time));
		}

		public void Destroying (float time)
		{
			SetDestroy (time);
		}

		public void Destroying ()
		{
		
			if (GetComponent<Renderer> ())
				GetComponent<Renderer> ().enabled = false;
		
			this.transform.localScale = scaleTemp;
			this.transform.position = positionTemp;
			this.transform.rotation = rotationTemp;
			if (rigidBody) {
				rigidBody.velocity = Vector3.zero;
				rigidBody.angularVelocity = Vector3.zero;
			}
			if (lineRenderer) {
				lineRenderer.SetPosition (0, this.transform.position);
				lineRenderer.SetPosition (1, this.transform.position);
			}

			if (GetComponent<ParticleSystem> ()) {
				GetComponent<ParticleSystem> ().Stop ();
			}

			foreach(TrailRenderer trail in trailRenderers){
				trail.time = 0;
				trail.Clear();
			}
		
			this.gameObject.SetActive (false);
			Active = false;
		}

		public virtual void OnDestroyed ()
		{
			Destroying ();
		}
	}
}
