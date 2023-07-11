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

const int ScoringFactor[] = { 0, 40, 100, 300, 1200 };
const int LevelSpeed[] = { 48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4,
		4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 };

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
#define OFFSET_I { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } }
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

class BackgroundView {
protected:
	Background *m_background;
	int m_offset_x;
	int m_offset_y;
	int m_width;
	int m_height;

public:
	BackgroundView(Background *background, int x, int y, int w, int h) :
			m_background(background), m_offset_x(x), m_offset_y(y), m_width(w), m_height(
					h) {
	}

	u16 getTile(int x, int y) {
		return m_background->getTile(m_offset_x + x, m_offset_y + y);
	}

	void setTile(int x, int y, u16 type) {
		m_background->setTile(m_offset_x + x, m_offset_y + y, type);
	}
	void fill(u16 type) {
		for (auto x = 0; x < m_width; x++) {
			for (auto y = 0; y < m_height; y++)
				m_background->setTile(m_offset_x + x, m_offset_y + y, type);
		}
	}

	s16 offsetX() {
		return m_offset_x;
	}
	s16 offsetY() {
		return m_offset_y;
	}
	s16 width() {
		return m_width;
	}
	s16 height() {
		return m_height;
	}

	bool outOfBounds(int x, int y) {
		return (x < 0 or x >= m_width or y < 0 or y >= m_height);
	}
	bool onGround(int x, int y) {
		if (outOfBounds(x, y))
			return false;

		if (y >= m_height)
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
		if (x >= m_width - 1)
			return true;
		if (getTile(x + 1, y) != TILE_Empty)
			return true;
		return false;
	}
};

class Playfield: public BackgroundView {
public:
	using BackgroundView::BackgroundView;
	s16 startX() {
		return m_width / 2;
	}
	s16 startY() {
		return 0;
	}
	int clearLines() {
		// check full lines
		int removed_lines = 0;
		for (int row = m_height - 1; row > 0; row--) {
			// check line
			bool found_empty = false;
			for (int col = 0; col < m_width; col++) {
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
					for (int col = 0; col < m_width; col++) {
						setTile(col, row + removed_lines, getTile(col, row));
						setTile(col, row, TILE_Empty);
					}
				}
			}
		}

		return removed_lines;
	}
};

class NumberField: public BackgroundView {
	const int number_tiles_offset = 31 * 64;
public:
	using BackgroundView::BackgroundView;
	void setNumber(int number) {
		//int digits[5];
		for (int i = 0; i < m_width; i++) {
			auto digit = number % 10;
			setTile(m_width - 1 - i, 0,
					tetrionTileMap[number_tiles_offset + digit]);
			number /= 10;
			if (number == 0)
				break;
		}
	}
};

class Tetris {
private:
	Playfield *m_playfield;
	BackgroundView *m_nextdisplay;
	Sprites *m_sprites;
	u16 m_tid, m_sid[4], m_sid_ghost[4];
	u8 m_rotation;
	PieceType m_pieceType;
	u16 m_x, m_y;
	u16 joy1_before;
	bool use_ghost = true;
//std::array
public:
	Tetris(Playfield *playfield, BackgroundView *nextdisplay, Sprites *sprites,
			u16 tid, PieceType pieceType) :
			m_playfield(playfield), m_nextdisplay(nextdisplay), m_sprites(
					sprites), m_tid(tid), m_rotation(0), m_pieceType(pieceType) {

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

		updateNext();

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

		if ((joy1_now & BUTTON_A) and not (joy1_before & BUTTON_A)) {
			rotateClockwise();

			// check if not leading to wall collision
			for (int i = 0; i < 4; i++) {
				if (posX(i) < 0 or posX(i) >= m_playfield->width()) {
					rotateCounterClockwise();
					break;
				}

			}
		}
		if ((joy1_now & BUTTON_B) and not (joy1_before & BUTTON_B)) {
			rotateCounterClockwise();

			// check if not leading to wall collision
			for (int i = 0; i < 4; i++) {
				if (posX(i) < 0 or posX(i) >= m_playfield->width()) {
					rotateClockwise();
					break;
				}

			}
		}

		paint();

		joy1_before = joy1_now;
	}

	void autoupdate() {
		if (not onGround()) {
			m_y++;
		} else {
			toBackground();
			updateNext();
		}
		paint();
	}

	void rotateClockwise() {
		m_rotation = (m_rotation + 1) % 4;
	}
	void rotateCounterClockwise() {
		m_rotation = (m_rotation + 3) % 4;
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
		// Todo: I piece has a particular rotation
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
		// Todo: I piece has a particular rotation
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
		m_pieceType = next();
		m_rotation = 0;

		for (int i = 0; i < 4; i++) {
			m_sprites->setTileID(m_sid[i], m_tid + TileID[m_pieceType]);
		}
	}

	void updateNext() {
		// repaint next
		m_nextdisplay->fill(TILE_Empty);
		auto type = next();
		for (int i = 0; i < 4; i++) {
			auto x = Offset[type][i][0];
			auto y = Offset[type][i][1];
			m_nextdisplay->setTile(x + 1, y, TileID[type]);
		}
	}

	PieceType next() {
		return static_cast<PieceType>((m_pieceType + 1) % 7);
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
	Playfield playfield(&background, 15, 0, 10, 20);
	BackgroundView nextdisplay(&background, 28, 14, 4, 2);
	NumberField scoredisplay(&background, 26, 3, 7, 1);
	NumberField leveldisplay(&background, 27, 7, 5, 1);
	NumberField linedisplay(&background, 27, 10, 5, 1);
	Tetris tetris(&playfield, &nextdisplay, &sprites, tid, L);

	SYS_enableInts();

	int cycles = 0;
	int level = 0;
	int lines = 0;
	int score = 0;

	scoredisplay.setNumber(score);
	linedisplay.setNumber(lines);
	leveldisplay.setNumber(level);

#pragma GCC unroll 1
	while (true) {
		// nothing to do here
		// ...

		tetris.update();

		if (cycles % LevelSpeed[level] == 0) {
			tetris.autoupdate();
			auto cleared_lines = playfield.clearLines();

			// update score
			score += ScoringFactor[cleared_lines] * (level + 1);
			scoredisplay.setNumber(score);

			// update lines
			lines += cleared_lines;
			linedisplay.setNumber(lines);

			// update level
			if (lines >= level * 10 + 10) {
				level++;
				leveldisplay.setNumber(level);
			}
		}
		cycles++;

		background.update();
		sprites.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
