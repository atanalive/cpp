using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AirPlane_Fight.Properties;

namespace AirPlane_Fight
{
    public abstract class Bullet : GameObject
    {
        public int Power { get; set; }
        public Bullet(int x,int y,int speed,int width,int height,Direction dir,int power):base(x,y,speed,width,height,dir)
        {
            this.Power = power;
        }

    }

    public class Playerbullet:Bullet
    {
        private static Image Mybulletimg = Resources.bullet1;

        public Playerbullet(int x,int y,int speed,int power):base(x,y,speed,Mybulletimg.Width,Mybulletimg.Height,Direction.up,power)
        {
        }

        public override void Draw(Graphics g)
        {
            this.Moving();
            g.DrawImage(Mybulletimg, X, Y);

        }
        
        public override void Onlydraw(Graphics g)
        {
            g.DrawImage(Mybulletimg, X, Y);
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
            if (Y < 0)
            {
                SingleObject.GetSingle().RemoveGameObject(this);
            }
        }
       
    }
    public class Enemybullet : Bullet
    {
        private static Image Enemybulletimg = Resources.bullet2;
        public Enemybullet(int x, int y, int speed,int power) : base(x, y, speed, Enemybulletimg.Width, Enemybulletimg.Height, Direction.down,power)
        {

        }
        public override void Draw(Graphics g)
        {
            this.Moving();
            g.DrawImage(Enemybulletimg, X, Y);
        }
        public override void Onlydraw(Graphics g)
        {
            g.DrawImage(Enemybulletimg, X, Y);
        }
        public override void Moving()
        {
            switch (this.Dir)
            {
                case Direction.up:
                    this.Y -= Speed;
                    break;
                case Direction.down:
                    this.Y += Speed;
                    break;
                case Direction.left:
                    this.X -= Speed;
                    break;
                case Direction.right:
                    this.X += Speed;
                    break;
            }
            if (Y > GameObject.Form_height)
            {
                SingleObject.GetSingle().RemoveGameObject(this);
            }
        }

      

    }

}


