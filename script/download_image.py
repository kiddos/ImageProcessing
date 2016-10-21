#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import os


def download(url, output_filename):
    if os.path.isfile(output_filename):
        print output_filename, ' already downloaded'
        return
    uf = urllib2.urlopen(url)
    meta = uf.info()
    content_length = meta.getheaders('Content-Length')[0]
    print 'Content-Length: ', content_length


    output = open(output_filename, 'wb')
    buf_size = 1024 * 64
    read_size = 0
    while read_size < content_length:
        buf = uf.read(buf_size)
        size = len(buf)
        if size == 0:
            break
        read_size += size
        output.write(buf)
        print 'download progress: ', read_size, '/', content_length
    uf.close()


def main():
    lena_url = 'http://optipng.sourceforge.net/pngtech/img/lena.png'
    download(lena_url, 'lena.png')


if __name__ == '__main__':
    main()
