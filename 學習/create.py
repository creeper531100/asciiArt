import os
import cv2
import sys


def cover(video_name, image_folder):
    vidcap = cv2.VideoCapture(video_name)
    count = 0
    while vidcap.isOpened():
        success, image = vidcap.read()
        if success:
            cv2.imwrite(os.path.join(image_folder, '%07d.png') % count, image)
            count += 1
        else:
            break
    cv2.destroyAllWindows()
    vidcap.release()

cover(sys.argv[1], sys.argv[2])