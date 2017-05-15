# DASHEncoder2

Tool to generated DASH content using command line
一个可以生成DASH媒体内容和MPD的工具

Dependencies:
MP4Box and FFmpeg

Sample CmdLine:
dashencode.bat -v -d -f --encfiles-output-dir=dashencode -b 100,300 -a 64,128 -r 512x288,640x360 -s 60 -o dashtest --mpd-name=cuc_ieschool.mpd --use-segment-timeline cuc_ieschool.mp4

see dashencode -h for more helps

张晖
Hui Zhang
中国传媒大学/数字电视技术
Communication University of China/Digital Video Technology

zhanghuicuc@gmail.com
http://blog.csdn.net/nonmarking
