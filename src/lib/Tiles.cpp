/*
 * Tiles.cpp
 *
 *  Created on: 21.06.2023
 *      Author: nickj
 */
extern "C" {
#include "genesis.h"
}

#include "Tiles.hpp"

Tiles::Tiles() {
	n = TILE_USERINDEX;
}
u16 Tiles::add(const u32 *tiles, u16 num) {
	u16 tid = n;
	VDP_loadTileData(tiles, tid, num / 8, DMA);
	n += num / 8;
	return tid;
}

void Tiles::remove(u16 tid, u16 num){
	// todo: reorganize tiles
	n -= num / 8;
}

void Tiles::update(u16 tid, const u32 *tiles, u16 num) {
	VDP_loadTileData(tiles, tid, num / 8, DMA);
}

