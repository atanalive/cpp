using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using AirPlane_Fight.Properties;
using System.Threading;
using System.Media;

namespace AirPlane_Fight
{
    public partial class Gameform : Form
    {
        public Gameform()
        {
            InitializeComponent();
        }

        static int boos_control = 0;
        //游戏绘制
        private void Gameform_Paint(object sender, PaintEventArgs e)
        {
            if (GameMenu.GetGameMenuObject().Is_menu_interface)
            {
                GameMenu.GetGameMenuObject().Draw(e.Graphics);
            }
            else if (!GameObject.Pause && !GameObject.Gameover)
            {
                SingleObject.GetSingle().Draw(e.Graphics);//绘制游戏对象
                SingleObject.GetSingle().CollisionDetection();//碰撞检测

                if (Enemyplane.Enemy_number == 0)//重新生成敌人
                {
                    //不同等级的敌机生成概率不一样
                    for (int i = 0; i < 4; i++)
                    {
                        if (r.Next(0, 100) < 80)
                        {
                            SingleObject.GetSingle().AddGameObject(new Enemyplane(r.Next(0, GameObject.Form_width), -200, 1));
                        }
                        else
                        {
                            SingleObject.GetSingle().AddGameObject(new Enemyplane(r.Next(0, GameObject.Form_width), -200, 2));

                        }
                    }
                    boos_control++;
                    //控制boos在每几波生成一次:10,放在这里避免第一波生成boos
                    if (boos_control % 10 == 0)
                    {
                        Enemyplane.Enemy_grade++;//怪物等级提升
                        SingleObject.GetSingle().AddGameObject(new Enemyplane(r.Next(0, GameObject.Form_width), -200, 3));
                    }
                }

            }
            else if (GameObject.Pause && !GameObject.Gameover)
            {

                SingleObject.GetSingle().Onlydraw(e.Graphics);
                e.Graphics.DrawImage(Resources.pause, 0, 0, 480, 700);
                //e.Graphics.DrawImage(Resources.gameover, 100, 400,300,100);
            }
            else if (GameObject.Gameover)
            {
                //绘制游戏图形不移动
                SingleObject.GetSingle().Onlydraw(e.Graphics);
                //绘制游戏结束文字图片
                e.Graphics.DrawImage(Resources.gameovertext, 0, 0, 480, 700);
                //绘制分数
                e.Graphics.DrawString("SCORE:", new Font("微软雅黑", 30, FontStyle.Regular), Brushes.Gray, new Point(150, 400));
                e.Graphics.DrawString(SingleObject.GetSingle().Player.Myscore.ToString(), new Font("微软雅黑", 30, FontStyle.Italic), Brushes.Gray, new Point(180- Digit_calculate()*10, 450));
                //显示两个按钮：Again  menu
                Again_Button.Show();
                Menu_Button.Show();
            }


        }

        private int Digit_calculate()
        {
            int count = 0;//取位数
            //用于得分位置的自动移位,实现得分位置的正确布局
            //循环次数是动态的,取决于位数
            for (int remenber = SingleObject.GetSingle().Player.Myscore, i = 0; i <= count; i++)
            {
                if ((remenber / 10) >= 10)
                {
                    count++;
                    remenber = remenber / 10;
                }
            }
            return count;
        }

        private void Gameform_Load(object sender, EventArgs e)
        {
            InitGame();
            pause_button.Hide();
            Again_Button.Hide();
            Menu_Button.Hide();
        }

        Random r = new Random((int)DateTime.Now.Ticks);
        //初始化游戏对象
        public void InitGame()
        {
            //播放菜单声音
            GameMusic.GetGaneMusic_object().Menu_sound.PlayLooping();
            //初始化背景
            SingleObject.GetSingle().AddGameObject(new Background(0, -700, 10));
            //初始化玩家
            SingleObject.GetSingle().AddGameObject(new Myplane(150, 250, 20, 3, 1));
        }


        private void Timer_draw_Tick(object sender, EventArgs e)
        {
            //刷新窗口
            this.Invalidate();
        }

        private void Gameform_MouseMove(object sender, MouseEventArgs e)
        {
            SingleObject.GetSingle().Player.Mousemove(e);

        }

        private void Gameform_MouseDown(object sender, MouseEventArgs e)
        {
            if (!GameObject.Pause)
            {
                SingleObject.GetSingle().Player.Mousefire(e);
            }


        }

        private void Start_button_Click(object sender, EventArgs e)
        {

            //用show重新恢复按钮
            //Start_button.Enabled = false;
            //button1.Visible = false;//内存一直在
            Start_button.Hide();//内存会暂时收回

            //菜单界面是否显示---》false
            GameMenu.GetGameMenuObject().Is_menu_interface = false;

            //关闭菜单声音，开启游戏声音
            GameMusic.GetGaneMusic_object().Menu_sound.Stop();
            GameMusic.GetGaneMusic_object().Fight_sound.PlayLooping();

            //显示暂停按钮
            //pause_button.Hide();
            //pause_button.Enabled=false;

            pause_button.Show();

        }


        public static int pause_button_control = 0;
        private void pause_button_Click(object sender, EventArgs e)
        {
            pause_button_control++;
            Image Pause_Pressed_Img = Resources.pause_pressed;
            Image Resume_Pressed_Img = Resources.resume_pressed;
            if (pause_button_control % 2 == 0)//非暂停模式
            {
                pause_button.BackgroundImage = Pause_Pressed_Img;
                GameMusic.GetGaneMusic_object().Fight_sound.PlayLooping();
                GameObject.Pause = false;
                Again_Button.Hide();
                Menu_Button.Hide();
            }
            else//暂停模式
            {
                pause_button.BackgroundImage = Resume_Pressed_Img;
                GameObject.Pause = true;
                Again_Button.Show();
                Menu_Button.Show();
                GameMusic.GetGaneMusic_object().Fight_sound.Stop();
                
            }

        }

        private void pause_button_MouseEnter(object sender, EventArgs e)
        {
            Image Pause_Pressed_Img = Resources.pause_pressed;
            Image Resume_Pressed_Img = Resources.resume_pressed;
            if (pause_button_control % 2 == 0)
            {
                pause_button.BackgroundImage = Pause_Pressed_Img;
            }
            else
            {
                pause_button.BackgroundImage = Resume_Pressed_Img;
            }

        }

        private void pause_button_MouseLeave(object sender, EventArgs e)
        {
            Image Pause_Img = Resources.pause_nor;
            Image Resume_Img = Resources.resume_nor;

            if (pause_button_control % 2 == 0)
            {
                pause_button.BackgroundImage = Pause_Img;
            }
            else
            {
                pause_button.BackgroundImage = Resume_Img;
            }
        }

        private void Menu_Button_Click(object sender, EventArgs e)
        {
            Init_Again();//清除旧场面
            GameMenu.GetGameMenuObject().Is_menu_interface = true;//开启菜单开关
            pause_button.BackgroundImage = Resources.pause_nor;//把暂停键的图片还原
            GameObject.Gameover = false;//解除游戏结束标志
            GameObject.Pause = false;//解除暂停
            pause_button_control=0;
            pause_button.Hide();
            Menu_Button.Hide();
            Again_Button.Hide();
            Start_button.Show();
        }

        private void Again_Button_Click(object sender, EventArgs e)
        {

            Init_Again();//再次初始化
            //关闭初始化声音，开启战斗声音
            GameMusic.GetGaneMusic_object().Menu_sound.Stop();
            GameMusic.GetGaneMusic_object().Fight_sound.PlayLooping();

            GameObject.Gameover = false;//清除结束标志 
            pause_button_control =0;//清除暂停状态计数
            GameObject.Pause = false;//退出暂停状态

            

            //pause_button.Hide();
            Menu_Button.Hide();
            Again_Button.Hide();
        }


        private void Init_Again()
        {
            //清除敌机子弹
            for (int i = 0; i < SingleObject.GetSingle().Enemybullets_object.Count; i++)
            {
                SingleObject.GetSingle().RemoveGameObject(SingleObject.GetSingle().Enemybullets_object[i]);
                i--;
            }
            //清除敌机
            for (int i = 0; i < SingleObject.GetSingle().Enemy_object.Count; i++)
            {
                SingleObject.GetSingle().RemoveGameObject(SingleObject.GetSingle().Enemy_object[i]);
                i--;
            }
            //清除玩家子弹
            for (int i = 0; i < SingleObject.GetSingle().Playerbullets_object.Count; i++)
            {
                SingleObject.GetSingle().RemoveGameObject(SingleObject.GetSingle().Playerbullets_object[i]);
                i--;
            }
            //清除补给
            for (int i = 0; i < SingleObject.GetSingle().Supply_object.Count; i++)
            {
                SingleObject.GetSingle().RemoveGameObject(SingleObject.GetSingle().Supply_object[i]);
                i--;
            }

            //初始化敌机
            boos_control = 0;//初始化波次
            Enemyplane.Enemy_grade = 0;//初始化等级

            //SingleObject.GetSingle().RemoveGameObject(SingleObject.GetSingle().Player);
            InitGame();
            Enemyplane.Enemy_number = 0;//把敌人数目归零，便于重新开始生成敌人
        }

    }
}
