#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jun  9 12:20:03 2022

@author: nickj
"""
import sys
import argparse
import cv2
import os

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('imagefile', type=str,
                    help='an integer for the accumulator')
args = parser.parse_args(sys.argv[1:])

img = cv2.imread(args.imagefile)


# img = cv2.resize(img, (8, 8))
# cv2.imwrite(args.imagefile, img)
print(img.shape)


def rgb888to333(img):
    return [[[c >> 5 for c in pix] for pix in row] for row in img]


img = rgb888to333(img)
print(img)


def tofile(filename, img):
    base = os.path.basename(filename)
    suffix = os.path.splitext(base)[0]
    print(suffix)

    with open(filename, 'w+') as fp:

        pal = []
        fp.write('const u16 %sPal[] = {\n' % (suffix))
        for row in img:
            for pix in row:
                if(pix not in pal):
                    pal.append(pix)
                    fp.write('\t0x%04X, \n' %
                             ((pix[0] << 9) + (pix[1] << 5) + (pix[2] << 1)))
        # for _ in range(len(pal), 16):
        #     fp.write('\t0x0000,\n')
        fp.write('};\n')
        fp.write('const u16 %sPalLen = %i;\n' % (suffix, len(pal)))
        print('palette: '+str(pal))

        
        fp.write('const u32 %sTiles[] = {\n' % (suffix))

        tilesLen = 0
        for col_i in range(0, len(img[0]), 8):
            for row_i in range(0, len(img), 8):
                for row in img[row_i:(row_i+8)]:
                    print('row: ' + str(row))

                    # for i in range(0, len(row), 8):

                    fp.write('\t0x')
                    for j in range(col_i, col_i+8):
                        index = pal.index(row[j])
                        fp.write('%X' % (index))
                    fp.write(', \n')
                    tilesLen += 1
        fp.write('};\n')
        fp.write('const u16 %sTilesLen = %i;\n' % (suffix, tilesLen))


tofile(os.path.splitext(args.imagefile)[0] + '.h', img)
