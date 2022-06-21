#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun 21 18:38:19 2022

@author: nickj
"""


def sign(d):
    if(d < 0):
        return -1
    else:
        return +1


def raster(m_x, m_y, sx, sy):
    x1 = m_x
    y1 = m_y
    x2 = m_x+sx
    y2 = m_y+sy
    dx = x2 - x1
    dy = y2 - y1
    print('dx=%d, dy=%d' % (dx, dy))
    if(abs(dx) > abs(dy)):
        while (m_x != x2):
            # m_y = y1 + int(int(dy * (m_x - x1)) / dx)
            while(int(m_y*dx) != int(y1 + (dy * (m_x - x1)))):
                print(int(m_y*dx), int(y1 + (dy * (m_x - x1))))
                m_y = m_y + sign(dy)
            m_x += sign(dx)
            print((m_x, m_y))
    else:
        while (m_y != y2):
            m_x = x1 + int(int(dx * (m_y - y1)) / dy)
            m_y += sign(dy)
            print((m_x, m_y))


raster(0, 0, 3, 4)
raster(0, 0, 4, 3)
raster(0, 0, 4, 0)
raster(0, 0, 3, -4)
raster(0, 0, -3, -4)
