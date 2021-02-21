import tkinter as tk
import json

window = tk.Tk()
window.title('設定APP')
window.geometry('400x500')
window.configure(background='white')
settingVal = []
setting = []
defaultVal = [55,55,520,0,600,5,30,True,0.01,8,5,5,5,20,20,1,0,255,0]
settingLable = {
    "detectRangeWidth": "最低偵測寬度",
    "detectRangeHeight": "最低偵測高度",
    "triggerLineHeight": "觸發高度",
    "startTriggerLineWidth": "起始線位置",
    "endTriggerLineWidth": "終點線位置",
    "history": "學習張數",
    "varThreshould": "光影敏感度",
    "detectShadows": "影子偵測",
    "learningRate": "學習時間",
    "elementKsizeWidth": "拉伸寬度",
    "elementKsizeHeight": "拉伸長度",
    "medianBlurKsize1": "第一次抹平等級",
    "medianBlurKsize2": "第二次抹平等級",
    "putTextX": "文字X位置",
    "putTextY": "文字Y位置",
    "waitKey": "影片播放速度",
    "colorR": "R",
    "colorG": "G",
    "colorB": "B"
}


def calculate_bmi_number():
    for i, row in enumerate(settingLable):
        if row == "detectShadows":
            settingLable[row] = bool(settingVal[i].get())
        elif row == "learningRate":
            settingLable[row] = float(settingVal[i].get())
        else:
            settingLable[row] = int(settingVal[i].get())
    with open("setting.json", "w", encoding="utf8") as f:
        json.dump(settingLable, f)

    print(settingLable)



def main():
    header_label = tk.Label(window, text='設置SETTING')
    header_label.pack()
    for i, row in enumerate(settingLable):
        setting.append(tk.Frame(window))
        setting[i].pack(side=tk.TOP)
        tk.Label(setting[i], text=settingLable[row] + ", " + row).pack(side=tk.LEFT)
        settingVal.append(tk.Entry(setting[i]))
        settingVal[i].insert(0, defaultVal[i])
        settingVal[i].pack(side=tk.LEFT)

    result_label = tk.Label(window)
    result_label.pack()

    calculate_btn = tk.Button(window, text='哈哈', command=calculate_bmi_number)
    calculate_btn.pack()

    window.mainloop()


if __name__ == '__main__':
    main()
