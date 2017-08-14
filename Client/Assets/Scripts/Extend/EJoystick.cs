using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class EJoystick : MonoBehaviour
{
    public delegate void JoystickEventHandler(EJoystick joystick);
    public event JoystickEventHandler On_JoystickMove;
    public event JoystickEventHandler On_JoystickMoveEnd;

    private Vector3  m_OriTouchPos     = Vector3.zero;
    private Vector3  m_OriPos          = Vector3.zero;
    private UIWidget m_Root;
    private UISprite m_Area;
    private UISprite m_Touch;
    private bool     m_IsPress         = false;
    private int      m_TouchID         = -1000;

    public int      joystickRadius     = 100;
    public float    joystickMinAlpha   = 0.3f;
    public Vector2  joystickAxis       = Vector2.zero;
    public bool     moveable           = false;
    public float    moveableRadius     = 200f;

    void Awake()
    {
        m_Root = this.GetComponent<UIWidget>();
        m_Area = transform.Find("Area").GetComponent<UISprite>();
        m_Touch = transform.Find("Touch").GetComponent<UISprite>();
    }

    void Start()
    {
        Init();
    }

    void Init()
    {
        m_Area.transform.localPosition = Vector3.zero;
        m_Touch.transform.localPosition = Vector3.zero;
        m_OriTouchPos = m_Touch.transform.localPosition;
        m_OriPos = transform.localPosition;
        Lighting(joystickMinAlpha);
    }

    void Update()
    {
        if (m_Touch == null || m_Area == null || m_Root == null)
        {
            return;
        }
        if (Vector3.Magnitude(m_Touch.transform.localPosition - m_OriTouchPos) > 0.01f)
        {
            Lighting(1f);
            Vector3 offset = m_Touch.transform.localPosition - m_OriTouchPos;
            if (offset.magnitude > joystickRadius)
            {
                if(moveable)
                {
                    Vector3 pos = transform.localPosition + offset.normalized * (offset.magnitude - joystickRadius);
                    Vector3 dir = pos - m_OriPos;
                    if (dir.magnitude < moveableRadius)
                    {
                        transform.localPosition = pos;
                    }
                    else
                    {
                        dir.Normalize();
                        pos = m_OriPos + dir * moveableRadius;
                        transform.localPosition = pos;
                    }
                }
                offset = offset.normalized * joystickRadius;
            }
            joystickAxis.x = offset.x / joystickRadius;
            joystickAxis.y = offset.y / joystickRadius;
            OnJoystickMove();
        }
    }

    void OnPress(bool isPressed)
    {
        if(NGUITools.GetActive(this) == false || !enabled)
        {
            return;
        }
        if (isPressed)
        {
            if(m_IsPress == false)
            {
                m_TouchID = UICamera.currentTouchID;
                m_IsPress = true;
                Lighting(1f);
                CalculateJoystickAxis();
            }
        }
        else
        {
            if(m_TouchID == UICamera.currentTouchID && m_IsPress)
            {
                CalculateJoystickAxis();
                FadeOut(joystickMinAlpha);
                OnJoystickMoveEnd();
            }
        }
    }

    void OnJoystickMove()
    {
        if (On_JoystickMove != null)
        {
            On_JoystickMove(this);
        }
    }

    void OnJoystickMoveEnd()
    {
        if (On_JoystickMoveEnd != null)
        {
            On_JoystickMoveEnd(this);
        }
        m_Touch.transform.localPosition = m_OriTouchPos;
        transform.localPosition = m_OriPos;
        m_IsPress = false;
        m_TouchID = -1000;
    }

    void OnDrag(Vector2 delta)
    {
        if (NGUITools.GetActive(this) == false || !enabled)
        {
            return;
        }
        Lighting(1f);
        CalculateJoystickAxis();
    }

    void CalculateJoystickAxis()
    {
        Vector3 offset = ScreenPos_to_NGUIPos(UICamera.currentTouch.pos);
        offset -= transform.localPosition;
        CalculateJoystickAxis(offset);
    }

    void CalculateJoystickAxis(Vector3 offset)
    {
        if (offset.magnitude > joystickRadius)
        {
            if (moveable)
            {
                Vector3 pos = transform.localPosition + offset.normalized * (offset.magnitude - joystickRadius);
                Vector3 dir = pos - m_OriPos;
                if (dir.magnitude < moveableRadius)
                {
                    transform.localPosition = pos;
                }
                else
                {
                    dir.Normalize();
                    pos = m_OriPos + dir * moveableRadius;
                    transform.localPosition = pos;
                }
            }
            offset = offset.normalized * joystickRadius;
        }
        m_Touch.transform.localPosition = offset;
        joystickAxis.x = offset.x / joystickRadius;
        joystickAxis.y = offset.y / joystickRadius;
    }

    public float Axis2Angle(bool inDegree = true)
    {
        float angle = Mathf.Atan2(joystickAxis.x, joystickAxis.y);
        if (inDegree)
        {
            return angle * Mathf.Rad2Deg;
        }
        else
        {
            return angle;
        }
    }

    public float Axis2Angle(Vector2 axis, bool inDegree = true)
    {
        float angle = Mathf.Atan2(axis.x, axis.y);

        if (inDegree)
        {
            return angle * Mathf.Rad2Deg;
        }
        else
        {
            return angle;
        }
    }

    public void ForceToEnd()
    {
        OnJoystickMoveEnd();
    }

    Vector3 ScreenPos_to_NGUIPos(Vector3 screenPos)
    {
        Vector3 uiPos = UICamera.currentCamera.ScreenToWorldPoint(screenPos);
        uiPos = UICamera.currentCamera.transform.InverseTransformPoint(uiPos);
        return uiPos;
    }

    Vector3 ScreenPos_to_NGUIPos(Vector2 screenPos)
    {
        return ScreenPos_to_NGUIPos(new Vector3(screenPos.x, screenPos.y, 0f));
    }

    void Lighting(float alpha)
    {
        m_Root.alpha = alpha;
    }

    void FadeOut(float toAlpha)
    {
        TweenAlpha.Begin(gameObject, 0.2f, toAlpha);
    }

}