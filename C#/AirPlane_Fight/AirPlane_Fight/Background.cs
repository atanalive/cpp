using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using AirPlane_Fight.Properties;
namespace AirPlane_Fight
{
    class Background:GameObject
    {
        //图片导入区
        private static Image img = Resources.background_long;

        //方向
        private static Direction BK_dir = Direction.down;
        public Background(int x,int y,int speed) 
            : base(x, y, speed,img.Width,img.Height, BK_dir, img.Width,img.Height/2)//第二种构造方法
        {
        }
        public override void Moving()
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
            if (this.Y >= 0)
            {
                this.Y -= GameObject.Form_height/2;
            }
        }

        public override void Draw(Graphics g)
        {
            this.Moving();
            g.DrawImage(img, X, Y);

        }
        public override void Onlydraw(Graphics g)
        {
            g.DrawImage(img, X, Y);
        }

    }
}
