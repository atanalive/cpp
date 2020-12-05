using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Media;
namespace AirPlane_Fight
{
    //饿汉式
    public class GameMusic
    {
        private static GameMusic GameMusic_object = new GameMusic();
        GameMusic() { }
        public static GameMusic GetGaneMusic_object()
        {
            return GameMusic_object;
        } 
        //播放音乐
        public SoundPlayer Fight_sound = new SoundPlayer(Properties.Resources.ResourceManager.GetStream("fight_music"));
        public SoundPlayer Menu_sound = new SoundPlayer(Properties.Resources.ResourceManager.GetStream("liangy"));
    }
}


