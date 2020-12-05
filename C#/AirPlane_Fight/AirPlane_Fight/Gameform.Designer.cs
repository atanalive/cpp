namespace AirPlane_Fight
{
    partial class Gameform
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.Timer_draw = new System.Windows.Forms.Timer(this.components);
            this.Start_button = new System.Windows.Forms.Button();
            this.Menu_Button = new System.Windows.Forms.Button();
            this.Again_Button = new System.Windows.Forms.Button();
            this.pause_button = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // Timer_draw
            // 
            this.Timer_draw.Enabled = true;
            this.Timer_draw.Interval = 50;
            this.Timer_draw.Tick += new System.EventHandler(this.Timer_draw_Tick);
            // 
            // Start_button
            // 
            this.Start_button.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.Start_button.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Start_button.Cursor = System.Windows.Forms.Cursors.Default;
            this.Start_button.FlatAppearance.BorderColor = System.Drawing.SystemColors.GrayText;
            this.Start_button.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.GrayText;
            this.Start_button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.Start_button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Start_button.Font = new System.Drawing.Font("Ink Free", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Start_button.ForeColor = System.Drawing.SystemColors.GrayText;
            this.Start_button.Location = new System.Drawing.Point(177, 412);
            this.Start_button.Name = "Start_button";
            this.Start_button.Size = new System.Drawing.Size(102, 36);
            this.Start_button.TabIndex = 0;
            this.Start_button.Text = "Start";
            this.Start_button.UseVisualStyleBackColor = false;
            this.Start_button.Click += new System.EventHandler(this.Start_button_Click);
            // 
            // Menu_Button
            // 
            this.Menu_Button.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.Menu_Button.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Menu_Button.Cursor = System.Windows.Forms.Cursors.Default;
            this.Menu_Button.FlatAppearance.BorderColor = System.Drawing.SystemColors.GrayText;
            this.Menu_Button.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.GrayText;
            this.Menu_Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.Menu_Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Menu_Button.Font = new System.Drawing.Font("Ink Free", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Menu_Button.ForeColor = System.Drawing.SystemColors.GrayText;
            this.Menu_Button.Location = new System.Drawing.Point(100, 350);
            this.Menu_Button.Name = "Menu_Button";
            this.Menu_Button.Size = new System.Drawing.Size(102, 36);
            this.Menu_Button.TabIndex = 2;
            this.Menu_Button.Text = "Menu";
            this.Menu_Button.UseVisualStyleBackColor = false;
            this.Menu_Button.Click += new System.EventHandler(this.Menu_Button_Click);
            // 
            // Again_Button
            // 
            this.Again_Button.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.Again_Button.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Again_Button.Cursor = System.Windows.Forms.Cursors.Default;
            this.Again_Button.FlatAppearance.BorderColor = System.Drawing.SystemColors.GrayText;
            this.Again_Button.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.GrayText;
            this.Again_Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.Again_Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Again_Button.Font = new System.Drawing.Font("Ink Free", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Again_Button.ForeColor = System.Drawing.SystemColors.GrayText;
            this.Again_Button.Location = new System.Drawing.Point(267, 350);
            this.Again_Button.Name = "Again_Button";
            this.Again_Button.Size = new System.Drawing.Size(102, 36);
            this.Again_Button.TabIndex = 3;
            this.Again_Button.Text = "Again";
            this.Again_Button.UseVisualStyleBackColor = false;
            this.Again_Button.Click += new System.EventHandler(this.Again_Button_Click);
            // 
            // pause_button
            // 
            this.pause_button.BackColor = System.Drawing.Color.Transparent;
            this.pause_button.BackgroundImage = global::AirPlane_Fight.Properties.Resources.pause_nor;
            this.pause_button.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pause_button.Cursor = System.Windows.Forms.Cursors.PanNE;
            this.pause_button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.pause_button.Location = new System.Drawing.Point(430, 656);
            this.pause_button.Name = "pause_button";
            this.pause_button.Size = new System.Drawing.Size(38, 32);
            this.pause_button.TabIndex = 1;
            this.pause_button.UseVisualStyleBackColor = false;
            this.pause_button.Click += new System.EventHandler(this.pause_button_Click);
            this.pause_button.MouseEnter += new System.EventHandler(this.pause_button_MouseEnter);
            this.pause_button.MouseLeave += new System.EventHandler(this.pause_button_MouseLeave);
            // 
            // Gameform
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.ClientSize = new System.Drawing.Size(480, 700);
            this.Controls.Add(this.Again_Button);
            this.Controls.Add(this.Menu_Button);
            this.Controls.Add(this.pause_button);
            this.Controls.Add(this.Start_button);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Gameform";
            this.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.Text = "Gameform";
            this.Load += new System.EventHandler(this.Gameform_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Gameform_Paint);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Gameform_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Gameform_MouseMove);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button Start_button;
        private System.Windows.Forms.Button pause_button;
        private System.Windows.Forms.Button Menu_Button;
        private System.Windows.Forms.Button Again_Button;
        public System.Windows.Forms.Timer Timer_draw;
    }
}

