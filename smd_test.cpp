extern "C" {
#include "genesis.h"
}
;
#include "kirby.h"
#include "mushroom.h"
#include "quietschi.h"
#include "bgA.h"
#include "bgB.h"

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

	void update() {
		DMA_transfer(DMA, DMA_VRAM, sprites, VDP_SPRITE_TABLE,
				n * sizeof(VDPSprite) / 2, 2);
	}
};

class Tiles {
	int n;
public:
	Tiles() {
		n = TILE_USERINDEX;
	}
	int add(const u32 *tiles, u16 num) {
		int tid = n;
		VDP_loadTileData(tiles, tid, num / 8, DMA);
		n += num / 8;
		return tid;
	}
};

#define SUBPIXELS 8
#define ABS(x) (x<0 ? (-x) : x )
#define SIGN(x) (x<0 ? (-1) : (x>0 ? 1 : 0) )

class Player {
private:
//	const s16 subpixels = 8;
	s16 m_x, m_y;
	u16 m_w, m_h;
	s16 m_speed[2];
//	float f_speed[2];
//	float f_x, f_y;
public:
	Player(s16 x, s16 y, u16 w, u16 h) :
			m_x(x*SUBPIXELS), m_y(y*SUBPIXELS), m_w(w), m_h(h) {
		m_speed[0] = 0;
		m_speed[1] = 0;
//		f_speed[0] = 0;
//		f_speed[1] = 0;
//		f_x = 0;
//		f_y = 0;
	}
	s16 x() {
		return (s16) m_x/SUBPIXELS;
	}
	s16 y() {
		return (s16) m_y/SUBPIXELS;
	}
	void update(TileMap *collision, u16 joypad) {
		u16 joy_state = JOY_readJoypad(joypad);
		char on_ground = onGround(collision);

		bool jump = (on_ground) and (joy_state & BUTTON_UP);
		bool jumpHeld = (!on_ground) and (joy_state & BUTTON_UP);

		s16 jumpSpeed = 40;
		s16 maxGravity   = 2;
		s16 maxFallSpeed = 30;

		if(jump)
			m_speed[1] = jumpSpeed;

		if(!jumpHeld and !on_ground){
			m_speed[1] = max(m_speed[1]-maxGravity, - maxFallSpeed);
		}else
			if(jumpHeld and !on_ground){
				m_speed[1] = max(m_speed[1]-maxGravity/2, - maxFallSpeed);
			}

		if(on_ground and !jump){
			// stop
			m_speed[1] = 0;
		}else{
			// move
			//m_y -= m_speed[1];
		}
//		if (joy_state & BUTTON_UP)
//			m_y--;
//		if (joy_state & BUTTON_DOWN)
//			m_y++;
		m_speed[0]=0;
		if (joy_state & BUTTON_LEFT){
			m_speed[0]-=8;
		}
		if (joy_state & BUTTON_RIGHT){
			m_speed[0]=+8;
		}
//		m_x += m_speed[0];

		// raster line
		s16 x1 = m_x;
		s16 y1 = m_y;
		s16 x2 = m_x+m_speed[0];
		s16 y2 = m_y-m_speed[1];
		s16 dx = x2 - x1;
		s16 dy = y2 - y1;
		if(abs(dx) > abs(dy)){
			if(dx != 0)
				for (m_x=x1; m_x!=x2; m_x+=SIGN(dx)){
//					s16 tmp =  dy * (m_x - x1);
////					tmp /= dx;
//					tmp += y1;
//					m_y = tmp;
					while(m_y*dx != y1 + (dy * (m_x - x1)))
						m_y = m_y + SIGN(dy);

	//				m_y = y1 + (dy * (m_x - x1)) / dx;
				}
		}
//		else{
//			if(dy != 0)
//				for (m_y=y1; m_y!=y2; m_y+=SIGN(dy))
//					m_x = x1 + (dx * (m_y - y1));// / dy;
//		}

//
//		if (onGround(collision) == FALSE)
//			m_y++;
//		else{
//
//		}
	}
	bool onGround(TileMap *collision) {
		u16 i_x = (u16) x();
		u16 i_y = (u16) y() + m_h;

		u16 empty = collision->tilemap[0];

		u16 tid = (i_x >> 3) + collision->w * (i_y >> 3);

		if (collision->tilemap[tid] != empty)
			return TRUE;
		else
			return FALSE;
	}
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
	VDP_setPalette(mushroomPid, quietschiPal);
	VDP_setPalette(bgaPid, bgAPal);
	VDP_setPalette(bgbPid, bgBPal);
//
//	// TILES
	Tiles tileEngine;
	u16 kirbyTid = tileEngine.add(kirbyTiles, kirbyTilesLen);
	u16 mushroomTid = tileEngine.add(quietschiTiles, quietschiTilesLen);

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

	Player player0(0, 0, 8, 8);
	Player player1(0, 0, 32, 32);

//	VDP_setPaletteColors(0,  bgAPal, bgAPalLen);
	SYS_enableInts();
	while (true) {
		// nothing to do here
		// ...

		player0.update(&map, JOY_1);
		spriteEngine.setX(0, player0.x());
		spriteEngine.setY(0, player0.y());

		player1.update(&map, JOY_2);
		spriteEngine.setX(1, player1.x());
		spriteEngine.setY(1, player1.y());

		spriteEngine.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
