extern "C" {
#include "genesis.h"
}
;
#include "kirby.h"
#include "mushroom.h"
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

	void setX(int index, s16 x){
		sprites[index].x = x + 0x80;
	}
	void setY(int index, s16 y){
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

int main(bool hardReset) {
	VDP_setScreenWidth320();
	SYS_disableInts();
	// PALETTES
	u16 kirbyPid = PAL0;
	u16 mushroomPid = PAL1;
	u16 bgaPid = PAL2;
	u16 bgbPid = PAL3;
	VDP_setPalette(kirbyPid, kirbyPal);
	VDP_setPalette(mushroomPid, mushroomPal);
	VDP_setPalette(bgaPid, bgAPal);
	VDP_setPalette(bgbPid, bgBPal);
//
//	// TILES
	Tiles tileEngine;
	u16 kirbyTid = tileEngine.add(kirbyTiles, kirbyTilesLen);
	u16 mushroomTid = tileEngine.add(mushroomTiles, mushroomTilesLen);

	// SPRITES
	Sprites spriteEngine(128);
	spriteEngine.add(10, 10, SPRITE_SIZE(1, 1),
			TILE_ATTR_FULL(kirbyPid, 1, 0, 0, kirbyTid));
	spriteEngine.add(100, 100, SPRITE_SIZE(2, 2),
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
	map.tilemap = bgATileMap;
	VDP_setTileMapEx(BG_B, &map, TILE_ATTR_FULL(bgaPid, FALSE, FALSE, FALSE, bgaTid), 0, 0,  0, 0, 64, 32, DMA);
	map.tilemap = bgBTileMap;
	VDP_setTileMapEx(BG_A, &map, TILE_ATTR_FULL(bgbPid, TRUE, FALSE, FALSE, bgbTid), 0, 0,  0, 0, 64, 32, DMA);
	//VDP_setTileMap(BG_A, &map, 0, 0, 64, 32, DMA);
	//VDP_drawText("Hello world !", 12, 12);


//	VDP_setPaletteColors(0,  bgAPal, bgAPalLen);
	SYS_enableInts();
	s16 x=10,y=10;
	while (true) {
		// nothing to do here
		// ...

		u16 joy_state = JOY_readJoypad(JOY_1);
		if(joy_state & BUTTON_UP)
			y--;
		if(joy_state & BUTTON_DOWN)
			y++;
		if(joy_state & BUTTON_LEFT)
			x--;
		if(joy_state & BUTTON_RIGHT)
			x++;
		spriteEngine.setX(0,x);
		spriteEngine.setY(0,y);
		spriteEngine.update();

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
