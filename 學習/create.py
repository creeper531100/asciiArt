import os
import cv2
def cover():
    image_folder = 'images'
    video_name = 'video.mp4'

    vidcap = cv2.VideoCapture(video_name)
    count = 0
    while vidcap.isOpened():
        success, image = vidcap.read()
        if success:
            cv2.imwrite(os.path.join(image_folder, '%d.png') % count, image)
            count += 1
        else:
            break
    cv2.destroyAllWindows()
    vidcap.release()

cover()
