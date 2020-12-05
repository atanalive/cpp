using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace AirPlane_Fight
{
    /// <summary>
    /// 单例设计模式(懒汉式)
    /// 用来管理各种实例化对象
    /// 可以看作实例对象管理器
    /// </summary>
    class SingleObject
    {
        //1.构造函数私有化
        private SingleObject(){ }
        //2.声明全局唯一变量
        private static SingleObject _single = null;
        //3.提供一个静态函数用于返回一个唯一的对象
        public static SingleObject GetSingle()
        {
            if (_single == null)
            {
                _single=new SingleObject();
            }
            return _single;
        }

        /// <summary>
        /// 具体的实例
        /// </summary>

        //背景实例 只需一个
        public Background BK { set; get; }
        //玩家实例
        public Myplane Player { set; get; }
        //敌人的实例列表
        public List<Enemyplane> Enemy_object = new List<Enemyplane>();
        //玩家子弹实例列表
        public List<Playerbullet> Playerbullets_object = new List<Playerbullet>();
        //敌人子弹实例列表
        public List<Enemybullet> Enemybullets_object = new List<Enemybullet>();
        //玩家爆炸实例
        public Playerbomb Playerbomb_object;
        //敌人爆炸实例
        public List<Enemybomb> Enemybomb_object=new List<Enemybomb>();
        //补给实例
        public List<Supply> Supply_object = new List<Supply>();

        /// <summary>
        /// 实例管理方法
        /// </summary>

        //添加实例
        public void AddGameObject(GameObject g_object)
        {
            if(g_object is Background)
            {
                this.BK = g_object as Background;
            }
            else if(g_object is Myplane)
            {
                this.Player = g_object as Myplane;
            }
            else if(g_object is Enemyplane)
            {
                Enemy_object.Add(g_object as Enemyplane);
            }
            else if (g_object is Playerbullet)
            {
                Playerbullets_object.Add(g_object as Playerbullet);
            }
            else if(g_object is Enemybullet)
            {
                Enemybullets_object.Add(g_object as Enemybullet);
            }
            else if(g_object is Enemybomb)
            {
                Enemybomb_object.Add(g_object as Enemybomb);
            }
            else if(g_object is Playerbomb)
            {
                Playerbomb_object = g_object as Playerbomb;
            }
            else if(g_object is Supply)
            {
                Supply_object.Add(g_object as Supply);
            }
        }

        //移除实例
        public void RemoveGameObject(GameObject g_object)
        {
            if(g_object is Enemyplane)
            {
                Enemy_object.Remove(g_object as Enemyplane);
            }
            else if(g_object is Playerbullet)
            {
                Playerbullets_object.Remove(g_object as Playerbullet);
            }
            else if(g_object is Enemybullet)
            {
                Enemybullets_object.Remove(g_object as Enemybullet);
            }
            else if(g_object is Enemybomb)
            {
                Enemybomb_object.Remove(g_object as Enemybomb);
            }
            else if(g_object is Playerbomb)
            {
                Playerbomb_object = null;
            }
            else if(g_object is Myplane)
            {
                Player = null;
            }
            else if(g_object is Supply)
            {
                Supply_object.Remove(g_object as Supply);
            }

        }

        //绘制实例图像
        public void Draw(Graphics g)
        {
            this.BK.Draw(g);
            this.Player.Draw(g);
            if (Playerbomb_object != null)
            {
                this.Playerbomb_object.Draw(g);
                SingleObject.GetSingle().RemoveGameObject(Playerbomb_object);
            }
            for(int i = 0; i < Enemy_object.Count; i++)
            {
                Enemy_object[i].Draw(g);
            }
            for (int i = 0; i < Playerbullets_object.Count; i++)
            {
                Playerbullets_object[i].Draw(g);
            }
            for (int i = 0; i < Enemybullets_object.Count; i++)
            {
                Enemybullets_object[i].Draw(g);
            }
            //把敌机移除函数放在这里会冲突,所以放到了Enemyplane里
            for (int i = 0; i < Enemybomb_object.Count; i++)
            {
                Enemybomb_object[i].Draw(g);
            } 
            for(int i = 0; i < Supply_object.Count; i++)
            {
                Supply_object[i].Draw(g);
            }


            int count = 0;//取位数
            //用于得分位置的自动移位,实现得分位置的正确布局
            for (int remenber= Player.Myscore, i = 0; i <= count; i++)//循环次数是动态的,取决于位数
            {
                if ((remenber / 10) >= 10)
                {
                    count++;
                    remenber = remenber / 10;
                }
            }


            g.DrawString(Player.Myscore.ToString(), new Font("微软雅黑", 20, FontStyle.Italic), Brushes.Gray, new Point(450-count*20 , 0));

        }

        //仅是画，不移动也不开火，用于暂停
        public void Onlydraw(Graphics g)
        {
            this.BK.Onlydraw(g);
            this.Player.Onlydraw(g);
            if (Playerbomb_object != null)
            {
                this.Playerbomb_object.Onlydraw(g);
                SingleObject.GetSingle().RemoveGameObject(Playerbomb_object);
            }
            for (int i = 0; i < Enemy_object.Count; i++)
            {
                Enemy_object[i].Onlydraw(g);
            }
            for (int i = 0; i < Playerbullets_object.Count; i++)
            {
                Playerbullets_object[i].Onlydraw(g);
            }
            for (int i = 0; i < Enemybullets_object.Count; i++)
            {
                Enemybullets_object[i].Onlydraw(g);
            }
            for (int i = 0; i < Enemybomb_object.Count; i++)
            {
                Enemybomb_object[i].Onlydraw(g);
            }
            for(int i = 0; i < Supply_object.Count; i++)
            {
                Supply_object[i].Onlydraw(g);
            }
        }

        //碰撞检测
        public void CollisionDetection()
        {
            //判断玩家子弹与敌人是否碰撞
            for(int i=0;i< Playerbullets_object.Count; i++)
            {
                for(int j=0; j < Enemy_object.Count; j++)
                {
                    //如果两矩形碰撞
                    if (Playerbullets_object[i].GetRectangle().IntersectsWith(Enemy_object[j].GetRectangle()))
                    {
                        Enemy_object[j].Life -= Playerbullets_object[i].Power;
                        SingleObject.GetSingle().RemoveGameObject(Playerbullets_object[i]);
                        Enemy_object[j].Is_destroy();
                        //巧妙利用i--保证循环继续进行
                        i--;
                        break;
                    }
                }
            }

            //判断玩家是否受伤
            DateTime player_start = DateTime.Now;
            TimeSpan player_spand = player_start - Player.Player_Hurt_Time;
            if (player_spand.Seconds>2)//玩家受伤后两秒内处于无敌状态
            {
               
                Player.Is_Player_Hurt = false;
                //玩家与敌人是否碰撞
                for (int i = 0; i < Enemy_object.Count; i++)
                {
                    if (Player.GetRectangle().IntersectsWith(Enemy_object[i].GetRectangle()))
                    {
                        Player.Life--;
                        Player.Is_Player_Hurt = true;//玩家受伤,获得无敌时间
                        Enemy_object[i].Life--;
                        Player.Is_destroy();//判断玩家是否死亡
                        Enemy_object[i].Is_destroy();//判断敌机是否死亡
                        Player.Player_Hurt_Time = DateTime.Now;//重置受伤时间
                        Player.fire_grade = 1;//丢失子弹装备
                        Console.Write("hurt1");//调试用
                        break;
                    }
                }

                //判断敌方子弹是否与玩家碰撞
                for (int i = 0; i < Enemybullets_object.Count; i++)
                {
                    if (Player.GetRectangle().IntersectsWith(Enemybullets_object[i].GetRectangle()))
                    {
                        SingleObject.GetSingle().RemoveGameObject(Enemybullets_object[i]);
                        Player.Life--;
                        Player.Is_destroy();
                        Player.Is_Player_Hurt = true;
                        Player.Player_Hurt_Time = DateTime.Now;
                        Player.fire_grade = 1;//丢失子弹装备
                        Console.Write("hurt2");
                        break;
                    }
                }

            }

            
            //判断补给与玩家是否碰撞
            for (int i = 0; i < Supply_object.Count; i++)
            {
                if (Supply_object[i].GetRectangle().IntersectsWith(Player.GetRectangle()))
                {
                    Console.Write("补给检测");
                    switch (Supply_object[i].Supply_Type)
                    {
                        case 1:
                            Player.fire_grade++;
                            Console.WriteLine();
                            Console.Write(Player.fire_grade);
                            SingleObject.GetSingle().RemoveGameObject(Supply_object[i]);
                            i--;
                            break;
                        case 2:
                            Player.Bomb_number++;
                            if (Player.Bomb_number >= 5) Player.Bomb_number = 5;//控制上限
                            SingleObject.GetSingle().RemoveGameObject(Supply_object[i]);
                            i--;
                            break;
                    }
                }
            }



        }

    }
}
