ffmpeg -r 1 -i ./imagetovideo/%03d.jpg  -absf aac_adtstoasc output.mp4
ffmpeg -r 1 -i ./imagetovideo/%03d.jpg  -vf fps=1  aa.mp4 
fswebcam -d /dev/video0 --no-banner -r 640x480 -S 1 019.jpg 
