/*
 * tetris.cpp
 *
 *  Created on: 08.07.2023
 *      Author: nick
 */

extern "C" {
#include "genesis.h"
}

#include "Sprites.hpp"
#include "Tiles.hpp"

#include "tetris.h"

enum PieceType: int {
	J = 0,
	S = 1,
	L = 2,
	T = 3,
	Z = 4,
	I = 5,
	O = 6,
	OFFSET_LENGTH = 7
};

#define OFFSET_J { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, 1 } }
#define OFFSET_L { { -1, 0 }, { -1, 1 }, { 0, 0 }, { 1, 0 } }
#define OFFSET_I { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } }
#define OFFSET_O { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
#define OFFSET_S { { -1, 1 }, { 0, 0 }, { 0, 1 }, { 1, 0 } }
#define OFFSET_Z { { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } }
#define OFFSET_T { { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 } }

s16 Offset[][4][2] = {
		OFFSET_J,
		OFFSET_S,
		OFFSET_L,
		OFFSET_T,
		OFFSET_Z,
		OFFSET_I,
		OFFSET_O
};
u16 TileID[] = { 1,2,3,4,5,6,7 };

class Tetris {
private:
	Sprites *m_sprites;
	u16 m_tid, m_sid[4];
	u16 m_x, m_y;
	u8 m_rotation;
	PieceType m_pieceType;
	u16 joy1_before;
//std::array
public:
	Tetris(Sprites *sprites, u16 tid, PieceType pieceType) :
			m_sprites(sprites), m_tid(tid), m_x(0), m_y(0), m_rotation(0), m_pieceType(pieceType) {
		for (int i = 0; i < 4; i++) {
			m_sid[i] = m_sprites->add(Offset[pieceType][i][0],
					Offset[pieceType][i][1], SPRITE_SIZE(1, 1),
					TILE_ATTR_FULL(PAL0, 1, 0, 0, m_tid + TileID[pieceType]));

		}
	}

	void update(){
		u16 joy1_now = JOY_readJoypad(JOY_1);

		if((joy1_now & BUTTON_LEFT) and not (joy1_before & BUTTON_LEFT))
			m_x --;
		if((joy1_now & BUTTON_RIGHT) and not (joy1_before & BUTTON_RIGHT))
			m_x ++;
		if(joy1_now & BUTTON_DOWN)
			m_y ++;

		if((joy1_now & BUTTON_A) and not (joy1_before & BUTTON_A))
			m_rotation = (m_rotation+1) % 4;

		if((joy1_now & BUTTON_B) and not (joy1_before & BUTTON_B))
			m_pieceType = static_cast<PieceType>((m_pieceType+1) % 7);

		paint();

		joy1_before = joy1_now;
	}

	void autoupdate() {
		m_y ++;

		paint();
	}

	void paint() {
		for (int i = 0; i < 4; i++) {
			m_sprites->setX(m_sid[i], 8*posX(i));
			m_sprites->setY(m_sid[i], 8*posY(i));
		}
	}

	s16 posX(int i){
		switch(m_rotation){
		case 0:
			return m_x+Offset[m_pieceType][i][0];
		case 1:
			return m_x+Offset[m_pieceType][i][1];
		case 2:
			return m_x-Offset[m_pieceType][i][0];
		case 3:
			return m_x-Offset[m_pieceType][i][1];
		}
	}
	s16 posY(int i){
		switch(m_rotation){
		case 0:
			return m_y+Offset[m_pieceType][i][1];
		case 1:
			return m_y-Offset[m_pieceType][i][0];
		case 2:
			return m_y-Offset[m_pieceType][i][1];
		case 3:
			return m_y+Offset[m_pieceType][i][0];
		}
	}
};

int main(int hardReset) {
	VDP_setScreenWidth320();
	SYS_disableInts();

	Sprites sprites;
	Tiles tiles;

// Palette
	u16 pid = PAL0;
	VDP_setPalette(pid, tetrisPal);

// Tiles
	u16 tid = tiles.add(tetrisTiles, tetrisTilesLen);

// Background
	/*TileMap map;
	 map.compression = COMPRESSION_NONE;
	 map.h = 32;
	 map.w = 64;
	 map.tilemap = static_cast<u16*>(bgBTileMap);
	 VDP_setTileMapEx(BG_B, &map,
	 TILE_ATTR_FULL(bgbPid, FALSE, FALSE, FALSE, bgbTid), 0, 0, 0, 0, 64,
	 32, DMA);*/

// Sprites
	Tetris tetris(&sprites, tid, L);

	SYS_enableInts();

	int cycles = 0;

#pragma GCC unroll 1
	while (true) {
		// nothing to do here
		// ...
		if(cycles%60==0)
			tetris.autoupdate();
		tetris.update();
		cycles++;

		sprites.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
