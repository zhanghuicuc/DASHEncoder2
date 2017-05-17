
# coding: GB2312
import os
import subprocess
# execute command, and return the output
def execCmd(cmd):
	r = os.popen(cmd)
	text = r.read()
	r.close()
	return text

def execCmdTrue(cmd):
    try:
        out_bytes = subprocess.check_output(cmd, shell=True)
        out_text = out_bytes.decode('utf-8')
        return out_text
    except subprocess.CalledProcessError as e:
        out_bytes = e.output       # Output generated before error
        code      = e.returncode   # Return code
    
if __name__ == '__main__':
    cmd = ".\\bin\\TIandSIprocmd.exe -i video_00300.mp4 -s 60 "
    print cmd
    result = execCmdTrue(cmd)
    TI,SI=result.split('||',1)
    print TI
    print SI
    TIs = [float(x) for x in TI.split(',')]
    for x in TIs:
        print "%.3f" %x
