import youtube_dl, sys, os
def main(url):
    ydl_opts = {
                'outtmpl': f'HTC.mp4',
                'extractaudio': True,
                'audioformat': 'mp3'
            }
    with youtube_dl.YoutubeDL(ydl_opts) as ydl:
        ydl.download([f'{url}'])
if __name__ == '__main__':
    if len(sys.argv) == 2:
        if os.path.isfile("HTC.mp4"):
            os.remove("HTC.mp4")
        main(sys.argv[1])