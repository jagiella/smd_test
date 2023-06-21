/*
 * Sprites.hpp
 *
 *  Created on: 21.06.2023
 *      Author: nickj
 */

#ifndef SRC_SPRITES_HPP_
#define SRC_SPRITES_HPP_

class Sprites {
	int maxlen;
	int n;
	//static VDPSprite sprites[2];
public:
	Sprites(int maxlen = 80);
	u16 add(u16 x, u16 y, u8 size, u16 attr);
	void setX(int index, s16 x);
	void setY(int index, s16 y);
	void setHFlip(int index, u16 hflip);
	void update();
};

#endif /* SRC_SPRITES_HPP_ */
