import tkinter as tk
from tkinter import filedialog
import json
import cv2
import numpy

window = tk.Tk()
window.title('設定APP')
window.geometry('400x550')
window.configure(background='white')
settingVal = []
setting = []
defaultVal = [55, 55, 520, 560, 0, 600, 5, 30, True, 0.01, 8, 5, 5, 5, 20, 20, 1, 0, 255, 0]
settingLable = {
    "detectRangeWidth": "最低偵測寬度",
    "detectRangeHeight": "最低偵測高度",
    "triggerLineHeight": "起始觸發高度",
    "triggerLineHeightEnd": "終點觸發高度",
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


# A required callback method that goes into the trackbar function.
def nothing(x):
    ...


def settingWindows():
    # Initializing the webcam feed.
    file_path = filedialog.askopenfilename()
    print(file_path)
    cap = cv2.VideoCapture(file_path)
    ret, img = cap.read()

    # Create a window named trackbars.
    cv2.namedWindow("Trackbars")

    cv2.createTrackbar("x0", "Trackbars", 0, img.shape[1], nothing)
    cv2.createTrackbar("x1", "Trackbars", 0, img.shape[1], nothing)
    cv2.createTrackbar("y0", "Trackbars", 0, img.shape[0], nothing)
    cv2.createTrackbar("y1", "Trackbars", 0, img.shape[0], nothing)

    while True:
        # Get the new values of the trackbar in real time as the user changes
        # them
        x0 = cv2.getTrackbarPos("x0", "Trackbars")
        y0 = cv2.getTrackbarPos("y0", "Trackbars")
        x1 = cv2.getTrackbarPos("x1", "Trackbars")
        y1 = cv2.getTrackbarPos("y1", "Trackbars")

        newImage = img.copy()

        newImage = cv2.resize(newImage, None, fx=0.4, fy=0.4)
        cv2.rectangle(newImage, (int(x0*0.4), int(y0*0.4)), (int(x1*0.4), int(y1*0.4)), (81, 255, 25), 1)
        cv2.imshow('Trackbars', newImage)

        key = cv2.waitKey(1)

        if key == 13:
            settingVal[2].delete(0, 5)
            settingVal[3].delete(0, 5)
            settingVal[4].delete(0, 5)
            settingVal[5].delete(0, 5)

            settingVal[2].insert(tk.END, y0)
            settingVal[3].insert(tk.END, y1)
            settingVal[4].insert(tk.END, x0)
            settingVal[5].insert(tk.END, x1)
            break

        if key == 27:
            break
    cap.release()
    cv2.destroyAllWindows()


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

    calculate_btn = tk.Button(window, text='寫入', command=calculate_bmi_number)
    calculate_btn.pack()
    preview_btn = tk.Button(window, text='設定範圍寬度', command=settingWindows)
    preview_btn.pack()

    window.mainloop()


if __name__ == '__main__':
    main()
