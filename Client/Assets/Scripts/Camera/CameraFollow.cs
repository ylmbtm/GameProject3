using UnityEngine;
using System.Collections;
using DG.Tweening;

public class CameraFollow : MonoBehaviour
{
    [SerializeField]
    private float           m_DefaultDistance = 12;                 // 默认视距
    [SerializeField]
    private float           m_MinDistance     = 8;
    [SerializeField]
    private float           m_MaxDistance     = 20;
    [SerializeField]
    private float           m_MinVertAngle    = -10f;
    [SerializeField]
    private float           m_MaxVertAngle    = 70f;
    [SerializeField]
    private float           m_CurrentXAngle   = 0;                 // 摄像机当前绕X轴旋转的角度
    [SerializeField]
    private float           m_ZoomRate        = 0.01f;
    [SerializeField]
    private float           m_XSensitivity    = 0.10f;
    [SerializeField]
    private float           m_YSensitivity    = 0.10f;

    private float           m_CurDistance;
    private Quaternion      m_OriginalRotation;
    private float           m_X;
    private float           m_Y;
    private float           m_PinchDelta;
    private Camera          m_Cam;
    private Transform       m_Hero;

    public bool isShake      { get; set; }
    public bool isLockFollow { get; set; }

    void Start()
    {
        m_Cam = GetComponentInChildren<Camera>();
    }

    void Update()
    {
        if(GTTouch.Instance.Drag)
        {
            m_X = - GTTouch.Instance.DragDelta.y * m_XSensitivity;
            m_Y =   GTTouch.Instance.DragDelta.x * m_YSensitivity;
        }
        else
        {
            m_X = 0;
            m_Y = 0;
        }
        if(GTTouch.Instance.Pinch)
        {
            m_PinchDelta = GTTouch.Instance.PinchDelta;
        }
        else
        {
            m_PinchDelta = 0;
        }

        m_CurDistance = Mathf.Clamp(m_CurDistance, m_MinDistance, m_MaxDistance);
    }

    void LateUpdate()
    {
        if (m_Hero == null || isLockFollow == true || isShake == true)
        {
            return;
        }
        transform.rotation = m_OriginalRotation;
        if (m_X != 0 || m_Y != 0)
        {
            transform.Rotate(0, m_Y, 0, Space.World);
            m_CurrentXAngle += m_X;
            m_CurrentXAngle = Mathf.Clamp(m_CurrentXAngle, m_MinVertAngle, m_MaxVertAngle);
            transform.rotation = Quaternion.Euler(m_CurrentXAngle, transform.eulerAngles.y, transform.eulerAngles.z);
            m_OriginalRotation = transform.rotation;
        }
        transform.rotation = m_OriginalRotation;

        Vector3 dir = m_Cam.transform.localPosition.normalized;
        RaycastHit hitInfo;
        float dist = m_CurDistance;
        if (Physics.SphereCast(transform.position, 0.5f, transform.TransformDirection(dir), out hitInfo, dist, LayerMask.GetMask("Default"), QueryTriggerInteraction.Ignore))
        {
            dist = hitInfo.distance;
        }
        m_Cam.transform.localPosition = dir * dist;
        m_CurDistance += m_PinchDelta * - m_ZoomRate;
    }

    public void PlayShake(float duration, float strength, int vibrato, float randomness, bool fadeout)
    {
        isShake = true;
        m_Cam.DOShakePosition(duration, strength, vibrato, randomness, fadeout);
    }

    public void StopShake()
    {
        isShake = false;
    }

    public void SetTarget(Transform trans)
    {
        transform.parent        = trans;
        m_Hero                  = trans;
        m_OriginalRotation      = transform.rotation;
        m_CurrentXAngle         = transform.eulerAngles.x;
        m_CurDistance           = m_DefaultDistance;
        transform.localPosition = new Vector3(0, 1, 0);
    }
}
