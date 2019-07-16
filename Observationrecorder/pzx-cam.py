
# -*- coding: utf-8-*-

import os
import subprocess
import time
import schedule

i=0
device='/dev/video0'


subprocess.run("echo 'hi'",shell=True)

def test():
        global i
        i=i+1

        dest_file = '/home/pzx/tmp/imagetovideo/%03d.jpg'%(i)

        command_tmp = ['fswebcam', '--no-banner', '-r', '640x480', '-q', '-d', device]	
        command_tmp.append(dest_file)
        print(command_tmp)
        subprocess.run(command_tmp, shell=False, check=True)

schedule.every(3).seconds.do(test)


while True:
    # 启动服务
    schedule.run_pending()
    time.sleep(1)
