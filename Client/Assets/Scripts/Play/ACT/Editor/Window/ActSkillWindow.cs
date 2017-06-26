using UnityEngine;
using System.Collections;
using UnityEditor;
using System;
using NLE;

namespace NLE
{
    public class ActSkillWindow : EditorWindow
    {
        private Texture mSettingsImage       = null;
        private Texture mRescaleImage        = null;
        private Texture mZoomInImage         = null;
        private Texture mZoomOutImage        = null;
        private Texture mSnapImage           = null;
        private Texture mRollingEditImage    = null;
        private Texture mRippleEditImage     = null;
        private Texture mPickerImage         = null;
        private Texture mRefreshImage        = null;
        private Texture mTitleImage          = null;
        private Texture mCropImage           = null;
        private Texture mScaleImage          = null;
        private GUISkin mSkin = null;

        private GUIStyle mToolbarStyle       = null;
        private GUIStyle mToolbarDropDown    = null;
        private GUIStyle mToolbarButton      = null;

        private const float V_TOOLBAR_HEIGHT = 30f;
        private const float V_FRAME_LIMITER  = 1 / 60f;
          
        private DateTime mPreviousTime       = DateTime.Now;
        private double   mAccumulatedTime    = 0f;
        private int      mPopupSelection     = 0;
        private float    mMenuWidth          = 300;

        public GUIStyle ToolbarStyle
        {
            get
            {
                mToolbarStyle = new GUIStyle(EditorStyles.toolbar);
                mToolbarStyle.alignment = TextAnchor.MiddleCenter;
                mToolbarStyle.fixedHeight = V_TOOLBAR_HEIGHT;
                mToolbarStyle.fontSize = 20;
                return mToolbarStyle;
            }
        }

        public GUIStyle ToolbarDropDown
        {
            get
            {
                mToolbarDropDown = new GUIStyle(EditorStyles.toolbarDropDown);
                mToolbarDropDown.alignment = TextAnchor.MiddleCenter;
                mToolbarDropDown.fixedHeight = V_TOOLBAR_HEIGHT;
                mToolbarDropDown.fontSize = 20;
                return mToolbarDropDown;
            }
        }

        public GUIStyle ToolbarButton
        {
            get
            {

                mToolbarButton = new GUIStyle(EditorStyles.toolbarButton);
                mToolbarButton.alignment = TextAnchor.MiddleCenter;
                mToolbarButton.fixedHeight = V_TOOLBAR_HEIGHT;
                mToolbarButton.fontSize = 20;
                return mToolbarButton;
            }
        }

        void OnEnable()
        {
            this.titleContent = new GUIContent("技能编辑器", mTitleImage);
            this.minSize = new Vector2(400, 250);
            this.mPreviousTime = DateTime.Now;
            this.mAccumulatedTime = 0;
            this.LoadTextures();
        }

        void OnGUI()
        {
            Rect toolbarArea = new Rect(0, 0, base.position.width, V_TOOLBAR_HEIGHT);
            Rect controlArea = new Rect(0, V_TOOLBAR_HEIGHT, base.position.width, base.position.height - V_TOOLBAR_HEIGHT);
            this.OnUpdateToolbar(toolbarArea);
            this.OnUpdateControl(controlArea);
        }

        void LoadTextures()
        {
            mSettingsImage     = Resources.Load<Texture>(NStyles.Director_SettingsIcon);
            mRescaleImage      = Resources.Load<Texture>(NStyles.Director_RescaleIcon);
            mZoomInImage       = Resources.Load<Texture>(NStyles.Director_ZoomInIcon);
            mZoomOutImage      = Resources.Load<Texture>(NStyles.Director_ZoomOutIcon);
            mSnapImage         = Resources.Load<Texture>(NStyles.Director_Magnet);
            mRollingEditImage  = Resources.Load<Texture>(NStyles.Director_RollingIcon);
            mRippleEditImage   = Resources.Load<Texture>(NStyles.Director_RippleIcon);
            mPickerImage       = Resources.Load<Texture>(NStyles.Director_PickerIcon);
            mRefreshImage      = Resources.Load<Texture>(NStyles.Director_RefreshIcon);
            mTitleImage        = Resources.Load<Texture>(NStyles.Director_Title);
            mCropImage         = Resources.Load<Texture>(NStyles.Director_Resize_Crop);
            mScaleImage        = Resources.Load<Texture>(NStyles.Director_Resize_Scale);

            mSkin = Resources.Load<GUISkin>(NStyles.Director_Skin);
        }

        void OnUpdateToolbar(Rect toolbarArea)
        {
            EditorGUILayout.BeginHorizontal(ToolbarStyle, GUILayout.ExpandWidth(true));
            if (GUILayout.Button("创建", ToolbarDropDown, GUILayout.Width(100), GUILayout.Height(V_TOOLBAR_HEIGHT)))
            {
                GenericMenu menu = new GenericMenu();
                menu.DropDown(new Rect(5, V_TOOLBAR_HEIGHT, 0, 0));
            }
            EditorGUILayout.EndHorizontal();
        }

        void OnUpdateControl(Rect controlArea)
        {
            EditorGUILayout.BeginHorizontal(ToolbarStyle, GUILayout.ExpandWidth(true));
            EditorGUILayout.EndHorizontal();
        }
    }
}

