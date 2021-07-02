using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using System.Windows.Forms;

namespace GUI
{
    class json
    {
        public int detectRangeWidth { get; set; }
        public int detectRangeHeight { get; set; }
        public int triggerLineHeight { get; set; }
        public int triggerLineHeightEnd { get; set; }
        public int startTriggerLineWidth { get; set; }
        public int endTriggerLineWidth { get; set; }
        public int history { get; set; }
        public int varThreshould { get; set; }
        public int detectShadows { get; set; }
        public double learningRate { get; set; }
        public int elementKsizeWidth { get; set; }
        public int elementKsizeHeight { get; set; }
        public int medianBlurKsize1 { get; set; }
        public int medianBlurKsize2 { get; set; }
        public int putTextX { get; set; }
        public int putTextY { get; set; }
        public int waitKey { get; set; }
        public int colorR { get; set; }
        public int colorG { get; set; }
        public int colorB { get; set; }
    }
    public class PackJson
    {
        public string JsonPack(List<TrackBar> trackBarList)
        {
            var weatherForecast = new json
            {
                detectRangeWidth = trackBarList[19].Value,
                detectRangeHeight = trackBarList[18].Value,
                triggerLineHeight = trackBarList[17].Value,
                triggerLineHeightEnd = trackBarList[16].Value,
                startTriggerLineWidth = trackBarList[15].Value,
                endTriggerLineWidth = trackBarList[14].Value,
                history = trackBarList[13].Value,
                varThreshould = trackBarList[12].Value,
                detectShadows = trackBarList[11].Value,
                learningRate = 0.01,
                elementKsizeWidth = trackBarList[9].Value,
                elementKsizeHeight = trackBarList[8].Value,
                medianBlurKsize1 = trackBarList[7].Value,
                medianBlurKsize2 = trackBarList[6].Value,
                putTextX = trackBarList[5].Value,
                putTextY = trackBarList[4].Value,
                waitKey = trackBarList[3].Value,
                colorR = trackBarList[2].Value,
                colorG = trackBarList[1].Value,
                colorB = trackBarList[0].Value
            };
            var jsonFile = JsonSerializer.Serialize<json>(weatherForecast);
            File.WriteAllTextAsync("setting.json", jsonFile);
            return jsonFile;
        }
    }
}
