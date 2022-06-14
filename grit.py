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
# print(img)

def getPalette(img):
    pal = []
    for row in img:
        for pix in row:
            if(pix not in pal):
                pal.append(pix)
    return pal

def getTiles(img, pal):
    tiles = []
    for row_i in range(0, len(img), 8):
        tiles_row = []
        for col_i in range(0, len(img[0]), 8):
            tile = []
            for row in img[row_i:(row_i+8)]:
                for j in range(col_i, col_i+8):
                    tile.append( pal.index(row[j]))
            tiles_row.append(tile)
        tiles.append(tiles_row)
    return tiles

def getTilemap(tiles):
    redTiles = []
    tileMap = []
    for tiles_row in tiles:
        tileMap_row = []
        for tile in tiles_row:
            if(tile not in redTiles):
                redTiles.append(tile)
            index = redTiles.index(tile)
            # print(index)
            tileMap_row.append(index)
        tileMap.append(tileMap_row)
    return tileMap, redTiles

def writePalette(fp, suffix, pal):
    fp.write('const u16 %sPal[] = {\n' % (suffix))
    for p in pal:
        fp.write('\t0x%04X, \n' % ((p[0]<<9) + (p[1]<<5) + (p[2]<<1)))
    fp.write('};\n')
    fp.write('const u16 %sPalLen = %i;\n' % (suffix, len(pal)))

def writeTiles(fp, suffix, tiles):
    tilesLen = 0
    fp.write('const u32 %sTiles[] = {\n' % (suffix))
    for col_i in range(len(tiles[0])):
        for row_i in range(len(tiles)):
            tile = tiles[row_i][col_i]
            for i in range(0, 64, 8):
                fp.write('\t0x')
                for j in range(i, i+8):
                    fp.write('%X' % (tile[j]))
                fp.write(', \n')
                tilesLen += 1
    fp.write('};\n')
    fp.write('const u16 %sTilesLen = %i;\n' % (suffix, tilesLen))
    


def tofile(filename, img):
    base = os.path.basename(filename)
    suffix = os.path.splitext(base)[0]
    print(suffix)

    with open(filename, 'w+') as fp:

        pal = getPalette(img)
        writePalette(fp, suffix, pal)
        # print('palette: '+str(pal))

        tiles = getTiles(img, pal)
        writeTiles(fp, suffix, tiles)

# tofile(os.path.splitext(args.imagefile)[0] + '.h', img)

def tilemapToFile(filename, img):
    pal = getPalette(img)
    tiles = getTiles(img, pal)
    tileMap, redTiles = getTilemap(tiles)
    
    print('pal: %d' % len(pal))
    print('tiles: %d x %d' % (len(tiles[0]), len(tiles)))
    print('red tiles: %d' % (len(redTiles)))
    
    base = os.path.basename(filename)
    suffix = os.path.splitext(base)[0]
    print(suffix)

    with open(filename, 'w+') as fp:
        writePalette(fp, suffix, pal)
        writeTiles(fp, suffix, [redTiles])
        
        # tilemap
        fp.write('u16 %sTileMap[] = {\n' % (suffix))
        for tileMapRow in tileMap:
            for tileIndex in tileMapRow:
                print(tileIndex)
                prio = 0
                palID = 0
                vflip = 0
                hflip = 0
                fp.write('\t0x%X, \n' % ((prio<<15) + (palID<<13) + (vflip<<12) + (hflip<<11) + tileIndex))
        fp.write('};\n')
    # print(tileMap)
    
tilemapToFile(os.path.splitext(args.imagefile)[0] + '.h', img)
    