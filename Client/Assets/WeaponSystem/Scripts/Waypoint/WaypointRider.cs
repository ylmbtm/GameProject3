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

public class WaypointRider : MonoBehaviour
{

	private int _targetWaypoint = 0;
	private Transform _waypoints;
	public float movementSpeed = 3f;
 
	// Use this for initialization
	void Start ()
	{
		_waypoints = GameObject.Find ("Waypoints").transform;
	}
     
	// Update is called once per frame
	void Update ()
	{
     
	}
 
	// Fixed update
	void FixedUpdate ()
	{
		handleWalkWaypoints ();
	}
 
	// Handle walking the waypoints
	private void handleWalkWaypoints ()
	{
		Transform targetWaypoint = _waypoints.GetChild (_targetWaypoint);
		Vector3 relative = targetWaypoint.position - transform.position;
		Vector3 movementNormal = Vector3.Normalize (relative);
		float distanceToWaypoint = relative.magnitude;
 
		if (distanceToWaypoint < 0.1) {
			if (_targetWaypoint + 1 < _waypoints.childCount) {
				// Set new waypoint as target
				_targetWaypoint++;
			} else {
				Destroy (gameObject);
				return;
			}
		} else {
            
			this.transform.position += (movementNormal * movementSpeed) * Time.fixedDeltaTime;
		}
		Quaternion look = Quaternion.LookRotation (movementNormal);
		this.transform.rotation = Quaternion.Lerp (this.transform.rotation, look, 0.5f);
	}
}
