using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using AirPlane_Fight.Properties;
using System.Runtime;
using System.Windows.Forms;


namespace AirPlane_Fight
{
    public abstract class Bomb:GameObject
    {
        public Bomb(int x,int y):base(x,y,0,0,0,Direction.wait)
        {
            this.X = x;
            this.Y = y;
        }

        public static bool delay_ms(int milliseconds)
        {
            DateTime start_time = DateTime.Now;
            int go_time;
            do
            {
                TimeSpan spand = DateTime.Now - start_time;
                go_time = spand.Milliseconds;
                Application.DoEvents();

            } while (go_time > milliseconds);
            return true;
        }

        public override void Onlydraw(Graphics g) { }
    }

    public class Playerbomb : Bomb
    {
        private Image[] Playerbomb_Img =
        {
            Resources.me_destroy_1,
            Resources.me_destroy_2,
            Resources.me_destroy_3,
            Resources.me_destroy_4
        };

        public Playerbomb(int x,int y) :base(x,y)
        {
            
        }

        public override void Draw(Graphics g)
        {
            for(int i = 0; i < Playerbomb_Img.Length; i++)
            {
                g.DrawImage(Playerbomb_Img[i], X, Y);
                delay_ms(1);
            }
        }

        public override void Moving() { }

    }

    public class Enemybomb : Bomb
    {

        //爆炸图片
        public static Image[] Enemy1bomb_Img =
        {
           Resources.enemy1_down1,Resources.enemy1_down2,Resources.enemy1_down3,Resources.enemy1_down4
        };
        public static Image[] Enemy2bomb_Img =
        {
            Resources.enemy2_down1,Resources.enemy2_down2,Resources.enemy2_down3,Resources.enemy2_down4
        };
        public static Image[] Enemy3bomb_Img =
        {
            Resources.enemy3_down1,Resources.enemy3_down2,Resources.enemy3_down3,Resources.enemy3_down4,
           Resources.enemy3_down5,Resources.enemy3_down6
        };

        //爆炸敌机类型
        public int Bomb_Type { get; set; }

        public Enemybomb(int x,int y,int type,Enemyplane enemy_destroy) : base(x, y)
        {
            Bomb_Type = type;
        }

        Random r = new Random();
        //爆炸图片绘制
        public override void Draw(Graphics g)
        {
            switch (Bomb_Type)
            {
                case 1:
                    for (int i = 0; i < Enemy1bomb_Img.Length; i++)
                    {
                        g.DrawImage(Enemy1bomb_Img[i], X, Y);

                        delay_ms(5);
                    }
                    break;
                case 2:
                    for (int i = 0; i < Enemy2bomb_Img.Length; i++)
                    {
                        g.DrawImage(Enemy2bomb_Img[i], X, Y);

                        delay_ms(5);
                    }
                    break;
                case 3:
                    for (int i = 0; i < Enemy3bomb_Img.Length; i++)
                    {
                        g.DrawImage(Enemy3bomb_Img[i], X, Y);
                        delay_ms(5);
                    }
                    break;
            }
            //道具的爆率
            if (r.Next(0, 99) < 5+Enemyplane.Enemy_grade)
            {
                SingleObject.GetSingle().AddGameObject(new Supply(X, Y, 5, 1));//子弹补给
            }
            if (r.Next(0, 99) > 96- Enemyplane.Enemy_grade)
            {
                SingleObject.GetSingle().AddGameObject(new Supply(X, Y, 5, 2));//炸弹补给
            }
            SingleObject.GetSingle().RemoveGameObject(this);

        }

        public override void Moving() { }
    }


}

