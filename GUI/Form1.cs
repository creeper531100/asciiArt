using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.Size = new Size(525, 426);
        }
        List<TrackBar> trackVal = new List<TrackBar>();
        private void anyTrackBar_ValueChanged(object sender, EventArgs e)
        {
            trackVal = groupBox2.Controls.OfType<TrackBar>().ToList();
            var textVal = groupBox2.Controls.OfType<TextBox>().ToList();
            for (var i = 0; i < textVal.Count; i++)
                textVal[i].Text = trackVal[i].Value.ToString();
        }

        private void anyTextBox_TextChanged(object sender, EventArgs e)
        {
            trackVal = groupBox2.Controls.OfType<TrackBar>().ToList();
            var textVal = groupBox2.Controls.OfType<TextBox>().ToList();
            for (var i = 0; i < textVal.Count; i++)
                try
                {
                    trackVal[i].Value = Convert.ToInt32(textVal[i].Text);
                }
                catch
                {
                    trackVal[i].Value = 0;
                }
        }

        private void anyRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            var radioButton = sender as RadioButton;

            List<string> info = new List<string>();
            info.Add("紀錄圖片相似度");
            info.Add("比較圖片相似度");
            info.Add("可以即時預覽影片，但沒有聲音");
            info.Add("快速匯出影片，檔案較大，文字較醜");
            info.Add("匯出彩色影片");
            info.Add("匯出單一文字的彩色影片，不受亮度影響");
            info.Add("使用OpenCV dnn 來自動更換臉部");
            info.Add("在攝影解析度較低時建議用的汽車辨識");
            info.Add("解析度較高時建議使用，能精準的辨識車輛");
            info.Add("使用點字進行即時預覽影片，但沒有聲音");
            info.Add("使用較為漂亮的新細明體匯出影片，檔案較小，文字較漂亮");
            info.Add("將點字影片匯出");
            info.Add("將點字影片匯出，但依輪廓繪製");

            if (radioButton.Checked)
            {
                textBox1.Text = info[radioButton.TabIndex];
            }
        }

        private void TabControl1_Selected(object sender, EventArgs e)
        {
            if (tabControl1.SelectedIndex == 0)
                this.Size = new Size(525, 426);
            else if (tabControl1.SelectedIndex == 1)
                this.Size = new Size(589, 696);
        }

        private String path = "";
        private int select = 0;


        private void button1_Click(object sender, EventArgs e)
        {
            var checkedButton = groupBox1.Controls.OfType<RadioButton>().FirstOrDefault(r => r.Checked);
            select = checkedButton.TabIndex;
            var selectResult = MessageBox.Show("你要導入影片還是下載影片\n(是).導入影片\n(否).下載影片", "提示", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Information);
            if (selectResult == DialogResult.Yes)
            {
                OpenFileDialog dialog = new OpenFileDialog();
                if (dialog.ShowDialog() == DialogResult.OK)
                    path = dialog.FileName;
            }
            else if (selectResult == DialogResult.No)
            {
                path = Interaction.InputBox("請輸入網址", "輸入");
            }
            label1.Text = "學習.exe " + select + " " + path;

            if (@select == 6)
            {
                var faceResult = MessageBox.Show("你要更換臉部嗎?\n(是).更換\n(否).維持原樣", "提示", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Information);
                if (faceResult == DialogResult.Yes)
                {
                    OpenFileDialog dialog = new OpenFileDialog();
                    if (dialog.ShowDialog() == DialogResult.OK)
                    {
                        File.Delete("face.png");
                        File.Copy(dialog.FileName, "face.png");
                    }
                }
            }

            string strCmdText = "/c start 學習.exe " + select + " " + path;
            Process.Start("cmd.exe", strCmdText);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Process.Start("cmd.exe", "/c start output.mp4");
        }


        private void button2_Click(object sender, EventArgs e)
        {
            PackJson packJson = new PackJson();
            if (trackVal.Any())
            {
                textBox1.Text = "輸出: " + packJson.JsonPack(trackVal);
                MessageBox.Show("成功套用設定", "成功", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
                MessageBox.Show("未套用設定", "錯誤", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            int[] array = { 0, 255, 0, 1, 20, 20, 5, 5, 5, 8, 100, 1, 30, 5, 600, 0, 560, 520, 55, 55 };
            for (var i = 0; i < array.Length; i++)
                trackVal[i].Value = array[i];
        }

        private void button4_Click(object sender, EventArgs e)
        {
            MessageBox.Show("最低偵測寬度、高度: 使用者設定偵測汽車大小的最低閾值。\n\n起始、終點觸發高度: 觸發計數的高度，Y軸0代表視窗頂端，數值越大越接近底端。X軸0代表視窗左邊，數值越大越右邊\n\n起始、終點線位置: 顯示觸發線的長度。\n\n學習張數: OpenCV判斷使用幾幀來構建背景模型。\n\n光影敏感度: 判斷與上幀的背景模型的敏感度，數值越高雜訊越高。\n\n影子偵測: 0關閉1開啟，判斷是否要偵測影子的開關。\n\n學習時間: 判斷背景模型幀的速度，0表示完全不更新，1表示從最後一幀完全重新初始化背景模型。\n\n拉伸寬度、長度: 用來擴充影像的長寬。\n\n抹平等級: 將原始圖像抹平的等級，第一次為擴充影像之前，第二次為擴充影像之後，0為不抹平，最高5級\n\n文字XY: 顯示記數的文字位置，(20, 20) 為左上角。\n\n影片播放速度: 控制播放速度，0為完全靜止，1為最高速，數值越大，停留越久。\n\nRGB: 顯示線、偵測框的顏色。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
