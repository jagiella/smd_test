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

enum TileTypes: u16 {
	TILE_Board = 0,
	TILE_J = 1,
	TILE_S = 2,
	TILE_L = 3,
	TILE_T = 4,
	TILE_Z = 5,
	TILE_I = 6,
	TILE_O = 7,
	TILE_Empty = 8
};

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
u16 TileID[] = {
		TILE_J,
		TILE_S,
		TILE_L,
		TILE_T,
		TILE_Z,
		TILE_I,
		TILE_O };

class Board {
	u16 m_tid;
	u16 m_pid;
	u16 tilemap[32*32];
	//TileMap map;
	const u16 m_h=32, m_w=32;
	bool pending_changes;
public:
	Board(u16 tid, u16 pid) : m_tid(tid), m_pid(pid), pending_changes(false){

		for(auto i=0; i<32*32; i++)
			tilemap[i] = TILE_Empty;

		// set vertical walls
		for(auto i=0; i<32; i++){
			setTile(0,i, TILE_Board);
			setTile(31,i, TILE_Board);
		}
		// set horizontal walls
		for(auto i=0; i<32; i++){
			setTile(i,27, TILE_Board);
		}


		TileMap map;
		map.compression = COMPRESSION_NONE;
		map.h = m_h;
		map.w = m_w;
		map.tilemap = static_cast<u16*>(tilemap);
		VDP_setTileMapEx(BG_B, &map,
				TILE_ATTR_FULL(m_pid, FALSE, FALSE, FALSE, m_tid), 0, 0, 0, 0, m_w,
				m_h, DMA);
	}

	TileTypes getTile(int x, int y){
		return static_cast<TileTypes>(tilemap[x + y*m_w]);
	}

	void setTile(int x, int y, TileTypes type){
		tilemap[x + y*m_w] = type;
		pending_changes = true;
	}


	void update() {
		// check full lines
		int removed_lines = 0;
		for(int row=26; row>0; row--){
			// check line
			bool found_empty = false;
			for(int col=1; col<31; col++){
				if(getTile(col, row) == TILE_Empty){
					found_empty = true;
					break;
				}
			}

			if(not found_empty){
				/*for(int col=1; col<31; col++){
					setTile(col, row, TILE_Empty);
				}*/
				removed_lines++;
			} else {
				if(removed_lines > 0) {
					// move lines down
					for(int col=1; col<31; col++){
						setTile(col, row+removed_lines, getTile(col, row));
						setTile(col, row, TILE_Empty);
					}
				}
			}
		}


		// update tiles
		if(pending_changes){
			TileMap map;
					map.compression = COMPRESSION_NONE;
					map.h = m_h;
					map.w = m_w;
					map.tilemap = static_cast<u16*>(tilemap);
			VDP_setTileMapEx(BG_B, &map,
							TILE_ATTR_FULL(m_pid, FALSE, FALSE, FALSE, m_tid), 0, 0, 0, 0, m_w,
							m_h, DMA);
			pending_changes = false;
		}
	}
};

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
			m_sprites(sprites), m_tid(tid), m_x(10), m_y(0), m_rotation(0), m_pieceType(pieceType) {
		for (int i = 0; i < 4; i++) {
			m_sid[i] = m_sprites->add(Offset[pieceType][i][0],
					Offset[pieceType][i][1], SPRITE_SIZE(1, 1),
					TILE_ATTR_FULL(PAL0, 1, 0, 0, m_tid + TileID[pieceType]));

		}
	}

	void update(Board *board){
		u16 joy1_now = JOY_readJoypad(JOY_1);

		if((joy1_now & BUTTON_LEFT) and not (joy1_before & BUTTON_LEFT))
			if(not onWallLeft(board))
				m_x --;
		if((joy1_now & BUTTON_RIGHT) and not (joy1_before & BUTTON_RIGHT))
			if(not onWallRight(board))
				m_x ++;
		if(joy1_now & BUTTON_DOWN)
			if(not onGround(board))
				m_y ++;

		if((joy1_now & BUTTON_A) and not (joy1_before & BUTTON_A))
			m_rotation = (m_rotation+1) % 4;

		if((joy1_now & BUTTON_B) and not (joy1_before & BUTTON_B)){
			m_pieceType = static_cast<PieceType>((m_pieceType+1) % 7);
			for (int i = 0; i < 4; i++) {
				m_sprites->setTileID(m_sid[i] , m_tid + TileID[m_pieceType]);
			}
		}

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

	bool onGround(Board *board) {
		for(int i=0; i<4; i++){
			if(board->getTile(posX(i), posY(i)+1) != TILE_Empty)
				return true;
		}
		return false;
	}

	bool onWallLeft(Board *board) {
		for(int i=0; i<4; i++){
			if(board->getTile(posX(i)-1, posY(i)) != TILE_Empty)
				return true;
		}
		return false;
	}

	bool onWallRight(Board *board) {
		for(int i=0; i<4; i++){
			if(board->getTile(posX(i)+1, posY(i)) != TILE_Empty)
				return true;
		}
		return false;
	}

	void toBoard(Board *board) {
		for(int i=0; i<4; i++){
			board->setTile(posX(i), posY(i), TILE_Board);
		}

		m_x = 10;
		m_y = 0;
		m_pieceType = static_cast<PieceType>((m_pieceType+1) % 7);
		m_rotation = 0;

		for (int i = 0; i < 4; i++) {
			m_sprites->setTileID(m_sid[i] , m_tid + TileID[m_pieceType]);
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
	Board board(tid, pid);
	Tetris tetris(&sprites, tid, L);

	SYS_enableInts();

	int cycles = 0;

#pragma GCC unroll 1
	while (true) {
		// nothing to do here
		// ...

		tetris.update(&board);

		if(cycles%60==0){
			if(not tetris.onGround(&board)){
				tetris.autoupdate();
			} else {
				tetris.toBoard(&board);
			}
		}

		board.update();


		cycles++;

		sprites.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
