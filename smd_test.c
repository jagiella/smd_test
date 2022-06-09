#include "genesis.h"
#include "kirby.h"
#include "mushroom.h"


int main(bool hardReset)
{
	int kirbyTid = TILE_USERINDEX+0;
	int kirbyPid = PAL1;
	int mushroomTid = TILE_USERINDEX+1;
	int mushroomPid = PAL2;

	VDP_setPalette(/*pid*/kirbyPid, kirbyPal);
	VDP_setPalette(/*pid*/mushroomPid, mushroomPal);

	VDP_loadTileData(kirbyTiles, kirbyTid, 4, CPU);
	VDP_loadTileData(mushroomTiles, mushroomTid, 4, CPU);

	VDPSprite sprites[2];

	sprites[0].x = 0x80 + 10;
	sprites[0].y = 0x80 + 10;
	sprites[0].size = SPRITE_SIZE(1,1);
	sprites[0].link = 1;
	sprites[0].attribut = TILE_ATTR_FULL(kirbyPid, 1, 0, 0, kirbyTid);

	sprites[1].x = 0x80 + 50;
	sprites[1].y = 0x80 + 50;
	sprites[1].size = SPRITE_SIZE(2,2);
	sprites[1].link = 0;
	sprites[1].attribut = TILE_ATTR_FULL(mushroomPid, 1, 0, 0, mushroomTid);
	DMA_transfer(CPU, DMA_VRAM, sprites, VDP_SPRITE_TABLE, (sizeof(VDPSprite) * 2) / 2, 2);

    //VDP_drawText("Hello world !", 12, 12);

    while(TRUE)
    {
        // nothing to do here
        // ...

        // always call this method at the end of the frame
        SYS_doVBlankProcess();
    }

    return 0;
}
