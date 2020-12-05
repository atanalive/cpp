using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

//属性的快捷键prop+ tab+tab
namespace AirPlane_Fight
{
    
    public enum Direction//方向枚举
    {
        up,
        down,
        left,
        right,
        wait
    }
    /// <summary>
    /// 项目类:这里写整个项目对象的共性
    /// </summary>
    public abstract class GameObject
    {
        //游戏的暂停
        public static bool Pause = false;
        //游戏的结束
        public static bool Gameover = false;

        //窗口的宽高
        public static int Form_width;
        public static int Form_height;
        public int X { get; set; }//X坐标
        public int Y { get; set; }//Y坐标
        public int Speed { get; set; }//速度
        public int Width { get; set; }//宽度
        public int Height { get; set; }//高度
        public Direction Dir { get; set; } //方向

        public virtual void Moving()//移动的动作
        {
            switch (this.Dir)
            {
                case Direction.up:
                    this.Y -= this.Speed;
                    break;
                case Direction.down:
                    this.Y += this.Speed;
                    break;
                case Direction.left:
                    this.X -= this.Speed;
                    break;
                case Direction.right:
                    this.X += this.Speed;
                    break;
            }
            if (this.X >= Form_width-Width)
            {
                this.X = Form_width-Width;
            }
            if (this.X <= 0)
            {
                this.X = 0;
            }
            if (this.Y >= Form_height-Height)
            {
                this.Y = Form_height - Height;
            }
            if (this.Y <= 0)
            {
                this.Y = 0;
            }
        }

        public abstract void Onlydraw(Graphics g);//仅是画，不移动也不开火
        public abstract void Draw(Graphics g);//画图

        ////其他无关类用构造方法
        //public GameObject() { }


        //飞机子弹的子类用构造方法
        public GameObject(int x,int y,int speed,int width,int height, Direction dir)
        {
            this.X = x;
            this.Y = y;
            this.Speed = speed;
            this.Width = width;
            this.Height = height;
            this.Dir = dir;
        }

        //背景专用构造方法
        public GameObject(int x, int y, int speed,int width, int height, Direction dir,int form_width, int form_height)
        {
            this.X = x;
            this.Y = y;
            this.Speed = speed;
            this.Width = width;
            this.Height = height;
            this.Dir = dir;
            GameObject.Form_width = form_width;
            GameObject.Form_height = form_height;
        }

        //为边缘检测提供矩形
        public virtual Rectangle GetRectangle()
        {
            return new Rectangle(X, Y, Width, Height);
            
        }
    }

}



