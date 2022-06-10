extern "C" {
#include "genesis.h"
}
;
#include "kirby.h"
#include "mushroom.h"

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
	// PALETTES
	int kirbyPid = PAL1;
	int mushroomPid = PAL2;
	VDP_setPalette(kirbyPid, kirbyPal);
	VDP_setPalette(mushroomPid, mushroomPal);

	// TILES
	Tiles tileEngine;
	int kirbyTid = tileEngine.add(kirbyTiles, kirbyTilesLen);
	int mushroomTid = tileEngine.add(mushroomTiles, mushroomTilesLen);

	// SPRITES
	Sprites spriteEngine(128);
	spriteEngine.add(10, 10, SPRITE_SIZE(1, 1),
			TILE_ATTR_FULL(kirbyPid, 1, 0, 0, kirbyTid));
	spriteEngine.add(100, 100, SPRITE_SIZE(2, 2),
			TILE_ATTR_FULL(mushroomPid, 1, 0, 0, mushroomTid));
	spriteEngine.update();

	//VDP_drawText("Hello world !", 12, 12);

	while (true) {
		// nothing to do here
		// ...

		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
