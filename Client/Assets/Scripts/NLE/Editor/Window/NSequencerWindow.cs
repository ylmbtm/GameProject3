using NLE;
using NLE.EDT;
using System;
using System.Timers;
using UnityEditor;
using UnityEngine;

namespace NLE.EDT
{
    public class NSequencerWindow : EditorWindow
    {
        private DirectorControl mDirectorControl     = null;
        private NSequencer      mSequencer           = null;
        private int             mSequencerInstanceID = -1;
        private CutsceneWrapper mCutSceneWrapper     = null;
        private DateTime        mPreviousTime        = DateTime.Now;
        private NSequencer[]    mCachedSequencers    = null;

        private Texture         mSettingsImage       = null;
        private Texture         mRescaleImage        = null;
        private Texture         mZoomInImage         = null;
        private Texture         mZoomOutImage        = null;
        private Texture         mSnapImage           = null;
        private Texture         mRollingEditImage    = null;
        private Texture         mRippleEditImage     = null;
        private Texture         mPickerImage         = null;
        private Texture         mRefreshImage        = null;
        private Texture         mTitleImage          = null;
        private Texture         mCropImage           = null;
        private Texture         mScaleImage          = null;
        private GUISkin         mSkin                = null;

        private GUIStyle        mToolbarStyle        = null;
        private GUIStyle        mToolbarDropDown     = null;
        private GUIStyle        mToolbarButton       = null;

        private const float     V_TOOLBAR_HEIGHT     = 30f;
        private const float     V_FRAME_LIMITER      = 1 / 60f;

        private double          mAccumulatedTime     = 0f;
        private int             mPopupSelection      = 0;

        public GUIStyle         ToolbarStyle
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

        public GUIStyle         ToolbarDropDown
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

        public GUIStyle         ToolbarButton
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
            this.titleContent     = new GUIContent("NLE", mTitleImage);
            this.minSize          = new Vector2(400, 250);
            this.mPreviousTime    = DateTime.Now;
            this.mAccumulatedTime = 0;
            this.LoadTextures();
            this.AddListeners();
            this.Init();
        }
   
        void OnGUI()
        {
            if (mSequencer == null)
            {
                return;
            }
            Rect toolbarArea = new Rect(0, 0, base.position.width, V_TOOLBAR_HEIGHT);
            Rect controlArea = new Rect(0, V_TOOLBAR_HEIGHT, base.position.width, base.position.height - V_TOOLBAR_HEIGHT);
            this.OnUpdateToolbar(toolbarArea);
            mCutSceneWrapper = NDirectorHelper.UpdateWrapper(mSequencer, mCutSceneWrapper);
            try
            {
                mDirectorControl.OnGUI(controlArea, mCutSceneWrapper);
            }
            catch(Exception ex)
            {
                Debug.LogError(ex.ToString());
            }
            NDirectorHelper.ReflectChanges(mSequencer, mCutSceneWrapper);
        }

        void LoadTextures()
        {
            mSettingsImage      = Resources.Load<Texture>(NStyles.Director_SettingsIcon);
            mRescaleImage       = Resources.Load<Texture>(NStyles.Director_RescaleIcon);
            mZoomInImage        = Resources.Load<Texture>(NStyles.Director_ZoomInIcon);
            mZoomOutImage       = Resources.Load<Texture>(NStyles.Director_ZoomOutIcon);
            mSnapImage          = Resources.Load<Texture>(NStyles.Director_Magnet);
            mRollingEditImage   = Resources.Load<Texture>(NStyles.Director_RollingIcon);
            mRippleEditImage    = Resources.Load<Texture>(NStyles.Director_RippleIcon);
            mPickerImage        = Resources.Load<Texture>(NStyles.Director_PickerIcon);
            mRefreshImage       = Resources.Load<Texture>(NStyles.Director_RefreshIcon);
            mTitleImage         = Resources.Load<Texture>(NStyles.Director_Title);
            mCropImage          = Resources.Load<Texture>(NStyles.Director_Resize_Crop);
            mScaleImage         = Resources.Load<Texture>(NStyles.Director_Resize_Scale);

            mSkin               = Resources.Load<GUISkin>(NStyles.Director_Skin);
        }

        void AddListeners()
        {
            EditorApplication.update = (EditorApplication.CallbackFunction)System.Delegate.Combine(EditorApplication.update, new EditorApplication.CallbackFunction(this.OnDirectorUpdate));
            EditorApplication.playmodeStateChanged = (EditorApplication.CallbackFunction)System.Delegate.Combine(EditorApplication.playmodeStateChanged, new EditorApplication.CallbackFunction(this.OnPlayModeChanged));
            mDirectorControl = new DirectorControl();
            mDirectorControl.OnLoad(mSkin);
            mDirectorControl.PlayCutscene     += OnDC_PlayCutscene;
            mDirectorControl.PauseCutscene    += OnDC_PauseCutscene;
            mDirectorControl.StopCutscene     += OnDC_StopCutscene;
            mDirectorControl.ScrubCutscene    += OnDC_ScrubCutscene;
            mDirectorControl.SetCutsceneTime  += OnDC_SetCutsceneTime;
            mDirectorControl.EnterPreviewMode += OnDC_EnterPreviewMode;
            mDirectorControl.ExitPreviewMode  += OnDC_ExitPreviewMode;
            mDirectorControl.DragPerformed    += OnDC_DragPerformed;
            mDirectorControl.RepaintRequest   += OnDC_RepaintRequest;
        }

        void Init()
        {
            mSequencerInstanceID = NSettings.NSequencerID;
            if (mSequencerInstanceID >= 0)
            {
                foreach (NSequencer c in GameObject.FindObjectsOfType<NSequencer>())
                {
                    if (c.GetInstanceID() == mSequencerInstanceID)
                    {
                        this.mSequencer = c;
                        break;
                    }
                }
            }
        }

        void OnDC_PlayCutscene(object sender, CinemaDirectorArgs e)
        {
            NSequencer ns = e.cutscene as NSequencer;
            if (ns != null)
            {
                if(Application.isPlaying)
                {
                    ns.Play();
                }
                else
                {
                    ns.PreviewPlay();
                    mPreviousTime = DateTime.Now;
                }
            }
        }

        void OnDC_PauseCutscene(object sender, CinemaDirectorArgs e)
        {
            NSequencer ns = e.cutscene as NSequencer;
            if (ns != null)
            {
                ns.Pause();
            }
        }

        void OnDC_StopCutscene(object sender, CinemaDirectorArgs e)
        {
            NSequencer ns = e.cutscene as NSequencer;
            if (ns != null)
            {
                ns.Stop();
            }
        }

        void OnDC_ScrubCutscene(object sender, CinemaDirectorArgs e)
        {
            NSequencer ns = e.cutscene as NSequencer;
            if (ns != null)
            {
                ns.ScrubToTime(e.timeArg);
            }
        }

        void OnDC_DragPerformed(object sender, CinemaDirectorDragArgs e)
        {

        }

        void OnDC_ExitPreviewMode(object sender, CinemaDirectorArgs e)
        {
            NSequencer ns = e.cutscene as NSequencer;
            if (ns != null)
            {
                ns.ExitPreviewMode();
            }
        }

        void OnDC_EnterPreviewMode(object sender, CinemaDirectorArgs e)
        {
            NSequencer ns = e.cutscene as NSequencer;
            if (ns != null)
            {
                ns.EnterPreviewMode();
            }
        }

        void OnDC_SetCutsceneTime(object sender, CinemaDirectorArgs e)
        {
            NSequencer ns = e.cutscene as NSequencer;
            if (ns != null)
            {
                ns.SetRunningTime(e.timeArg);
                mCutSceneWrapper.RunningTime = e.timeArg;
            }
        }

        void OnDC_RepaintRequest(object sender, CinemaDirectorArgs e)
        {
            base.Repaint();
        }

        void OnDirectorUpdate()
        {
            double delta = (DateTime.Now - mPreviousTime).TotalSeconds;
            mPreviousTime = DateTime.Now;
            if (delta > 0)
            {
                mAccumulatedTime += delta;
            }
            if (mAccumulatedTime >= V_FRAME_LIMITER)
            {
                base.Repaint();
                mAccumulatedTime -= V_FRAME_LIMITER;
            }
            if (mSequencer != null)
            {
                if (!Application.isPlaying && mSequencer.State == CutsceneState.PreviewPlaying)
                {
                    mSequencer.UpdateCutscene((float)delta);
                }
            }
        }

        void OnUpdateToolbar(Rect toolbarArea)
        {
            EditorGUILayout.BeginHorizontal(ToolbarStyle);

            if (GUILayout.Button("创建", ToolbarDropDown, GUILayout.Width(100), GUILayout.Height(V_TOOLBAR_HEIGHT)))
            {
                GenericMenu menu = new GenericMenu();
                menu.AddItem(new GUIContent("New Sequencer"), false, OnOpenSequencerCreatorWindow);
                if (mSequencer != null)
                {
                    menu.AddSeparator(string.Empty);
                    foreach(Type type in NDirectorHelper.GetAllSubTypes(typeof(NGroup)))
                    {
                        NGroupAttribute attr = NDirectorHelper.GetAttribute<NGroupAttribute>(type);
                        string label = attr != null ? attr.Label : type.Name;
                        GroupContextData userData = new GroupContextData(type, label);
                        menu.AddItem(new GUIContent(label), false, new GenericMenu.MenuFunction2(AddGroupContext), userData);
                    }
                }
                menu.DropDown(new Rect(5, V_TOOLBAR_HEIGHT, 0, 0));
            }

            if (GUILayout.Button(mPickerImage, ToolbarButton, GUILayout.Width(V_TOOLBAR_HEIGHT)))
            {
                Selection.activeObject = mSequencer;
            }

            if (GUILayout.Button(mRefreshImage, ToolbarButton, GUILayout.Width(V_TOOLBAR_HEIGHT)))
            {
                mSequencer.Refresh();
            }

            if (Event.current.control && Event.current.keyCode == KeyCode.Space)
            {
                mSequencer.Refresh();
                Event.current.Use();
            }

            GUILayout.FlexibleSpace();

            mDirectorControl.IsSnappingEnabled = GUILayout.Toggle(mDirectorControl.IsSnappingEnabled, mSnapImage, ToolbarButton, GUILayout.Width(V_TOOLBAR_HEIGHT));
            GUILayout.Space(10f);
            if (GUILayout.Button(mRescaleImage, ToolbarButton, GUILayout.Width(V_TOOLBAR_HEIGHT)))
            {
                mDirectorControl.Rescale();
            }

            if (GUILayout.Button(mZoomInImage, ToolbarButton, GUILayout.Width(V_TOOLBAR_HEIGHT)))
            {
                mDirectorControl.ZoomIn();
            }

            if (GUILayout.Button(mZoomOutImage, ToolbarButton, GUILayout.Width(V_TOOLBAR_HEIGHT)))
            {
                mDirectorControl.ZoomOut();
            }

            GUILayout.Space(10);

            Color tempColor = GUI.color;
            GUI.color = mDirectorControl.InPreviewMode ? Color.green : tempColor;
            mDirectorControl.InPreviewMode = GUILayout.Toggle(mDirectorControl.InPreviewMode, "预览模式", ToolbarButton, GUILayout.Width(150));
            GUI.color = tempColor;
            GUILayout.Space(10);

            EditorGUILayout.EndHorizontal();
        }

        void OnOpenSequencerCreatorWindow()
        {

        }

        void OnChooseResizeOption(object userData)
        {
            int selection = (int)userData;
            mDirectorControl.ResizeOption = (DirectorEditor.ResizeOption)selection;
        }

        void OnPlayModeChanged()
        {
            mDirectorControl.InPreviewMode = false;
        }

        void AddGroupContext(object userData)
        {
            GroupContextData data = userData as GroupContextData;
            if (data != null)
            {
                GameObject item = NFactory.CreateGroup(mSequencer, data.Type, data.Label).gameObject;
                Undo.RegisterCreatedObjectUndo(item, string.Format("Create {0}", item.name));
            }
        }

        void AddTrackContext(object userData)
        {

        }

        void AddEventContext(object userData)
        {

        }

        void OnDisable()
        {
            mDirectorControl.OnDisable();
            if (Application.isEditor && mSequencer != null)
            {
                mSequencer.ExitPreviewMode();
            }
            if (mSequencer != null)
            {
                NSettings.NSequencerID = mSequencer.GetInstanceID();
            }
        }

        void OnDestroy()
        {

        }

        public void OnFocusSequencer(NSequencer sequencer)
        {
            if (this.mSequencer != null)
            {
                this.mSequencer.ExitPreviewMode();
            }
            mDirectorControl.InPreviewMode = false;
            NSettings.NSequencerID = sequencer.GetInstanceID();
            this.mSequencer = sequencer;
        }
    }
}