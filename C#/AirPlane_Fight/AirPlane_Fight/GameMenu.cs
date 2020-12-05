using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using AirPlane_Fight.Properties;
using System.Windows.Forms;

namespace AirPlane_Fight
{

    /// <summary>
    /// 饿汉式单例
    /// </summary>
    public class GameMenu
    {
        //饿汉式
        private static GameMenu GameMenu_object = new GameMenu();

        //是否进入菜单界面控制量
        public bool Is_menu_interface { get; set; }
        //图片
        private Image Menu_Img = Resources.menu_little;

        //开始图片坐标
        private const int start_x= 155;
        private const int start_y = 400;

        public GameMenu()
        {
            Is_menu_interface = true;
        }
        //得到单例
        public static GameMenu GetGameMenuObject()
        {
            return GameMenu_object;
        }

        public void Draw(Graphics g)
        {
            //画菜单背景
            g.DrawImage(Menu_Img, 0, 0);
        }

    }
}
