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
    /// 飞机类:所有飞机的共性
    /// </summary>
    public abstract class Plane : GameObject
    {
        public int Life { get; set; }//生命值
        public int Bullet_grade { get; set; }//子弹的等级
        public abstract void Fire();//开火的动作

        public Plane(int x,int y,int speed,int width,int height, Direction dir,int life,int bullet_grade)
            : base(x,y,speed,width,height,dir)
        {
            this.Life = life;
            this.Bullet_grade = bullet_grade;
        }
        public abstract bool Is_destroy();
    }

    public class Myplane : Plane
    {
        private static readonly Image Myimg = Resources.me2;
        private static readonly Image Myimg_go = Resources.me1;
        private static readonly Image Myimg_life = Resources.life;
        private Image Myimg_hurt = Resources.me2_hurt;
        private Image Myimg_go_hurt = Resources.me1_hurt;

        private Image mybomb_Img = Resources.bomb;
        //是否处于受伤无敌状态
        public bool Is_Player_Hurt;
        //用来判断飞机是否前进,控制切换前进图片
        private bool Is_Myplane_go;
        //玩家火力等级
        public int fire_grade { set; get; }
        //玩家得分
        public int Myscore { set; get; }
        //玩家拥有的炸弹数量
        public int Bomb_number { set; get; }
        public DateTime Player_Hurt_Time { set; get; }
        public Myplane(int x, int y, int speed,int life, int bullet_grade) 
            : base(x, y, speed, Myimg.Width/2, Myimg.Height/2, Direction.wait,life,bullet_grade)
        {
            Player_Hurt_Time = DateTime.Now;
            Bomb_number = 3;
            fire_grade = 1;
            Is_Myplane_go = false;
            Is_Player_Hurt = true;
            Myscore = 0;
        }

        public override void Fire()
        { 
        }

        public void Mousefire(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (fire_grade >= 5) fire_grade = 5;
                switch (fire_grade)
                {
                    case 1:
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4-1, e.Y, 12, 1));
                        break;
                    case 2:
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 - 10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 + 10, e.Y, 12, 1));
                        break;
                    case 3:
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 - 5-10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 + 5+10, e.Y, 12, 1));
                        break;
                    case 4:
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 - 10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 + 10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 - 10-10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 + 10+10, e.Y, 12, 1));
                        break;
                    case 5:
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 , e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 - 10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 + 10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1- 10 -10, e.Y, 12, 1));
                        SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(e.X + Myimg.Width / 4 - 1 + 10+10, e.Y, 12, 1));
                        break;
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                if (Bomb_number > 0)
                {
                    Bomb_number--;
                    for (int i = 0; i < 24; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            SingleObject.GetSingle().Playerbullets_object.Add(new Playerbullet(20 * i, GameObject.Form_height+20 * j, 15, 1));
                        }
                    }

                    //清除敌机子弹
                    for (int i = 0; i < SingleObject.GetSingle().Enemybullets_object.Count; i++)
                    {
                        SingleObject.GetSingle().RemoveGameObject(SingleObject.GetSingle().Enemybullets_object[i]);
                        i--;
                    }
                }
            }
        }


        public override void Moving()
        {
            //switch (this.Dir)
            //{
            //    case Direction.up:
            //        this.Y -= this.Speed;
            //        break;
            //    case Direction.down:
            //        this.Y += this.Speed;
            //        break;
            //    case Direction.left:
            //        this.X -= this.Speed;
            //        break;
            //    case Direction.right:
            //        this.X += this.Speed;
            //        break;
            //    case Direction.wait:
            //        break;
            //}
            //if (this.X >= Form_width - Width/2)
            //{
            //    this.X = Form_width - Width/2;
            //}
            //if (this.X <= 0)
            //{
            //    this.X = 0;
            //}
            //if (this.Y >= Form_height - Height/2)
            //{
            //    this.Y = Form_height - Height/2;
            //}
            //if (this.Y <= 0)
            //{
            //    this.Y = 0;
            //}

        }

        public override void Draw(Graphics g)
        {
            if (Is_Myplane_go)//前进图片切换
            {
                if (Is_Player_Hurt)//受伤图片切换
                {
                    g.DrawImage(Myimg_go_hurt, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
                else
                {
                    g.DrawImage(Myimg_go, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
                Console.Write("go");
            }
            else
            {
                if (Is_Player_Hurt)
                {
                    g.DrawImage(Myimg_hurt, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
                else
                {
                    g.DrawImage(Myimg, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
            }
           
            for(int i = 0; i < Life; i++)//生命值绘制
            {
                g.DrawImage(Myimg_life, 0+i*(Myimg_life.Width/2+15),10,Myimg_life.Width/2, Myimg_life.Height/2);
            }
            for(int i = 0; i < Bomb_number; i++)//炸弹数绘制
            {
                g.DrawImage(mybomb_Img, 0 + i * (mybomb_Img.Width / 2 + 10), Myimg_life.Height,
                    mybomb_Img.Width / 2, mybomb_Img.Height / 2);
            }

        }
        //仅是画，不移动，用于pause
        public override void Onlydraw(Graphics g)
        {
            if (Is_Myplane_go)
            {
                if (Is_Player_Hurt)
                {
                    g.DrawImage(Myimg_go_hurt, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
                else
                {
                    g.DrawImage(Myimg_go, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
                Console.Write("go");
            }
            else
            {
                if (Is_Player_Hurt)
                {
                    g.DrawImage(Myimg_hurt, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
                else
                {
                    g.DrawImage(Myimg, base.X, base.Y, Myimg.Width / 2, Myimg.Height / 2);
                }
            }
            for (int i = 0; i < Life; i++)//生命值绘制
            {
                g.DrawImage(Myimg_life, 0 + i * (Myimg_life.Width / 2 + 15), 10, Myimg_life.Width / 2, Myimg_life.Height / 2);
            }
            for (int i = 0; i < Bomb_number; i++)
            {
                g.DrawImage(mybomb_Img, 0 + i * (mybomb_Img.Width / 2 + 10), Myimg_life.Height + 10,
                    mybomb_Img.Width / 2, mybomb_Img.Height / 2);
            }
        }

        public void Mousemove(MouseEventArgs m)
        {
            if (m.Y < Y)
            {
                Is_Myplane_go = true;
            }
            else
            {
                Is_Myplane_go = false;
            }
            X = m.X;
            Y = m.Y;
            if (m.X >= GameObject.Form_width - Width/2)
            {
                X = GameObject.Form_width - Width/2;
            }
            if(m.Y >= GameObject.Form_height - Height/2)
            {
                Y = GameObject.Form_height - Height/2;
            }          
        }
        public override bool Is_destroy()
        {
            if (Life <= 0)
            {
                GameObject.Gameover = true;
            }
            return false;
        }


    }


    public class Enemyplane : Plane
    {

        //统计敌机的数量，方便重新生成下一波
        public static int Enemy_number = 0;
        //怪物等级
        public static int Enemy_grade = 0;

        //敌机图片
        private static readonly Image[] Enemyimg =
            {
            Resources.enemy1,
            Resources.enemy2,
            Resources.enemy3_n1
            };

        //敌机类型
        public int Type { get; set; }
        //控制迂回行走
        private int Control_to_patrol { get; set; }
        //定义卡牌数组
        private int[] card = new int[100];
        //定义随机数(使用计时周期数作为时间种子)
        Random fire_random = new Random((int)DateTime.Now.Ticks);
        //防止洗牌结果重复
        public static int Prevent_repeat = 0;


        public Enemyplane(int x, int y,int type)
            : base(x, y, Type_speed(type), Type_width(type), Type_height(type), Direction.down, Type_life(type), 1)
        {
            this.Type = type;
            Control_to_patrol = 0;
            Enemy_number++;//敌机计数

            //发牌
            for (int i = 0; i < 100; i++)
            {
                card[i] = i;
            }

            //防止卡牌洗出来的结果重复（对首元素操作错开重复）(循环错开)
            //多次洗牌，知道不重复
            do
            {
                //洗牌
                int change_random = 0;
                int change_number = 0;
                for (int j = 0; j < 100; j++)
                {
                    //随机交换位置,达到洗牌效果(其他法:也可以通过范围内随机生成数字,如果与前面相同则再重新生成,,缺点:更加耗时)
                    change_random = fire_random.Next(0, 100);
                    change_number = card[j];
                    card[j] = card[change_random];
                    card[change_random] = change_number;
                }


            } while (Prevent_repeat != card[0]);
            Prevent_repeat=card[0];//洗完牌后赋值，并以此为依据避免下一轮洗牌重复




            ////调试段代码
            //for (int i = 0; i < card.Length; i++)
            //{
            //    Console.Write(card[i]);
            //    Console.Write(" ");
               
            //}
            //Console.WriteLine();
        }
        
        
         
        // 不同敌人的宽度
        private static int Type_width(int type)
        {
            
            switch (type)
            {
                case 1:
                    return Enemyimg[0].Width;
                case 2:
                    return Enemyimg[1].Width;
                case 3:
                    return Enemyimg[2].Width;
            }
            return 0;
        }
        //不同敌人的高度
        private static int Type_height(int type)
        {
            switch (type)
            {
                case 1:
                    return Enemyimg[0].Height;
                case 2:
                    return Enemyimg[1].Height;
                case 3:
                    return Enemyimg[2].Height;
            }
            return 0;
        }
        //不同敌人的速度
        private static int Type_speed(int type)
        {
            switch (type)
            {
                case 1:
                    return 10+(Enemy_grade/2)*3;
                case 2:
                    return 7+ (Enemy_grade / 2) * 3;
                case 3:
                    return 5+ (Enemy_grade / 2) * 3;
            }
            return 0;
        }
        //不同敌人的生命值
        private static int Type_life(int type)
        {
            switch(type)
            {
                case 1:
                    return 2+Enemy_grade*2;
                case 2:
                    return 5+Enemy_grade*5;
                case 3:
                    return 100+Enemy_grade*100;
            }
            return 0;
        }


        //敌人的移动
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
            if (this.Y >= Form_height - Height)
            {
                this.Y = Form_height - Height;
            }
            if (this.Y <= 0)
            {
                this.Y = 0;
            }
            switch (Type)//对不同种类的敌人进行不同的路径规划
            {
                case 1:
                    if (Control_to_patrol == 0 && Y > 200) { Speed -= 1; }
                    if (Control_to_patrol == 0 && Speed <= -10 && Y <= 150) { Control_to_patrol = 1; }
                    else if (Control_to_patrol == 1)
                    {
                        this.Dir = Direction.left;
                        Speed = 10;
                        Control_to_patrol = 2;
                    }
                    else if (Control_to_patrol == 2)
                    {
                        if (X == 0 && this.Dir == Direction.left) { this.Dir = Direction.right; }
                        else if (X == Form_width - Width && this.Dir == Direction.right) { this.Dir = Direction.left; }

                    }
                    break;
                case 2:
                    if (Control_to_patrol == 0 && Y > 200) { Speed -= 1; }
                    if (Control_to_patrol == 0 && Speed <= -7 && Y <= 150) { Control_to_patrol = 1; }
                    else if (Control_to_patrol == 1)
                    {
                        this.Dir = Direction.left;
                        Speed = 7;
                        Control_to_patrol = 2;
                    }
                    else if (Control_to_patrol == 2)
                    {
                        if (X == 0 && this.Dir == Direction.left) { this.Dir = Direction.right; }
                        else if (X == Form_width - Width && this.Dir == Direction.right) { this.Dir = Direction.left; }

                    }
                    break;
                case 3:
                    if (Control_to_patrol == 0 && Y > 50) { Speed -= 1; }
                    if (Control_to_patrol == 0 && Speed <= -5 && Y <= 50) { Control_to_patrol = 1; }
                    else if (Control_to_patrol == 1)
                    {
                        this.Dir = Direction.left;
                        Speed = 5;
                        Control_to_patrol = 2;
                    }
                    else if (Control_to_patrol == 2)
                    {
                        if (X == 0 && this.Dir == Direction.left) { this.Dir = Direction.right; }
                        else if (X == Form_width - Width && this.Dir == Direction.right) { this.Dir = Direction.left; }

                    }
                    break;
            }
            
        }


        public override void Fire()
        {
            //优化一:子弹速度范围随机   优化二:子弹发射时间自创算法突破伪随机
            //伪随机数(在同一时刻它们的随机值都一样),这样会导致多辆敌机同时发子弹,这不是我想要的
            //自创优化算法如下:(结合洗牌算法),进化成三阶的随机数
            //实现了即使同一时间,不同敌机也能得到不同的随机值(避免同发),
            //而且同一敌机在同一时刻不会得到多个相同值(避免混乱连发)
            //总结:共实现了三阶,二阶错开不同敌机的同发,三阶避免了同一敌机的同一时刻连发
            //遇到的问题：可能因为多线程的问题，洗出来的牌有一些会一样，加控制量循环洗牌基本解决问题！！
            //（但有时候还是会同时进入洗牌导致控制量失效）（但基本上已经完成任务）


            //简单来说就是每一个敌机手里的卡牌顺序都不一样,而卡牌顺序关联随机数,因此随机出来的会不一样.
            //把随机到的数字,取对应位置的卡牌,把卡牌值对应的位置的值与当前位置的值交换(第三阶算法)
            int value_random = 0;
            int position = 0;
            int position_number = 0;
            value_random = fire_random.Next(0, 100);
            position = card[value_random];
            position_number = card[position];
            card[position] = card[value_random];
            card[value_random] = position_number;

            //交换完后,取得目标随机值数据为card[value_random],也可以取card[position]
            //这里取card[value_random](范围为0到99)


            //下面实现的是,不同敌机发射子弹的概率不同
            switch (Type)
            {
                case 1:
                    if (card[value_random] < 4)//概率%4
                    {
                        
                        SingleObject.GetSingle().AddGameObject(new Enemybullet(X + Width / 2 + 10, Y + Height, fire_random.Next(7, 12), 1));
                    }
                    break;
                case 2:
                    if (card[value_random] < 4)//概率%4
                    {
                        SingleObject.GetSingle().AddGameObject(new Enemybullet(X + Width / 2 + 10, Y + Height, fire_random.Next(7, 12), 1));
                    }
                    break;
                case 3:
                    if (card[value_random]<4)//概率%4
                    {
                        SingleObject.GetSingle().AddGameObject(new Enemybullet(X + Width / 2 + 10, Y + Height, fire_random.Next(7, 12), 1));
                    }
                    break;
            }
            
        }
        //绘制图片
        public override void Draw(Graphics g)
        {
            this.Fire();
            this.Moving();
            g.DrawImage(Enemyimg[Type-1], base.X, base.Y);
        }
        public override void Onlydraw(Graphics g)
        {
            g.DrawImage(Enemyimg[Type - 1], base.X, base.Y);
        }

        //是否销毁
        public override bool Is_destroy()
        {
            if (Life <= 0)
            {
                Dir = Direction.wait;//停止运动
                //爆炸
                SingleObject.GetSingle().AddGameObject(new Enemybomb(X, Y, Type,this));
                SingleObject.GetSingle().Player.Myscore += Player_Getscore(Type);
                SingleObject.GetSingle().RemoveGameObject(this);
                Enemy_number--;
                return true;
            }
            return false;
        }
        //返回分数
        private int Player_Getscore(int Type)
        {
            switch (Type)
            {
                case 1:
                    return 100+ Enemy_grade*100;
                case 2:
                    return 200+Enemy_grade * 200;
                case 3:
                    return 500+Enemy_grade * 500;
            }
            return 0;
        }

    }


}


