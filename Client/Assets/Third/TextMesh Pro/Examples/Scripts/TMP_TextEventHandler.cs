using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using System;


namespace TMPro
{

    public class TMP_TextEventHandler : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
    {
        [Serializable]
        public class CharacterSelectionEvent : UnityEvent<char, int> { }

        [Serializable]
        public class WordSelectionEvent : UnityEvent<string, int, int> { }

        [Serializable]
        public class LinkSelectionEvent : UnityEvent<string, string, int> { }

        /// <summary>
        /// Event delegate triggered when pointer is over a character.
        /// </summary>
        public CharacterSelectionEvent onCharacterSelection
        {
            get { return m_OnCharacterSelection; }
            set { m_OnCharacterSelection = value; }
        }
        [SerializeField]
        private CharacterSelectionEvent m_OnCharacterSelection = new CharacterSelectionEvent();

        /// <summary>
        /// Event delegate triggered when pointer is over a word.
        /// </summary>
        public WordSelectionEvent onWordSelection
        {
            get { return m_OnWordSelection; }
            set { m_OnWordSelection = value; }
        }
        [SerializeField]
        private WordSelectionEvent m_OnWordSelection = new WordSelectionEvent();

        /// <summary>
        /// Event delegate triggered when pointer is over a link.
        /// </summary>
        public LinkSelectionEvent onLinkSelection
        {
            get { return m_OnLinkSelection; }
            set { m_OnLinkSelection = value; }
        }
        [SerializeField]
        private LinkSelectionEvent m_OnLinkSelection = new LinkSelectionEvent();



        private TMP_Text m_TextComponent;

        private Camera m_Camera;
        private Canvas m_Canvas;

        private int m_selectedLink = -1;
        private int m_lastCharIndex = -1;
        private int m_lastWordIndex = -1;

        void Awake()
        {
            // Get a reference to the text component.
            m_TextComponent = gameObject.GetComponent<TMP_Text>();

            // Get a reference to the camera rendering the text taking into consideration the text component type.
            if (m_TextComponent.GetType() == typeof(TextMeshProUGUI))
            {
                m_Canvas = gameObject.GetComponentInParent<Canvas>();
                if (m_Canvas != null)
                {
                    if (m_Canvas.renderMode == RenderMode.ScreenSpaceOverlay)
                        m_Camera = null;
                    else
                        m_Camera = m_Canvas.worldCamera;
                }
            }
            else
            {
                m_Camera = Camera.main;
            }
        }


        void LateUpdate()
        {
            if (TMP_TextUtilities.IsIntersectingRectTransform(m_TextComponent.rectTransform, Input.mousePosition, m_Camera))
            {
                #region Example of Character Selection
                int charIndex = TMP_TextUtilities.FindIntersectingCharacter(m_TextComponent, Input.mousePosition, m_Camera, true);
                if (charIndex != -1 && charIndex != m_lastCharIndex)
                {
                    m_lastCharIndex = charIndex;

                    // Send event to any event listeners.
                    SendOnCharacterSelection(m_TextComponent.textInfo.characterInfo[charIndex].character, charIndex);
                }
                #endregion


                #region Example of Word Selection
                // Check if Mouse intersects any words and if so assign a random color to that word.
                int wordIndex = TMP_TextUtilities.FindIntersectingWord(m_TextComponent, Input.mousePosition, m_Camera);
                if (wordIndex != -1 && wordIndex != m_lastWordIndex)
                {
                    m_lastWordIndex = wordIndex;

                    // Get the information about the selected word.
                    TMP_WordInfo wInfo = m_TextComponent.textInfo.wordInfo[wordIndex];

                    // Send the event to any listeners.
                    SendOnWordSelection(wInfo.GetWord(), wInfo.firstCharacterIndex, wInfo.characterCount);
                }
                #endregion


                #region Example of Link Handling
                // Check if mouse intersects with any links.
                int linkIndex = TMP_TextUtilities.FindIntersectingLink(m_TextComponent, Input.mousePosition, m_Camera);

                // Handle new Link selection.
                if (linkIndex != -1 && linkIndex != m_selectedLink)
                {
                    m_selectedLink = linkIndex;

                    // Get information about the link.
                    TMP_LinkInfo linkInfo = m_TextComponent.textInfo.linkInfo[linkIndex];

                    // Send the event to any listeners. 
                    SendOnLinkSelection(linkInfo.GetLinkID(), linkInfo.GetLinkText(), linkIndex);
                }
                #endregion
            }
        }


        public void OnPointerEnter(PointerEventData eventData)
        {
            //Debug.Log("OnPointerEnter()");
        }


        public void OnPointerExit(PointerEventData eventData)
        {
            //Debug.Log("OnPointerExit()");
        }


        private void SendOnCharacterSelection(char character, int characterIndex)
        {
            if (onCharacterSelection != null)
                onCharacterSelection.Invoke(character, characterIndex);
        }

        private void SendOnWordSelection(string word, int charIndex, int length)
        {
            if (onWordSelection != null)
                onWordSelection.Invoke(word, charIndex, length);
        }

        private void SendOnLinkSelection(string linkID, string linkText, int linkIndex)
        {
            if (onLinkSelection != null)
                onLinkSelection.Invoke(linkID, linkText, linkIndex);
        }

    }
}
