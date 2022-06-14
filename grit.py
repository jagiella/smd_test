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
parser.add_argument('-m', dest='map', nargs='?', choices=['!'], default='!', help='Include or exclude map data [exc]')
parser.add_argument('-mR', dest='map_reduction', nargs='*', choices=['t','p','f'], default=['t','p','f'], help='Tile reduction: (t)iles, (p)al, (f)lipped [Rtpf]')
parser.add_argument('-mu', dest='map_dtype', choices=['8','16','32'], default='16', help='Map data type: u8, u16, u32 [u16]')
parser.add_argument('-gT', dest='transparent_color', default='FF00FF', help='Transparent color; rrggbb hex or 16bit BGR hex [FF00FF]')
parser.print_help()
args = parser.parse_args(sys.argv[1:])

print(args)

img = cv2.imread(args.imagefile)


# img = cv2.resize(img, (8, 8))
# cv2.imwrite(args.imagefile, img)
print(img.shape)


def rgb888to333(img):
    return [[[c >> 5 for c in pix] for pix in row] for row in img]


img = rgb888to333(img)
# print(img)

def getPalette(img):
    # transparent color
    tCol = [int(args.transparent_color[i:i+2], 16)>>5 for i in range(0,6,2)]
    print(tCol)
    
    pal = [tCol]
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

def hflip(tile):
    flipped = [tile[j] for i in range(0, len(tile), 8) for j in reversed(range(i, i+8))]
    return flipped

def vflip(tile):
    flipped = [tile[j] for i in reversed(range(0, len(tile), 8)) for j in range(i, i+8)]
    return flipped

# def vflip(tile):
#     flipped = [tile[j] for i in reversed(range(0, len(tile), 8)) for j in range(i, i+8)]
#     return flipped

def getTilemap(tiles):
    redTiles = []
    tileMap = []
    for tiles_row in tiles:
        tileMap_row = []
        for tile in tiles_row:
            
            h,v = 0,0
            if(tile in redTiles):
                index = redTiles.index(tile)
            elif(hflip(tile) in redTiles):
                index = redTiles.index(hflip(tile))
                h=1
            elif(vflip(tile) in redTiles):
                index = redTiles.index(vflip(tile))
                v=1
            elif(vflip(hflip(tile)) in redTiles):
                index = redTiles.index(vflip(hflip(tile)))
                h,v=1,1
            else:
                index = len(redTiles)
                redTiles.append(tile)
                # print('flipped found')
            
            # index = redTiles.index(tile)
            # print(index)
            tileMap_row.append((index,h,v))
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
    


def spritesToFile(filename, img):
    print('sprite(s):')
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
    print('tile map:')
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
            for tileIndex,hflip,vflip in tileMapRow:
                # print(tileIndex)
                prio = 0
                palID = 0
                # vflip = 0
                # hflip = 0
                fp.write('\t0x%X, \n' % ((prio<<15) + (palID<<13) + (vflip<<12) + (hflip<<11) + tileIndex))
        fp.write('};\n')
    # print(tileMap)

if(args.map != '!'):
    tilemapToFile(os.path.splitext(args.imagefile)[0] + '.h', img)
else:
    spritesToFile(os.path.splitext(args.imagefile)[0] + '.h', img)
    