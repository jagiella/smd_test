extern "C" {
#include "genesis.h"
}
;
#include "kirby.h"
#include "mario.h"
#include "mushroom.h"
#include "quietschi.h"
#include "bgA.h"
#include "bgB.h"

//#include <iterator> // For std::forward_iterator_tag
//#include <cstddef>  // For std::ptrdiff_t

class Sprites {
	int maxlen;
	int n;
	VDPSprite sprites[2];
public:
	Sprites(int maxlen) :
			maxlen(maxlen) {
		n = 0;

	}
	void add(u16 x, u16 y, u8 size, u16 attr) {
		if (n > 0)
			sprites[n - 1].link = n;
		sprites[n].link = 0;
		sprites[n].attribut = attr;
		sprites[n].size = size;
		sprites[n].x = x + 0x80;
		sprites[n].y = y + 0x80;
		n++;
	}

	void setX(int index, s16 x) {
		sprites[index].x = x + 0x80;
	}
	void setY(int index, s16 y) {
		sprites[index].y = y + 0x80;
	}
	void setHFlip(int index, u16 hflip){
		sprites[index].attribut = (sprites[index].attribut & (~TILE_ATTR_HFLIP_MASK)) + (hflip << TILE_ATTR_HFLIP_SFT);
	}

	void update() {
		DMA_transfer(DMA, DMA_VRAM, sprites, VDP_SPRITE_TABLE,
				n * sizeof(VDPSprite) / 2, 2);
	}
};

class Tiles {
	u16 n;
public:
	Tiles() {
		n = TILE_USERINDEX;
	}
	u16 add(const u32 *tiles, u16 num) {
		u16 tid = n;
		VDP_loadTileData(tiles, tid, num / 8, DMA);
		n += num / 8;
		return tid;
	}

	void update(u16 tid, const u32 *tiles, u16 num){
		VDP_loadTileData(tiles, tid, num / 8, DMA);
	}
};

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

class Player {
private:
//	const s16 subpixels = 8;
	s16 m_x, m_y;
	u16 m_w, m_h;
	s16 m_speed[2];
	u16 m_hflip, m_vflip;
	u8 m_aid, m_rt;
//	float f_speed[2];
//	float f_x, f_y;
public:
	Player(s16 x, s16 y, u16 w, u16 h) :
			m_x(x * SUBPIXELS), m_y(y * SUBPIXELS), m_w(w), m_h(h) {
		m_speed[0] = 0;
		m_speed[1] = 0;
		m_hflip = 0;
		m_vflip = 0;
		m_aid = 0;m_rt=0;
//		f_speed[0] = 0;
//		f_speed[1] = 0;
//		f_x = 0;
//		f_y = 0;
	}
	s16 x() {
		return (s16) m_x / SUBPIXELS;
	}
	s16 y() {
		return (s16) m_y / SUBPIXELS;
	}
	u16 hflip(){return m_hflip;}
	u16 vflip(){return m_vflip;}
	u16 animationID(){return m_aid;}


	void update(TileMap *collision, u16 joypad) {
		u16 joy_state = JOY_readJoypad(joypad);
		m_y++;
		char on_ground = onGround(collision);
		m_y--;

		bool jump = (on_ground) and (joy_state & (BUTTON_A|BUTTON_UP));
		bool jumpHeld = (!on_ground) and (joy_state & (BUTTON_A|BUTTON_UP));

		s16 jumpSpeed = SUBPIXELS*7;
		s16 maxGravity = SUBPIXELS/2;
		s16 maxFallSpeed = SUBPIXELS*4;

		if (jump)
			m_speed[1] = jumpSpeed;

		if (!jumpHeld and !on_ground) {
			m_speed[1] = max(m_speed[1] - maxGravity, -maxFallSpeed);
		} else if (jumpHeld and !on_ground) {
			m_speed[1] = max(m_speed[1] - maxGravity / 2, -maxFallSpeed);
		}

		if (on_ground and !jump) {
			// stop
			m_speed[1] = 0;
		} else {
			// move
//			m_y -= m_speed[1];
		}
//		if (joy_state & BUTTON_UP)
//			m_y--;
//		if (joy_state & BUTTON_DOWN)
//			m_y++;
		//bool was_running = (m_speed[0]!=0);
		m_speed[0] = 0;
		if (joy_state & BUTTON_LEFT) {
			m_speed[0] -= SUBPIXELS*2;
			m_hflip = 1;
		}
		if (joy_state & BUTTON_RIGHT) {
			m_speed[0] += SUBPIXELS*2;
			m_hflip = 0;
		}
		bool is_running = (m_speed[0]!=0) and on_ground;
		if(is_running){
			m_rt ++;
			if(m_rt == 4){
				m_rt = 0;
				m_aid++;
			}
		}else{
			m_rt=0;
			m_aid = 0;
		}

		//m_aid = (u16)(m_rt / 2);
//		m_x += m_speed[0];

// raster line
		s16 x2 = m_x + m_speed[0];
		s16 y2 = m_y - m_speed[1];
		LineIterator it( { m_x, m_y }, { x2, y2 });
		for (auto p : it) {
			for(u8 i=0; i<7; i++)
				it++;
			if(p.x != m_x){
				s16 old_x = m_x;
				m_x = p.x;
				if (colliding(collision))
					m_x = old_x;
			}
			if(p.y != m_y){
				s16 old_y = m_y;
				m_y = p.y;
				if (colliding(collision))
					m_y = old_y;
			}
		}
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
		u16 t_y = pos2tid(y() + m_h);
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

int main(bool hardReset) {
	VDP_setScreenWidth320();
	SYS_disableInts();
	// PALETTES
	u16 kirbyPid = PAL0;
	u16 mushroomPid = PAL1;
	u16 bgaPid = PAL2;
	u16 bgbPid = PAL3;
	VDP_setPalette(kirbyPid, kirbyPal);
	VDP_setPalette(mushroomPid, marioPal);
//	VDP_setPalette(mushroomPid, quietschiPal);
	VDP_setPalette(bgaPid, bgAPal);
	VDP_setPalette(bgbPid, bgBPal);
//
//	// TILES
	Tiles tileEngine;
	u16 kirbyTid = tileEngine.add(kirbyTiles, kirbyTilesLen);
//	u16 mushroomTid = tileEngine.add(quietschiTiles, quietschiTilesLen);
	u16 mushroomTid = tileEngine.add(marioTiles, 8*(4*4));

	// SPRITES
	Sprites spriteEngine(128);
	spriteEngine.add(10, 10, SPRITE_SIZE(1, 1),
			TILE_ATTR_FULL(kirbyPid, 1, 0, 0, kirbyTid));
	spriteEngine.add(140, 72, SPRITE_SIZE(4, 4),
			TILE_ATTR_FULL(mushroomPid, 1, 0, 0, mushroomTid));
	spriteEngine.update();

	// BACKGROUND
	/*TileSet bg_a;
	 bg_a.compression = COMPRESSION_NONE;
	 bg_a.numTile = bgATilesLen / 8;
	 bg_a.tiles = bgATiles;*/
	u16 bgaTid = tileEngine.add(bgATiles, bgATilesLen);
	u16 bgbTid = tileEngine.add(bgBTiles, bgBTilesLen);

	TileMap map;
	map.compression = COMPRESSION_NONE;
	map.h = 32;
	map.w = 64;
	map.tilemap = bgBTileMap;
	VDP_setTileMapEx(BG_B, &map,
			TILE_ATTR_FULL(bgbPid, FALSE, FALSE, FALSE, bgbTid), 0, 0, 0, 0, 64,
			32, DMA);
	map.tilemap = bgATileMap;
	VDP_setTileMapEx(BG_A, &map,
			TILE_ATTR_FULL(bgaPid, FALSE, FALSE, FALSE, bgaTid), 0, 0, 0, 0, 64,
			32, DMA);
	//VDP_setTileMap(BG_A, &map, 0, 0, 64, 32, DMA);
	//VDP_drawText("Hello world !", 12, 12);

	Player player0(20, 40, 8, 8);
	Player player1(20, 40, 32, 32);

//	VDP_setPaletteColors(0,  bgAPal, bgAPalLen);
	SYS_enableInts();
	while (true) {
		// nothing to do here
		// ...

		player0.update(&map, JOY_1);
		spriteEngine.setX(0, player0.x());
		spriteEngine.setY(0, player0.y());
		spriteEngine.setHFlip(0, player0.hflip());

		player1.update(&map, JOY_2);
		tileEngine.update(mushroomTid, marioTiles+8*(4*4)*(player1.animationID()%8), 8*(4*4));
		spriteEngine.setX(1, player1.x());
		spriteEngine.setY(1, player1.y());
		spriteEngine.setHFlip(1, player1.hflip());
		spriteEngine.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
