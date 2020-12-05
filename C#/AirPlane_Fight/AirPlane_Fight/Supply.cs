using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AirPlane_Fight.Properties;
namespace AirPlane_Fight
{
    /// <summary>
    /// 空降补给
    /// </summary>
    public class Supply : GameObject
    {
        private static Image Bullet_Supply_Img = Resources.bullet_supply;
        private static Image Bomb_Supply_Img = Resources.bomb_supply;
        public int Supply_Type;
        public Supply(int x,int y,int speed, int type) : base(x,y,speed, TypeofSupply_width(type), TypeofSupply_height(type), Direction.down)
        {
            Supply_Type = type;
        }

        public override void Draw(Graphics g)
        {
            this.Moving();
            switch (Supply_Type)
            {
                case 1:
                    g.DrawImage(Bullet_Supply_Img, X, Y, Bullet_Supply_Img.Width / 2, Bullet_Supply_Img.Height / 2);
                    break;
                case 2:
                    g.DrawImage(Bomb_Supply_Img, X, Y, Bomb_Supply_Img.Width / 2, Bomb_Supply_Img.Height / 2);
                    break;
            }
           
        }
        public static int TypeofSupply_width(int Supply_Type)
        {
            switch (Supply_Type)
            {
                case 1:
                    return Bullet_Supply_Img.Width;
                case 2:
                    return Bomb_Supply_Img.Width;
            }
            return 0;
        }
        public static int TypeofSupply_height(int Supply_Type)
        {
            switch (Supply_Type)
            {
                case 1:
                    return Bullet_Supply_Img.Height;
                case 2:
                    return Bomb_Supply_Img.Height;
            }
            return 0;
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
            if (this.X >= Form_width - Width)
            {
                this.X = Form_width - Width;
            }
            if (this.X <= 0)
            {
                this.X = 0;
            }
            if (this.Y >= Form_height)
            {
                SingleObject.GetSingle().RemoveGameObject(this);
            }
            if (this.Y <= 0)
            {
                this.Y = 0;
            }
        }

        public override void Onlydraw(Graphics g)
        {
            switch (Supply_Type)
            {
                case 1:
                    g.DrawImage(Bullet_Supply_Img, Bullet_Supply_Img.Width / 2, Bullet_Supply_Img.Height / 2);
                    break;
                case 2:
                    g.DrawImage(Bomb_Supply_Img, Bomb_Supply_Img.Width / 2, Bomb_Supply_Img.Height / 2);
                    break;
            }
        }
        public override Rectangle GetRectangle()
        {
            //这里是为了让玩家碰到伞的时候不能拾起，只有真正碰到补给矩形才能拾起
            switch (Supply_Type)
            {
                case 1:
                    return new Rectangle(X, Y+25, Width, Height);
                case 2:
                    return new Rectangle(X, Y+30, Width, Height);
            }
            return new Rectangle(X, Y, Width, Height);
        }

    }
}
