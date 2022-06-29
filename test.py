#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun 29 12:46:55 2022

@author: nickj
"""

import numpy as np

x = np.arange(16).reshape((2, 8))

print(x)
print(np.asarray(np.hsplit(x, 4)).flatten())
# print(np.concatenate(np.hsplit(x, 4).flatten())

# mt = np.hsplit(x, 4)
# ids = [m[:] for m in mt]
# print(ids)
