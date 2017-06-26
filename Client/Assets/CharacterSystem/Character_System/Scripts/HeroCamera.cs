using UnityEngine;
using System.Collections;

public class HeroCamera : MonoBehaviour
{
	public LayerMask collisionLayers = -1;
    public float heroHeight = 2.0f;
    public float heroDistance = 5.0f;
    public float minDistance = 2.0f;
    public float maxDistance = 10.0f;
	public int zoomRate = 200;
	public float zoomDampening = 5.0f;
	public float xSpeed = 200.0f;
	public float ySpeed = 200.0f;
	public bool invertMouseY = false;
    public float rotationDampening = 3.0f;
    public float offsetFromWall = 0.1f;
	public float fpsCamDist = -0.15f;
	
	public bool useIdleOrbit = true;
	
	public enum CameraState
	{
		FirstPerson,
		ThirdPerson,
		Orbit
	}
	public CameraState camState = CameraState.ThirdPerson;
	
	public Transform cam = null;
	
	Transform hero;
	public Transform headBone = null;
	public int minAngleY = -50;
	public int maxAngleY = 60;
    float xAngl = 0.0f;
    float yAngl = 0.0f;
    float curDist;
    float desDist;
    float finalDist;
	
	
	HeroCtrl hCtrl;
	
	
	public CamInput camInput;
	[System.Serializable]
	public class CamInput
	{
		public float mX = 0;
		public float mY = 0;
		public float mSW = 0;
		public bool doFPS = false;
		public bool do3rdP = false;
		public bool doOrbit = false;
		public bool doLShift = false;
	}
	
	
	//=================================================================================================================o
    void Start ()
    {
    	hero = transform;
		
		hCtrl = GetComponent <HeroCtrl>() as HeroCtrl;
		
		
    	Vector3 angls = cam.eulerAngles;
    	xAngl = angls.x;
    	yAngl = angls.y;

		curDist = heroDistance;
    	desDist = heroDistance;
    	finalDist = heroDistance;
				
		//cam.camera.nearClipPlane = 0.01f;
		
    	//Screen.lockCursor = true;
    	Cursor.visible = false;
		
		// if no headbone search for it
		if (headBone == null) {
			Transform[] bones = GetComponentsInChildren <Transform>() as Transform[];
			foreach (Transform t in bones) {
				if (t.name == "head")
					headBone = t;
			}
		}
    }
	//=================================================================================================================o
	void IdleOrbit ()
	{
		if(hCtrl == null)
			return;
		if(hCtrl.v == 0 && hCtrl.h == 0)
		{
			hCtrl.canRotate = false;
			camState = CameraState.Orbit;
		}
		else
		{
			if(camState == CameraState.Orbit)
			{
				Quaternion r = Quaternion.Euler(0,cam.eulerAngles.y,0);
				hero.rotation = Quaternion.Lerp(hero.rotation, r, Time.deltaTime * 15);
				hCtrl.canRotate = true;
				camState = CameraState.ThirdPerson;
			}
		}
	}
	//=================================================================================================================o
    void LateUpdate ()
    {
		// Cached Input
		camInput.doFPS = Input.GetKeyDown ("1");
		camInput.doOrbit = Input.GetKeyDown ("2");
		camInput.do3rdP = Input.GetKeyDown ("3");
		camInput.doLShift = Input.GetKey (KeyCode.LeftShift);
		camInput.mX = Input.GetAxis ("Mouse X");
		camInput.mY = Input.GetAxis ("Mouse Y");
		camInput.mSW = Input.GetAxis ("Mouse ScrollWheel");
		
		
		
		
		// 1,2,3 buttons for switching camera modi
    	if (camInput.doFPS) 
		{
    		// FirstPerson
    		cam.GetComponent<Camera>().fieldOfView = 80.0f;
    		camState = CameraState.FirstPerson;
    	}
		else if (camInput.doOrbit) 
		{
    		// Orbit
    		cam.GetComponent<Camera>().fieldOfView = 70.0f;
    		camState = CameraState.Orbit;
    	}
		else if (camInput.do3rdP)
		{
    		// ThirdPerson
			cam.GetComponent<Camera>().fieldOfView = 70.0f;
    		camState = CameraState.ThirdPerson;
    	}
		else if(useIdleOrbit)
		{
			IdleOrbit();
		}
		
		
		
		// Camera states
		switch (camState)
		{
		case CameraState.FirstPerson:
			FirstPerson();
			break;
		case CameraState.ThirdPerson:
			ThirdPerson();
			break;
		case CameraState.Orbit:
			Orbit();
			break;
		}
	}
	//=================================================================================================================o
	void FirstPerson ()
	{
		// Horizontal
		xAngl = hero.eulerAngles.y;
		// Vertical
    	yAngl = ClampAngle (yAngl, minAngleY /1.5f, maxAngleY /1.1f);
		
		// Desired distance
		desDist = fpsCamDist;
		// Camera rotation
    	Quaternion camRot = Quaternion.Euler (yAngl, xAngl, 0);
    	// Camera position
		Vector3 camPos = headBone.position - (cam.forward * desDist) - (cam.up * -heroHeight /4);
		
		// Apply Y-mouse axis
		if(invertMouseY)
		    yAngl += Input.GetAxis ("Mouse Y") * ySpeed * 0.02f;
		else
		    yAngl -= Input.GetAxis ("Mouse Y") * ySpeed * 0.02f;
		
		// Apply position and rotation
		cam.rotation = camRot;
		cam.position = camPos;
	}
	//=================================================================================================================o
	void ThirdPerson ()
	{
		// Desired distance via mouse wheel
		desDist -= camInput.mSW * Time.deltaTime * zoomRate * Mathf.Abs (desDist);
		desDist = Mathf.Clamp (desDist, minDistance, maxDistance);
		finalDist = desDist;
		
		// Horizontal smooth rotation
		xAngl = Mathf.LerpAngle (cam.eulerAngles.y, hero.eulerAngles.y, rotationDampening * Time.deltaTime);
		// Vertical angle limitation
    	yAngl = ClampAngle (yAngl, minAngleY, maxAngleY);
    	// Camera rotation
    	Quaternion camRot = Quaternion.Euler (yAngl, xAngl, 0);
    	// Camera height
    	Vector3 headPos = new Vector3 (0, -heroHeight /1.2f, 0);
    	// Camera position
    	Vector3 camPos = hero.position - (camRot * Vector3.forward * desDist + headPos);
		
		// Recalculate hero position
		Vector3 trueHeroPos = new Vector3 (hero.position.x, hero.position.y + heroHeight, hero.position.z);
		
		// Check for collision with Linecast
		RaycastHit hit;
		bool isOk = false;
		if ( Physics.Linecast (trueHeroPos, camPos - Vector3.up + Vector3.forward, out hit, collisionLayers.value)) // slightly behind and below the camera
		{
			// Final distance
			finalDist = Vector3.Distance (trueHeroPos, hit.point) - offsetFromWall;
			isOk = true;
		}
		
		// Lerp current distance if not corrected
		if ( !isOk || ( finalDist > curDist ) )
			curDist = Mathf.Lerp (curDist, finalDist, Time.deltaTime * zoomDampening);
		else
			curDist = finalDist;
		
		// Clamp current distance
		//curDist = Mathf.Clamp (curDist, minDistance, maxDistance);
		
		// Recalculate camera position
		camPos = hero.position - (camRot * Vector3.forward * curDist + headPos);
		
		// Left shift = no y rotation
		if(!camInput.doLShift)
		{
			// Apply Y-mouse axis
			if(invertMouseY)
			    yAngl += Input.GetAxis ("Mouse Y") * ySpeed * 0.02f;
			else
			    yAngl -= Input.GetAxis ("Mouse Y") * ySpeed * 0.02f;
		}
		
		
		// Apply position and rotation
		cam.rotation = camRot;
		cam.position = camPos;
	}
	//=================================================================================================================o
	void Orbit ()
	{
		// Desired distance via mouse wheel
		desDist -= camInput.mSW * Time.deltaTime * zoomRate * Mathf.Abs (desDist);
		desDist = Mathf.Clamp (desDist, minDistance, maxDistance);
		finalDist = desDist;
		
		// Horizontal smooth rotation
		xAngl += camInput.mX * xSpeed * 0.02f;
		// Vertical angle limitation
    	yAngl = ClampAngle (yAngl, minAngleY, maxAngleY);
		
		// Camera rotation
    	Quaternion camRot = Quaternion.Euler (yAngl, xAngl, 0);
    	// Camera height
    	Vector3 headPos = new Vector3 (0, -heroHeight /1.2f, 0);
    	// Camera position
    	Vector3 camPos = hero.position - (camRot * Vector3.forward * desDist + headPos);
		
		// Recalculate hero position
		Vector3 trueHeroPos = new Vector3 (hero.position.x, hero.position.y + heroHeight, hero.position.z);
		
		// Check if there is something between camera and character
		RaycastHit hit;
		bool isOk = false;
		if ( Physics.Linecast (trueHeroPos, camPos, out hit, collisionLayers.value))
		{
			// Final distance
			finalDist = Vector3.Distance (trueHeroPos, hit.point) - offsetFromWall;
			isOk = true;
		}
		
		// Lerp current distance if not corrected
		if ( !isOk || ( finalDist > curDist ) )
			curDist = Mathf.Lerp (curDist, finalDist, Time.deltaTime * zoomDampening);
		else
			curDist = finalDist;
		
		// Clamp current distance
		//curDist = Mathf.Clamp (curDist, minDistance, maxDistance);
		
		// Recalculate camera position
		camPos = hero.position - (camRot * Vector3.forward * curDist + headPos);
		
		// Left shift = no y rotation
		if(!camInput.doLShift)
		{
			// Apply Y-mouse axis
			if(invertMouseY)
			    yAngl += Input.GetAxis ("Mouse Y") * ySpeed * 0.02f;
			else
			    yAngl -= Input.GetAxis ("Mouse Y") * ySpeed * 0.02f;
		}

		
		// Apply position and rotation
		cam.rotation = camRot;
		cam.position = camPos;
	}
	//=================================================================================================================o
	
	// Clamp angle at 360deg
	static float ClampAngle ( float angle, float min, float max )
	{
		if (angle < -360)
			angle += 360;
		if (angle > 360)
			angle -= 360;
		return Mathf.Clamp (angle, min, max);
	}
	//=================================================================================================================o
}
