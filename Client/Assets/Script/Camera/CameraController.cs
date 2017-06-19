using UnityEngine;
using System.Collections;
using DG.Tweening;
using System.Collections.Generic;

public class CameraController : MonoBehaviour
{
    private Vector3   touchPos;
    private Vector3   touchStartPos;
    private Camera    cam;

    public LayerMask  collisionLayers = 1 << 0;
    public float      heroHeight = 4.0f;
    public float      heroDistance = 12.0f;
    public float      minDistance = 2.0f;
    public float      maxDistance = 20.0f;
    public int        zoomRate = 200;
    public float      zoomDampening = 5.0f;
    public float      xSpeed = 200.0f;
    public float      ySpeed = 200.0f;
    public bool       invertMouseY = false;
    public float      rotationDampening = 0f;
    public float      offsetFromWall = 0.1f;
    public int        minAngleY = 10;
    public int        maxAngleY = 75;

    public bool       isShake      { get; set; }
    public bool       isLockFollow { get; set; }
    public bool       isOpenPhysic { get; set; }

    private Transform hero;
    private float     xAngle = 0.0f;
    private float     yAngle = 0.0f;
    private float     curDist;
    private float     desDist;
    private float     finalDist;
    private float     mX;
    private float     mY;
    private float     mSW;

    private List<Touch> mTouches = new List<Touch>();

    public void DoShake(float duration, float strength, int vibrato, float randomness, bool fadeout)
    {
        isShake = true;
        cam.DOShakePosition(duration, strength, vibrato, randomness, fadeout);
    }

    public void SetTarget(Transform trans)
    {
        hero = trans;
        xAngle = 0;
        yAngle = 30;
        curDist = heroDistance;
        desDist = heroDistance;
        finalDist = heroDistance;
    }

    public void StopShake()
    {
        isShake = false;
    }

    void Start()
    {
        cam = this.GetComponent<Camera>();
    }

    void OnWindowInput()
    {
        mX = Input.GetAxis("Mouse X");
        mY = Input.GetAxis("Mouse Y");
        mSW = Input.GetAxis("Mouse ScrollWheel");
    }

    void OnMobileInput()
    {
        mX = 0;
        mY = 0;
        mSW = 0;
        switch (Input.touchCount)
        {
            case 1:
                {
                    Touch th = Input.GetTouch(0);
                    if(UICamera.Raycast(th.position))
                    {
                        return;
                    }
                    if (th.phase == TouchPhase.Began)
                    {
                        this.touchPos = th.position;
                        this.touchStartPos = th.position;
                    }
                    if (th.phase == TouchPhase.Moved && this.touchStartPos.x > Screen.width * 0.5f)
                    {
                        mX = th.position.x - this.touchPos.x;
                        mY = th.position.y - this.touchPos.y;
                        this.touchPos = th.position;
                    }
                    if (th.phase == TouchPhase.Ended)
                    {
                        this.touchStartPos = Vector3.zero;
                    }
                }
                break;
            default:
                mTouches.Clear();
                for (int i = 0; i < Input.touchCount; i++)
                {
                    Touch th = Input.GetTouch(i);
                    if (UICamera.Raycast(th.position) == false)
                    {
                        mTouches.Add(th);
                    }
                }
                if (mTouches.Count == 1)
                {
                    Touch th = mTouches[0];
                    if (th.phase == TouchPhase.Began)
                    {
                        this.touchPos = th.position;
                        this.touchStartPos = th.position;
                    }
                    if (th.phase == TouchPhase.Moved && this.touchStartPos.x > Screen.width * 0.5f)
                    {
                        mX = th.position.x - this.touchPos.x;
                        mY = th.position.y - this.touchPos.y;
                        this.touchPos = th.position;
                    }
                    if (th.phase == TouchPhase.Ended)
                    {
                        this.touchStartPos = Vector3.zero;
                    }
                }
                else if (mTouches.Count >= 2)
                {
                    Touch touch0 = mTouches[0];
                    Touch touch1 = mTouches[1];
                    if (touch0.phase == TouchPhase.Moved && touch1.phase == TouchPhase.Moved)
                    {
                        Vector2 touchZeroPrevPos = touch0.position - touch0.deltaPosition;
                        Vector2 touchOnePrevPos = touch1.position - touch1.deltaPosition;
                        float prevTouchDeltaMag = (touchZeroPrevPos - touchOnePrevPos).magnitude;
                        float touchDeltaMag = (touch0.position - touch1.position).magnitude;
                        float deltaMagnitudeDiff = prevTouchDeltaMag - touchDeltaMag;
                        mSW = -deltaMagnitudeDiff * zoomDampening * 0.0001f;
                    }
                }
                break;
        }
        float absX = Mathf.Abs(mX);
        float absY = Mathf.Abs(mY);
        if (absY > absX)
        {
            mY = mY > 0 ? -2 : (mY < 0 ? 2 : 0);
            mX = 0;
        }
        else
        {
            mX = mX * 0.25f;
            mY = 0;
        }
    }

    void LateUpdate()
    {
        if(isShake || isLockFollow)
        {
            return;
        }
        if (hero == null)
        {
            return;
        }
        switch (Application.platform)
        {
            case RuntimePlatform.Android:
            case RuntimePlatform.IPhonePlayer:
                if (Input.touchCount > 0)
                {
                    OnMobileInput();
                }
                break;
            case RuntimePlatform.WindowsPlayer:
            case RuntimePlatform.WindowsEditor:
                {
                   
                }
                break;
        }
        CalcCameraPosition();
    }

    void CalcCameraPosition()
    {
        desDist -= mSW * Time.deltaTime * zoomRate * Mathf.Abs(desDist);
        desDist = Mathf.Clamp(desDist, minDistance, maxDistance);
        finalDist = desDist;
        yAngle += mY;
        xAngle = Mathf.LerpAngle(transform.eulerAngles.y + mX, hero.eulerAngles.y , rotationDampening * Time.deltaTime);
        yAngle = ClampAngle(yAngle, minAngleY, maxAngleY);
        Quaternion camRot = Quaternion.Euler(yAngle, xAngle, 0);
        Vector3 camPos = hero.position - (camRot * Vector3.forward * desDist);
        Vector3 trueHeroPos = new Vector3(hero.position.x, hero.position.y + heroHeight, hero.position.z);
        RaycastHit hit;
        bool isOk = false;
        if(isOpenPhysic)
        {
            if (Physics.Linecast(trueHeroPos, camPos - Vector3.up + Vector3.forward, out hit, collisionLayers.value)) // slightly behind and below the camera
            {
                finalDist = Vector3.Distance(trueHeroPos, hit.point) - offsetFromWall;
                isOk = true;
            }
        }
        if (!isOk || (finalDist > curDist))
        {
            curDist = Mathf.Lerp(curDist, finalDist, Time.deltaTime * zoomDampening);
        }
        else
        {
            curDist = finalDist;
        }
        camPos = hero.position - (camRot * Vector3.forward * curDist);
        transform.rotation = camRot;
        transform.position = camPos;
    }

    static float ClampAngle(float angle, float min, float max)
    {
        if (angle < -360)
            angle += 360;
        if (angle > 360)
            angle -= 360;
        return Mathf.Clamp(angle, min, max);
    }
}