from moviepy.video.io.VideoFileClip import VideoFileClip
import sys, os, time

def main(input_name, out_putname):
    clip1 = VideoFileClip(out_putname)
    audioclip1 = clip1.audio
    clip2 = VideoFileClip(input_name)
    new_video = clip2.set_audio(audioclip1)
    new_video.write_videofile(f"{out_putname}_1080p.mp4")

if __name__ == '__main__':
    if len(sys.argv) >= 2:
        main(sys.argv[1], sys.argv[2])