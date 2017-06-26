using UnityEngine;
using System.Collections;

namespace NLE
{
    public interface INItem
    {
        void Init();
        void Begin();
        void Loop  (float time, float delta);
        void Sample(float time);
        void End();
        void Pause();
        void Stop();
        void Resume();

        void Trigger();//正向播放时，到了触发时间调用
        void Reverse();//反向播放时，越过触发时间调用
    }
}

