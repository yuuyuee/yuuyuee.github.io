# Author: yuyue (yuyue2200@hotmail.com)

from __future__ import print_function

import os
import sys
import base64

def Translate(argv):
    if len(argv) != 2:
        print('Usage: {} image'.format(argv[0]))
        return

    fname = argv[1]
    ext = os.path.splitext(fname)[1]
    if ext:
        ext = ext.lstrip('.')
    else:
        raise "Not found extend name"

    with open(fname, 'rb') as fd:
        data = base64.b64encode(fd.read())
        data = data.decode()
        print('![](data:image/{}.base64,{})'.format(ext, data))

if __name__ == '__main__':
    Translate(sys.argv)

