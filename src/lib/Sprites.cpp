/*
 * Sprites.cpp
 *
 *  Created on: 21.06.2023
 *      Author: nickj
 */

extern "C" {
#include "genesis.h"
}

#include "Sprites.hpp"

VDPSprite sprites[2];

Sprites::Sprites(int maxlen) :
		maxlen(maxlen), n(0) {
}

u16 Sprites::add(u16 x, u16 y, u8 size, u16 attr) {
	u16 sid = n;
	if (n == maxlen)
		return sid;
	if (n > 0)
		sprites[n - 1].link = n;
	sprites[n].link = 0;
	sprites[n].attribut = attr;
	sprites[n].size = size;
	sprites[n].x = x + 0x80;
	sprites[n].y = y + 0x80;
	n++;
	return sid;
}

void Sprites::remove(int index) {
	// todo: assume to be last
	n--;
	sprites[n].link = 0;
	sprites[n].y = 0;
	update();
	sprites[n-1].link = 0;
}

void Sprites::setX(int index, s16 x) {
	sprites[index].x = x + 0x80;
}
void Sprites::setY(int index, s16 y) {
	sprites[index].y = y + 0x80;
}
void Sprites::setHFlip(int index, u16 hflip) {
	sprites[index].attribut =
			(sprites[index].attribut & (~TILE_ATTR_HFLIP_MASK))
					+ (hflip << TILE_ATTR_HFLIP_SFT);
}
void Sprites::setTileID(int index, u16 tid){
	sprites[index].attribut = tid + (sprites[index].attribut & (TILE_ATTR_PALETTE_MASK | TILE_ATTR_HFLIP_MASK | TILE_ATTR_VFLIP_MASK));
}

void Sprites::update() {
	DMA_transfer(DMA, DMA_VRAM, sprites, VDP_SPRITE_TABLE,
			n * sizeof(VDPSprite) / 2, 2);
}

