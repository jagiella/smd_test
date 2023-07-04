extern "C" {
#include "genesis.h"
}

void* operator new(unsigned int size) {
	return malloc(size);
}
void operator delete(void *ptr) {
	free(ptr);
}
void operator delete(void *ptr, unsigned int) {
	free(ptr);
}
/*void LIBGCC2_UNWIND_ATTRIBUTE _Unwind_Resume(struct _Unwind_Exception * object){

 }*/

#include <initializer_list>
#include <array>
//#include <list>

#include "kirby.h"
#include "mario.h"
//#include "mushroom.h"
#include "quietschi.h"
#include "char.h"
#include "bgA.h"
#include "bgB.h"
#include "background.h"
#include "foreground.h"
#include "height.h"

#include "src/fix16.hpp"
#include "src/list.hpp"
#include "src/Sprites.hpp"
#include "src/Tiles.hpp"
#include "src/Projectile.hpp"

//#include <iterator> // For std::forward_iterator_tag
//#include <cstddef>  // For std::ptrdiff_t

//Tiles tileEngine;
//Sprites spriteEngine;
/*
 class SpriteSequence {
 const u16 m_spriteWidth;
 const u16 m_spriteHeight;
 const u16 m_spriteCount;
 const u32 *m_tiles;
 u16 m_tid;
 u8 m_frameCount;
 u8 m_animationFrames;
 public:
 SpriteSequence(const u16 spriteWidth, const u16 spriteHeight,
 const u16 spriteCount, const u32 *tiles, u8 frames) :
 m_spriteWidth(spriteWidth), m_spriteHeight(spriteHeight), m_spriteCount(
 spriteCount), m_tiles(tiles), m_frameCount(0), m_animationFrames(
 frames) {
 //m_tid = tileEngine.add(m_tiles, 8 * (spriteWidth * spriteHeight));

 }
 void operator++() {
 m_frameCount = (m_frameCount + 1) % (m_animationFrames * m_spriteCount);
 if (m_frameCount % m_animationFrames == 0) {
 // load tiles
 auto spriteIndex = m_frameCount / m_animationFrames;
 auto spriteSize = 8 * (m_spriteWidth * m_spriteHeight);
 //tileEngine.update(m_tid, m_tiles + spriteIndex, spriteSize);

 //spriteEngine.setX(1, player1.x());
 //spriteEngine.setY(1, player1.y());
 //spriteEngine.setHFlip(1, player1.hflip());
 //spriteEngine.update();
 }
 }
 };
 */

#define SUBPIXELS 4
#define ABS(x) ((x)<0 ? -(x) : (x) )
//#define sign(x) (x<0 ? (-1) : (x>0 ? 1 : 0) )
#define sign(x) ((x)<0 ? (-1) : ((x)==0 ? 0 : 1) )

struct Position {
	s16 x;
	s16 y;
};

struct LineIterator {

	// Iterator constructors here...
	LineIterator(Position pa, Position pe) :
			m_pa(pa), m_pe(pe) {
		s16 dx = pe.x - pa.x;
		s16 dy = pe.y - pa.y;
		s16 nx = ABS(dx);
		s16 ny = ABS(dy);
		x = (nx > ny);
		sx = sign(dx);
		sy = sign(dy);
		if (nx > ny) {
			d = 2 * ny - nx;
			D0 = 2 * ny;
			DN0 = 2 * (ny - nx);
		} else {
			d = 2 * nx - ny;
			D0 = 2 * nx;
			DN0 = 2 * (nx - ny);
		}
	}

	Position operator*() const {
		return m_pa;
	}
	Position* operator->() {
		return &m_pa;
	}

	// Prefix increment
	LineIterator& operator++() {

		increase();
		return *this;
	}

	// Postfix increment
	LineIterator operator++(int) {
		LineIterator tmp = *this;
		increase();
		return tmp;
	}

	friend bool operator!=(const LineIterator &a, const LineIterator &b) {
		return a.m_pa.x != b.m_pa.x or a.m_pa.y != b.m_pa.y;
	}
	LineIterator begin() {
		return LineIterator(m_pa, m_pe);
	}
	LineIterator end() {
		return LineIterator(m_pe, m_pe);
	}
private:

	Position m_pa;
	Position m_pe;
	s16 d, D0, DN0, sx, sy;
	bool x;

	void increase() {
		if (x) {
			if (d <= 0) {
				d = d + D0;
			} else {
				d = d + DN0;
				m_pa.y += sy;
			}
			m_pa.x += sx;
		} else {
			if (d <= 0) {
				d = d + D0;
			} else {
				d = d + DN0;
				m_pa.x += sx;
			}
			m_pa.y += sy;
		}
	}
};

class PlayerIso {
	std::array<FixPoint, 3> m_pos;
	std::array<FixPoint, 3> m_speed;
	u16 m_aid, m_rt;
	u16 m_hflip, m_vflip;
	//SpriteSequence sprite;
	Sprites *m_sprites;
	Tiles *m_tiles;
	u16 m_tid;
	u16 m_sid;
public:
	PlayerIso(Sprites *sprites, Tiles *tiles) :
			m_pos( { 0.f, 0.f, 0.f }), m_speed( { 0.f, 0.f, 0.f }), m_sprites(
					sprites), m_tiles(tiles) //, sprite(4, 4, 8, marioTiles, 4)
	{
		/*#pragma unroll
		 for (const int dim : { 0, 1, 2 }) {
		 m_speed[dim] = 0;
		 }*/
		m_aid = 0;
		m_rt = 0;
		m_hflip = 0;
		m_vflip = 0;

		// add palette

		// add tiles
		m_tid = m_tiles->add(marioTiles, 8 * (4 * 4));

		// add sprite
		m_sid = m_sprites->add(140, 72, SPRITE_SIZE(4, 4),
				TILE_ATTR_FULL(PAL1, 1, 0, 0, m_tid));

	}

	void update(TileMap *collision, u16 joypad) {
		u16 joy_state = JOY_readJoypad(joypad);

		FixPoint jumpSpeed = 7.f;
		FixPoint walkSpeed = 2.f;
		FixPoint gravity = 1.f;

		m_speed[0] = 0;
		m_speed[1] = 0;

		if (joy_state & BUTTON_LEFT) {
			m_speed[0] -= walkSpeed;
			m_hflip = 1;
			m_rt++;
		}
		if (joy_state & BUTTON_RIGHT) {
			m_speed[0] += walkSpeed;
			m_hflip = 0;
			m_rt++;
		}
		if (joy_state & BUTTON_UP) {
			m_speed[1] -= walkSpeed;
			m_vflip = 1;
			m_rt++;
		}
		if (joy_state & BUTTON_DOWN) {
			m_speed[1] += walkSpeed;
			m_vflip = 0;
			m_rt++;
		}

		if (m_rt > 2 * 4)
			m_rt = 0;
//		if (m_rt >= 30) {
//			m_rt = 0;
//			m_aid = m_aid + 1;
//			if (m_aid > 4)
//				m_aid = 0;
//		}

		if (joy_state & BUTTON_A) {
			m_speed[2] = jumpSpeed;
		} else {
			if (m_pos[2] <= FixPoint(0.f)) {
				m_pos[2] = 0;
				m_speed[2] = 0;
			} else {
				m_speed[2] = m_speed[2] - gravity;
			}
		}

		m_pos[0] += m_speed[0];
		m_pos[1] += m_speed[1];
		m_pos[2] += m_speed[2];

		// update sprite
		m_sprites->setX(m_sid, x());
		m_sprites->setY(m_sid, y());
		m_sprites->setHFlip(m_sid, hflip());

		// update tiles
		const u32 *tiles = &marioTiles[animationID() * 8 * (4 * 4)];
		m_tiles->update(m_tid, tiles, 8 * (4 * 4));

	}
	s16 x() {
		return (s16) m_pos[0];
	}
	s16 y() {
		return (s16)(m_pos[1] - m_pos[2]);
	}
	u16 hflip() {
		return m_hflip;
	}
	u16 vflip() {
		return m_vflip;
	}
	u16 animationID() {
		return (m_rt >> 2) % 8;
	}
};

class Player {
private:
//	const s16 subpixels = 8;
	std::array<FixPoint, 2> m_pos;
	u16 m_w, m_h;
	std::array<FixPoint, 2> m_speed;
	u16 m_hflip, m_vflip;
	u8 m_aid, m_rt;

//	float f_speed[2];
//	float f_x, f_y;
	Sprites *m_sprites;
	Tiles *m_tiles;
	u16 m_tid;
	u16 m_sid;
public:
	Player(s16 x, s16 y, u16 w, u16 h, Sprites *sprites, Tiles *tiles) :
			m_pos( { x, y }), m_w(w), m_h(h), m_speed( { 0.f, 0.f }), m_sprites(
					sprites), m_tiles(tiles) {

		m_hflip = 0;
		m_vflip = 0;
		m_aid = 0;
		m_rt = 0;

		// add palette

		// add tiles
		m_tid = m_tiles->add(kirbyTiles, 8 * (1 * 1));

		// add sprite
		m_sid = m_sprites->add(140, 72, SPRITE_SIZE(1, 1),
				TILE_ATTR_FULL(PAL0, 1, 0, 0, m_tid));
	}
	s16 x() {
		//return (s16) m_x / SUBPIXELS;
		return (s16) m_pos[0];
	}
	s16 y() {
		//return (s16) m_y / SUBPIXELS;
		return (s16) m_pos[1];
	}
	u16 hflip() {
		return m_hflip;
	}
	u16 vflip() {
		return m_vflip;
	}
	u16 animationID() {
		return m_aid;
	}

	void update(TileMap *collision, u16 joypad) {
		u16 joy_state = JOY_readJoypad(joypad);

		// on
		bool on_ground = onGround(collision);

		//bool jump = (on_ground) and (joy_state & (BUTTON_A | BUTTON_UP));
		//bool jumpHeld = (!on_ground) and (joy_state & (BUTTON_A | BUTTON_UP));
		// player action
		bool jump = (joy_state & (BUTTON_A | BUTTON_UP));

		FixPoint walkSpeed = 2.f;
		FixPoint jumpSpeed = 7.f;
		FixPoint maxGravity = 0.5f;
		FixPoint maxFallSpeed = 4.f;

		// vertical speed
		if (on_ground) {
			// on ground
			if (jump)
				m_speed[1] = jumpSpeed;
			else
				m_speed[1] = 0;

		} else {
			// in air
			if (jump) {
				m_speed[1] = std::max(m_speed[1] - maxGravity / (s16) 2,
						-maxFallSpeed);
			} else {
				m_speed[1] = std::max(m_speed[1] - maxGravity, -maxFallSpeed);
			}

		}

		// horizontal speed
		m_speed[0] = 0;
		if (joy_state & BUTTON_LEFT) {
			m_speed[0] -= walkSpeed;
			m_hflip = 1;
		}
		if (joy_state & BUTTON_RIGHT) {
			m_speed[0] += walkSpeed;
			m_hflip = 0;
		}

		// animation
		bool is_running = ((s16) m_speed[0] != 0) and on_ground;
		if (is_running) {
			m_rt++;
			if (m_rt == 4) {
				m_rt = 0;
				m_aid++;
			}
		} else {
			m_rt = 0;
			m_aid = 0;
		}

		//m_aid = (u16)(m_rt / 2);
//		m_x += m_speed[0];

// raster line
		/*FixPoint sign = (m_speed[0] < FixPoint(0.f) ? -1.f : +1.f);
		 FixPoint dst = m_speed[0]+m_x;
		 for(; (s16)m_x != (s16)dst; m_x += sign)
		 if( colliding(collision)){
		 m_x -= sign;
		 break;
		 }*/
		FixPoint zero = 0.f;
		if (m_speed[0] == zero && m_speed[1] == zero)
			return;

		//m_x += sign;
		FixPoint dx, dy;
		s16 n;
		if (m_speed[0].fabs() > m_speed[1].fabs()) {
			n = m_speed[0].fabs();
			dx = m_speed[0] / m_speed[0].fabs();
			dy = m_speed[1] / m_speed[0].fabs();
		} else {
			n = m_speed[1].fabs();
			dx = m_speed[0] / m_speed[1].fabs();
			dy = m_speed[1] / m_speed[1].fabs();
		}

		for (s16 i = 0; i < n; i++) {
			m_pos[0] += dx;
			if (colliding(collision)) {
				m_pos[0] -= dx;
				m_speed[0] = 0;
			}
			m_pos[1] -= dy;
			if (colliding(collision)) {
				m_pos[1] += dy;
				m_speed[1] = 0;
			}
		}

		m_sprites->setX(0, x());
		m_sprites->setY(0, y());
		m_sprites->setHFlip(0, hflip());
	}

	u16 pos2tid(s16 pos) {
		u16 p = (u16) pos;
		return p >> 3;
	}

	bool colliding(TileMap *collision) {
		u16 empty = collision->tilemap[0];

		u16 t_x, t_y;

		// top
		t_y = pos2tid(y());
		for (t_x = pos2tid(x()); t_x <= pos2tid(x() + m_w); t_x++) {
			if (collision->tilemap[t_x + t_y * collision->w] != empty)
				return true;
		}
		// ground
		t_y = pos2tid(y() + m_h);
		for (t_x = pos2tid(x()); t_x <= pos2tid(x() + m_w); t_x++) {
			if (collision->tilemap[t_x + t_y * collision->w] != empty)
				return true;
		}

		// left
		t_x = pos2tid(x());
		for (t_y = pos2tid(y()) + 1; t_y <= pos2tid(y() + m_h) - 1; t_y++) {
			if (collision->tilemap[t_x + t_y * collision->w] != empty)
				return true;
		}
		// right
		t_x = pos2tid(x() + m_w);
		for (t_y = pos2tid(y()) + 1; t_y <= pos2tid(y() + m_h) - 1; t_y++) {
			if (collision->tilemap[t_x + t_y * collision->w] != empty)
				return true;
		}
		return false;
	}

	bool onGround(TileMap *collision) {
		u16 empty = collision->tilemap[0];
		u16 t_y = pos2tid(y() + 1 + m_h);
		for (u16 t_x = pos2tid(x()); t_x <= pos2tid(x() + m_w); t_x++) {
			u16 tid = t_x + t_y * collision->w;

			if (collision->tilemap[tid] != empty)
				return true;
		}
		return false;
	}
//	bool onWall(TileMap *collision) {
//		u16 empty = collision->tilemap[0];
//		u16 t_x = pos2tid(x());
//		for(u16 t_x = pos2tid(x()); t_x <= pos2tid(x() + m_w); t_x++){
//			u16 tid = t_x + t_y * collision->w;
//
//			if (collision->tilemap[tid] != empty)
//				return true;
//		}
//		return false;
//	}
};

int main(int hardReset) {
	VDP_setScreenWidth320();
	SYS_disableInts();
	// PALETTES
	u16 kirbyPid = PAL0;
	u16 mushroomPid = PAL1;
	u16 bgaPid = PAL2;
	u16 bgbPid = PAL3;
	VDP_setPalette(kirbyPid, kirbyPal);
	VDP_setPalette(mushroomPid, marioPal);
//	VDP_setPalette(mushroomPid, charPal);
//	VDP_setPalette(mushroomPid, quietschiPal);
	VDP_setPalette(bgaPid, bgAPal);
	VDP_setPalette(bgbPid, bgBPal);
//
//	// TILES
	Tiles tileEngine;

	// SPRITES
	Sprites spriteEngine;

	// BACKGROUND
	u16 bgaTid = tileEngine.add(bgATiles, bgATilesLen);
	u16 bgbTid = tileEngine.add(bgBTiles, bgBTilesLen);

	TileMap map;
	map.compression = COMPRESSION_NONE;
	map.h = 32;
	map.w = 64;
	map.tilemap = static_cast<u16*>(bgBTileMap);
	VDP_setTileMapEx(BG_B, &map,
			TILE_ATTR_FULL(bgbPid, FALSE, FALSE, FALSE, bgbTid), 0, 0, 0, 0, 64,
			32, DMA);
	map.tilemap = static_cast<u16*>(bgATileMap);
	VDP_setTileMapEx(BG_A, &map,
			TILE_ATTR_FULL(bgaPid, FALSE, FALSE, FALSE, bgaTid), 0, 0, 0, 0, 64,
			32, DMA);
	//VDP_setTileMap(BG_A, &map, 0, 0, 64, 32, DMA);
	//VDP_drawText("Hello world !", 12, 12);

	Player player0(20, 40, 8, 8, &spriteEngine, &tileEngine);
	PlayerIso player1(&spriteEngine, &tileEngine);

	list<Projectile> projectiles;
	//int n_projectiles = 0;

//	VDP_setPaletteColors(0,  bgAPal, bgAPalLen);
	SYS_enableInts();
	//u16 aid_old = 0;

	u16 joy1_before = JOY_readJoypad(JOY_1);

#pragma GCC unroll 1
	while (true) {
		// nothing to do here
		// ...

		player0.update(&map, JOY_2);

		player1.update(&map, JOY_1);

		u16 joy1_now = JOY_readJoypad(JOY_1);
		if ((joy1_now & BUTTON_B) and not (joy1_before & BUTTON_B)) {
			if (projectiles.empty()) {
				projectiles.emplace(player1.x(), player1.y(), player1.hflip(), &spriteEngine,
						&tileEngine);
			} else {
				projectiles.pop();
				//spriteEngine.remove(2);
			}
		}
		joy1_before = joy1_now;

		/*for (auto projectile : projectiles.iter()) {
		 projectile->update();
		 }*/
		if (not projectiles.empty()) {
			projectiles[0]->update();
		}

		spriteEngine.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
