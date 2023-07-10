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

//#include "tetris.h"
#include "res/tetrion.h"

enum TileTypes : u16 {
	TILE_Empty = 0,
	TILE_J = 1,
	TILE_S = 2,
	TILE_L = 3,
	TILE_T = 4,
	TILE_Z = 5,
	TILE_I = 6,
	TILE_O = 7,
	TILE_Ghost = 8
};

enum PieceType : int {
	J = 0, S = 1, L = 2, T = 3, Z = 4, I = 5, O = 6, OFFSET_LENGTH = 7
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
OFFSET_O };
u16 TileID[] = { TILE_J, TILE_S, TILE_L, TILE_T, TILE_Z, TILE_I, TILE_O };

class Background {
	u16 m_tid;
	u16 m_pid;
	const u16 m_h = 32, m_w = 64;
	u16 tilemap[32 * 64];
	bool pending_changes;
public:
	Background(u16 tid, u16 pid) :
			m_tid(tid), m_pid(pid), pending_changes(false) {

		//auto tmp = static_cast<const u16*>(&tetrionTiles[0]);
		//auto tmp = reinterpret_cast<u32*>(tilemap);
		//for (auto i = 0; i < tetrionTilesLen; i++)
		//	tmp[i] =  tetrionTiles[i];
		for (auto i = 0; i < m_w * m_h; i++)
			tilemap[i] = tetrionTileMap[i];

		// set vertical walls
		/*for (auto i = 0; i < 32; i++) {
		 setTile(0, i, TILE_Background);
		 setTile(31, i, TILE_Background);
		 }
		 // set horizontal walls
		 for (auto i = 0; i < 32; i++) {
		 setTile(i, 27, TILE_Background);
		 }*/

		TileMap map;
		map.compression = COMPRESSION_NONE;
		map.h = m_h;
		map.w = m_w;
		map.tilemap = static_cast<u16*>(tilemap);
		VDP_setTileMapEx(BG_B, &map,
				TILE_ATTR_FULL(m_pid, FALSE, FALSE, FALSE, m_tid), 0, 0, 0, 0,
				m_w, m_h, DMA);
	}

	u16 getTile(int x, int y) {
		return tilemap[x + y * m_w];
	}

	void setTile(int x, int y, u16 type) {
		tilemap[x + y * m_w] = type;
		pending_changes = true;
	}

	void update() {
		// check full lines
		/*int removed_lines = 0;
		 for (int row = 26; row > 0; row--) {
		 // check line
		 bool found_empty = false;
		 for (int col = 1; col < 31; col++) {
		 if (getTile(col, row) == TILE_Empty) {
		 found_empty = true;
		 break;
		 }
		 }

		 if (not found_empty) {
		 removed_lines++;
		 } else {
		 if (removed_lines > 0) {
		 // move lines down
		 for (int col = 1; col < 31; col++) {
		 setTile(col, row + removed_lines, getTile(col, row));
		 setTile(col, row, TILE_Empty);
		 }
		 }
		 }
		 }*/

		// update tiles
		if (pending_changes) {
			TileMap map;
			map.compression = COMPRESSION_NONE;
			map.h = m_h;
			map.w = m_w;
			map.tilemap = static_cast<u16*>(tilemap);
			VDP_setTileMapEx(BG_B, &map,
					TILE_ATTR_FULL(m_pid, FALSE, FALSE, FALSE, m_tid), 0, 0, 0,
					0, m_w, m_h, DMA);
			pending_changes = false;
		}
	}
};

class Playfield {
	Background *m_background;
	const int width = 10;
	const int height = 20;
	int m_offset_x;
	int m_offset_y;

public:
	Playfield(Background *background, int x, int y) :
			m_background(background), m_offset_x(x), m_offset_y(y) {
		// paint tetrion (surrounding frame)
		// set vertical walls
		/*for (auto y = 0; y < height; y++) {
			// left
			setTile(-1, y, TILE_Ghost);
			// right
			setTile(width, y, TILE_Ghost);
		}
		// set horizontal walls
		for (auto x = -1; x < width + 1; x++) {
			setTile(x, height, TILE_Ghost);
		}*/
	}

	void update() {
		// check full lines
		int removed_lines = 0;
		for (int row = height - 1; row > 0; row--) {
			// check line
			bool found_empty = false;
			for (int col = 0; col < width; col++) {
				if (getTile(col, row) == TILE_Empty) {
					found_empty = true;
					break;
				}
			}

			if (not found_empty) {
				removed_lines++;
			} else {
				if (removed_lines > 0) {
					// move lines down
					for (int col = 0; col < width; col++) {
						setTile(col, row + removed_lines, getTile(col, row));
						setTile(col, row, TILE_Empty);
					}
				}
			}
		}
	}

	u16 getTile(int x, int y) {
		return m_background->getTile(m_offset_x + x, m_offset_y + y);
	}

	void setTile(int x, int y, u16 type) {
		m_background->setTile(m_offset_x + x, m_offset_y + y, type);
	}

	s16 offsetX() {
		return m_offset_x;
	}
	s16 offsetY() {
		return m_offset_y;
	}
	s16 startX() {
		return width / 2;
	}
	s16 startY() {
		return 0;
	}
	bool outOfBounds(int x, int y) {
		return (x < 0 or x >= width or y < 0 or y >= height);
	}
	bool onGround(int x, int y) {
		if (outOfBounds(x, y))
			return false;

		if (y >= height)
			return true;
		if (getTile(x, y + 1) != TILE_Empty)
			return true;
		return false;
	}
	bool onWallLeft(int x, int y) {
		if (outOfBounds(x, y))
			return false;
		if (x <= 0)
			return true;
		if (getTile(x - 1, y) != TILE_Empty)
			return true;
		return false;
	}
	bool onWallRight(int x, int y) {
		if (outOfBounds(x, y))
			return false;
		if (x >= width - 1)
			return true;
		if (getTile(x + 1, y) != TILE_Empty)
			return true;
		return false;
	}
};

class Tetris {
private:
	Playfield *m_playfield;
	Sprites *m_sprites;
	u16 m_tid, m_sid[4], m_sid_ghost[4];
	u8 m_rotation;
	PieceType m_pieceType;
	u16 m_x, m_y;
	u16 joy1_before;
	bool use_ghost = true;
//std::array
public:
	Tetris(Playfield *playfield, Sprites *sprites, u16 tid, PieceType pieceType) :
			m_playfield(playfield), m_sprites(sprites), m_tid(tid), m_rotation(
					0), m_pieceType(pieceType) {

		// piece
		for (int i = 0; i < 4; i++) {
			m_sid[i] = m_sprites->add(Offset[pieceType][i][0],
					Offset[pieceType][i][1], SPRITE_SIZE(1, 1),
					TILE_ATTR_FULL(PAL0, 1, 0, 0, m_tid + TileID[pieceType]));
		}
		// ghost piece
		if (use_ghost)
			for (int i = 0; i < 4; i++) {
				m_sid_ghost[i] = m_sprites->add(Offset[pieceType][i][0],
						Offset[pieceType][i][1], SPRITE_SIZE(1, 1),
						TILE_ATTR_FULL(PAL0, 0, 0, 0, m_tid + TILE_Ghost));
			}

		m_x = m_playfield->startX();
		m_y = m_playfield->startY();
		joy1_before = JOY_readJoypad(JOY_1);
	}

	void update() {
		u16 joy1_now = JOY_readJoypad(JOY_1);

		if ((joy1_now & BUTTON_LEFT) and not (joy1_before & BUTTON_LEFT))
			if (not onWallLeft())
				m_x--;
		if ((joy1_now & BUTTON_RIGHT) and not (joy1_before & BUTTON_RIGHT))
			if (not onWallRight())
				m_x++;
		if (joy1_now & BUTTON_DOWN)
			if (not onGround())
				m_y++;

		if ((joy1_now & BUTTON_A) and not (joy1_before & BUTTON_A))
			m_rotation = (m_rotation + 1) % 4;

		if ((joy1_now & BUTTON_B) and not (joy1_before & BUTTON_B)) {
			m_pieceType = static_cast<PieceType>((m_pieceType + 1) % 7);
			for (int i = 0; i < 4; i++) {
				m_sprites->setTileID(m_sid[i], m_tid + TileID[m_pieceType]);
			}
		}

		paint();

		joy1_before = joy1_now;
	}

	void autoupdate() {
		if (not onGround())
			m_y++;
		else
			toBackground();

		paint();
	}

	void paint() {
		for (int i = 0; i < 4; i++) {
			m_sprites->setX(m_sid[i], 8 * (posX(i) + m_playfield->offsetX()));
			m_sprites->setY(m_sid[i], 8 * (posY(i) + m_playfield->offsetY()));
		}

		//estimate ghost y
		if (use_ghost) {
			s16 dy_ghost = 0;
			bool ghost_on_ground = onGround();
			while (not ghost_on_ground) {
				dy_ghost++;
				for (int i = 0; i < 4; i++) {
					if (m_playfield->onGround(posX(i), posY(i) + dy_ghost))
						ghost_on_ground = true;
				}
			}

			for (int i = 0; i < 4; i++) {
				m_sprites->setX(m_sid_ghost[i],
						8 * (posX(i) + m_playfield->offsetX()));
				m_sprites->setY(m_sid_ghost[i],
						8 * (posY(i) + dy_ghost + m_playfield->offsetY()));
			}
		}

	}

	s16 posX(int i) {
		switch (m_rotation) {
		case 0:
			return m_x + Offset[m_pieceType][i][0];
		case 1:
			return m_x + Offset[m_pieceType][i][1];
		case 2:
			return m_x - Offset[m_pieceType][i][0];
		case 3:
			return m_x - Offset[m_pieceType][i][1];
		}
		return 0;
	}
	s16 posY(int i) {
		switch (m_rotation) {
		case 0:
			return m_y + Offset[m_pieceType][i][1];
		case 1:
			return m_y - Offset[m_pieceType][i][0];
		case 2:
			return m_y - Offset[m_pieceType][i][1];
		case 3:
			return m_y + Offset[m_pieceType][i][0];
		}
		return 0;
	}

	bool onGround() {
		for (int i = 0; i < 4; i++) {
			if (m_playfield->onGround(posX(i), posY(i)))
				return true;
		}
		return false;
	}

	bool onWallLeft() {
		for (int i = 0; i < 4; i++) {
			if (m_playfield->onWallLeft(posX(i), posY(i)))
				return true;
		}
		return false;
	}

	bool onWallRight() {
		for (int i = 0; i < 4; i++) {
			if (m_playfield->onWallRight(posX(i), posY(i)))
				return true;
		}
		return false;
	}

	void toBackground() {
		for (int i = 0; i < 4; i++) {
			//m_playfield->setTile(posX(i), posY(i), static_cast<TileTypes>(m_tid + TileID[m_pieceType]));
			m_playfield->setTile(posX(i), posY(i), TileID[m_pieceType]);
		}

		m_x = m_playfield->startX();
		m_y = m_playfield->startY();
		m_pieceType = static_cast<PieceType>((m_pieceType + 1) % 7);
		m_rotation = 0;

		for (int i = 0; i < 4; i++) {
			m_sprites->setTileID(m_sid[i], m_tid + TileID[m_pieceType]);
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
	VDP_setPalette(pid, tetrionPal);

// Tiles
	u16 tid = tiles.add(tetrionTiles, tetrionTilesLen);

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
	Background background(tid, pid);
	Playfield playfield(&background, 15, 0);
	Tetris tetris(&playfield, &sprites, tid, L);

	SYS_enableInts();

	int cycles = 0;

#pragma GCC unroll 1
	while (true) {
		// nothing to do here
		// ...

		tetris.update();

		if (cycles % 60 == 0) {
			tetris.autoupdate();
		}

		playfield.update();

		cycles++;

		background.update();
		sprites.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
