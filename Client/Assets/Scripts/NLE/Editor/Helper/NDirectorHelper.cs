using UnityEngine;
using System.Collections;
using NLE;
using System;
using System.Collections.Generic;
using System.Reflection;

namespace NLE
{
    public class NDirectorHelper
    {
        public static CutsceneWrapper UpdateWrapper(NSequencer cutscene, CutsceneWrapper wrapper)
        {
            if (cutscene == null)
            {
                return null;
            }
            if (wrapper == null || cutscene.Equals(wrapper.Behaviour))
            {
                return CreateWrapper(cutscene);
            }
            else
            {
                wrapper.Behaviour = cutscene;
                wrapper.Duration  = cutscene.Duration;
                wrapper.IsPlaying = cutscene.State == CutsceneState.PreviewPlaying ||
                                    cutscene.State == CutsceneState.Playing;

                List<Behaviour> removeList = new List<Behaviour>();
                foreach(Behaviour behaviour in wrapper.Behaviours)
                {
                    bool found = true;
                    foreach (NGroup group in cutscene.Groups)
                    {
                        if (behaviour.Equals(group))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found || behaviour == null)
                    {
                        removeList.Add(behaviour);
                    }
                }

                foreach(NGroup group in removeList)
                {
                    wrapper.HasChanged = true;
                    wrapper.RemoveTrackGroup(group);
                }

                foreach(NGroup group in cutscene.Groups)
                {
                    TrackGroupWrapper groupWrapper = null;
                    if (!wrapper.ContainsTrackGroup(group, out groupWrapper))
                    {
                        groupWrapper = new TrackGroupWrapper(group);
                        wrapper.AddTrackGroup(group, groupWrapper);
                        wrapper.HasChanged = true;
                    }

                    for (int k = 0; k < group.Children.Count; k++)
                    {
                        NTrack track = group.Children[k];
                        TimelineTrackWrapper trackWrapper = null;
                        if (!groupWrapper.ContainsTrack(track, out trackWrapper))
                        {
                            trackWrapper = new TimelineTrackWrapper(track);
                            groupWrapper.AddTrack(track, trackWrapper);
                            groupWrapper.HasChanged = true;
                        }

                        foreach (NItem item in track.Children)
                        {
                            TimelineItemWrapper itemWrapper = null;
                            if (!trackWrapper.ContainsItem(item, out itemWrapper))
                            {
                                if (item.GetType().IsSubclassOf(typeof(NEvent)))
                                {
                                    itemWrapper = new TimelineItemWrapper(item, item.FireTime);
                                    trackWrapper.AddItem(item, itemWrapper);
                                }
                                else
                                {
                                    NItem action = item as NItem;
                                    itemWrapper = new CinemaActionWrapper(action, action.FireTime, action.Duration);
                                    trackWrapper.AddItem(action, itemWrapper);
                                }
                                trackWrapper.HasChanged = true;
                            }
                            else
                            {
                                if (GUIUtility.hotControl == 0)
                                {
                                    if (itemWrapper.GetType() == (typeof(TimelineItemWrapper)))
                                    {
                                        itemWrapper.Firetime = item.FireTime;
                                    }
                                    else
                                    {

                                        NItem action = item as NItem;
                                        CinemaActionWrapper actionWrapper = itemWrapper as CinemaActionWrapper;
                                        actionWrapper.Firetime = action.FireTime;
                                        actionWrapper.Duration = action.Duration;
                                    }
                                }
                            }
                        }

                        List<Behaviour> itemRemovals = new List<Behaviour>();
                        foreach (Behaviour behaviour in trackWrapper.Behaviours)
                        {
                            bool found = false;
                            foreach (NItem item in track.Children)
                            {
                                if (behaviour.Equals(item))
                                {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found || behaviour == null)
                            {
                                itemRemovals.Add(behaviour);
                            }
                        }
                        foreach (Behaviour item in itemRemovals)
                        {
                            trackWrapper.HasChanged = true;
                            trackWrapper.RemoveItem(item);
                        }
                    }

                    List<Behaviour> removals = new List<Behaviour>();
                    foreach (Behaviour behaviour in groupWrapper.Behaviours)
                    {
                        bool found = false;
                        foreach (NItem track in group.Children)
                        {
                            if (behaviour.Equals(track))
                            {
                                found = true;
                                break;
                            }
                        }
                        if (!found || behaviour == null)
                        {
                            removals.Add(behaviour);
                        }
                    }
                    foreach (Behaviour track in removals)
                    {
                        groupWrapper.HasChanged = true;
                        groupWrapper.RemoveTrack(track);
                    }
                }

                }
                return wrapper;
            }
        
        public static CutsceneWrapper CreateWrapper(NSequencer cutscene)
        {
            CutsceneWrapper wrapper = new CutsceneWrapper(cutscene);
            if (cutscene != null)
            {
                wrapper.RunningTime = cutscene.RunningTime;
                wrapper.Duration    = cutscene.Duration;
                wrapper.IsPlaying   = cutscene.State == CutsceneState.PreviewPlaying ||
                                      cutscene.State == CutsceneState.Playing;
            }
            for(int i=0;i<cutscene.Groups.Count;i++)
            {
                NGroup group = cutscene.Groups[i];
                TrackGroupWrapper groupWrapper = new TrackGroupWrapper(group);
                wrapper.AddTrackGroup(group, groupWrapper);
                for (int k = 0; k < group.Children.Count; k++)
                {
                    NTrack track = group.Children[k];
                    TimelineTrackWrapper trackWrapper = new TimelineTrackWrapper(track);
                    groupWrapper.AddTrack(track, trackWrapper);
                    for (int m = 0; m < track.Children.Count; m++)
                    {
                        NItem item = track.Children[m];
                        if (item.GetType().IsSubclassOf(typeof(NEvent)))
                        {
                            TimelineItemWrapper itemWrapper = new TimelineItemWrapper(item, item.FireTime);
                            trackWrapper.AddItem(item, itemWrapper);
                        }
                        else
                        {
                            CinemaActionWrapper itemWrapper = new CinemaActionWrapper(item, item.FireTime, item.Duration);
                            trackWrapper.AddItem(item, itemWrapper);
                        }

                    }
                }
            }

            return wrapper;
        }

        public static void            ReflectChanges(NSequencer cutscene, CutsceneWrapper wrapper)
        {
            if (cutscene == null || wrapper == null)
            {
                return;
            }
            cutscene.Duration = wrapper.Duration;
        }

        public static List<Type>      GetAllSubTypes(Type parentType)
        {
            List<Type> list = new List<Type>();
            foreach (Assembly a in System.AppDomain.CurrentDomain.GetAssemblies())
            {
                foreach (System.Type type in a.GetTypes())
                {
                    if (type.IsSubclassOf(parentType))
                    {
                        list.Add(type);
                    }
                }
            }
            return list;
        }

        public static TAtr            GetAttribute<TAtr>(Type type) where TAtr : Attribute
        {
            TAtr[] attrs = (TAtr[])type.GetCustomAttributes(typeof(TAtr), false);
            if (attrs != null && attrs.Length > 0)
            {
                TAtr attr = attrs[0];
                return attr;
            }
            return null;
        }
    }
}

