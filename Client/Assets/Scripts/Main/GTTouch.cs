using UnityEngine;
using System.Collections;
using System;

public class GTTouch : GTMonoSingleton<GTTouch>
{
    private PinchRecognizer    m_PinchRecognizer;
    private DragRecognizer     m_DragRecognizer;
    private FingerDownDetector m_FingerDownDetector;
    private FingerUpDetector   m_FingerUpDetector;
    private DragGesture        m_Drag = null;

    public bool    Pinch
    {
        get; private set;
    }

    public float   PinchDelta
    {
        get; private set;
    }

    public bool    Drag
    {
        get; private set;
    }

    public Vector2 DragDelta
    {
        get; private set;
    }

    public override void SetRoot(Transform parent)
    {
        base.SetRoot(parent);
        m_DragRecognizer  = GetComponent<DragRecognizer>();
        m_PinchRecognizer = GetComponent<PinchRecognizer>();
        m_FingerDownDetector = GetComponent<FingerDownDetector>();
        m_FingerUpDetector = GetComponent<FingerUpDetector>();
        m_DragRecognizer. OnGesture += OnDrag;
        m_PinchRecognizer.OnGesture += OnPinch;
        m_FingerDownDetector.OnFingerDown += OnFingerDown;
        m_FingerUpDetector.OnFingerUp += OnFingerUp;
    }

    private void OnFingerUp(FingerUpEvent eventData)
    {
        if (Camera.main == null)
        {
            return;
        }
        if (Drag || Pinch)
        {
            return;
        }
        if (UICamera.Raycast(eventData.Position))
        {
            return;
        }
        if (UICamera.Raycast(eventData.Finger.Position))
        {
            return;
        }
        if (UICamera.Raycast(eventData.Finger.StartPosition))
        {
            return;
        }
        RaycastHit hit;
        Ray ray = Camera.main.ScreenPointToRay(eventData.Position);
        if(Physics.Raycast(ray, out hit, 1000, 1<< GTLayer.LAYER_DEFAULT) && Application.isEditor)
        {          
            GTNetworkSend.Instance.TryPursue(hit.point);
        }

    }

    private void OnFingerDown(FingerDownEvent eventData)
    {
       
    }

    private void OnPinch(PinchGesture gesture)
    {
        for (int i = 0; i < gesture.Fingers.Count; i++)
        {
            if (UICamera.Raycast(gesture.Fingers[i].StartPosition))
            {
                Pinch = false;
                PinchDelta = 0;
                return;
            }
        }
        switch (gesture.Phase)
        {
            case ContinuousGesturePhase.Started:
                {
                    Pinch = true;
                    PinchDelta = gesture.Delta;
                }
                break;
            case ContinuousGesturePhase.Updated:
                if (Pinch)
                {
                    PinchDelta = gesture.Delta;
                }
                break;
            case ContinuousGesturePhase.None:
            case ContinuousGesturePhase.Ended:
                {
                    Pinch = false;
                    PinchDelta = 0;
                }
                break;
        }
    }

    private void OnDrag(DragGesture gesture)
    {
        if(Pinch)
        {
            return;
        }
        switch(gesture.Phase)
        {
            case ContinuousGesturePhase.Started:
                if (UICamera.Raycast(gesture.StartPosition) == false)
                {
                    m_Drag = gesture;
                    Drag = true;
                    DragDelta = gesture.DeltaMove;
                }
                break;
            case ContinuousGesturePhase.Updated:
                if (Drag && gesture.ClusterId == m_Drag.ClusterId)
                {
                    DragDelta = gesture.DeltaMove;
                }
                break;
            case ContinuousGesturePhase.Ended:
            case ContinuousGesturePhase.None:
                if (m_Drag != null && m_Drag.ClusterId == gesture.ClusterId)
                {
                    m_Drag = null;
                    Drag = false;
                    DragDelta = Vector2.zero;
                }
                break;
        }
    }
}
