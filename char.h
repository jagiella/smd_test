const u16 charPal[] = {
	0x0E0E, 
	0x0000, 
	0x022E, 
	0x0228, 
	0x046C, 
	0x0EEE, 
	0x08AE, 
	0x0222, 
	0x0840, 
	0x0666, 
};
const u16 charPalLen = 10;
const u32 charTiles[] = {
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000001, 
	0x00000001, 
	0x00000113, 
	0x00001322, 
	0x00013222, 
	0x00013222, 
	0x00012222, 
	0x00133222, 
	0x00013332, 
	0x00013143, 
	0x00001146, 
	0x00000014, 
	0x00000177, 
	0x00001447, 
	0x00001661, 
	0x00000111, 
	0x00000013, 
	0x00000001, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x10000000, 
	0x21000000, 
	0x23100000, 
	0x22111100, 
	0x22332310, 
	0x23323100, 
	0x22222310, 
	0x22222231, 
	0x22232310, 
	0x33112100, 
	0x31541000, 
	0x36564100, 
	0x66641000, 
	0x11110000, 
	0x71100000, 
	0x77100000, 
	0x81000000, 
	0x91000000, 
	0x11000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000001, 
	0x00000001, 
	0x00000113, 
	0x00001322, 
	0x00013222, 
	0x00013222, 
	0x00012222, 
	0x00133222, 
	0x00013332, 
	0x00013143, 
	0x00001146, 
	0x00000114, 
	0x00000171, 
	0x00001877, 
	0x00013881, 
	0x00019111, 
	0x00001100, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x10000000, 
	0x21000000, 
	0x23100000, 
	0x22111100, 
	0x22332310, 
	0x23323100, 
	0x22222310, 
	0x22222231, 
	0x22232310, 
	0x33112100, 
	0x31541000, 
	0x36564100, 
	0x66641000, 
	0x11110000, 
	0x46111100, 
	0x66193100, 
	0x11331000, 
	0x00110000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000001, 
	0x00000001, 
	0x00000113, 
	0x00001322, 
	0x00013222, 
	0x00013222, 
	0x00012222, 
	0x00133222, 
	0x00013332, 
	0x00013143, 
	0x00001146, 
	0x00000014, 
	0x00000177, 
	0x00001447, 
	0x00001661, 
	0x00000111, 
	0x00000013, 
	0x00000001, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x10000000, 
	0x21000000, 
	0x23100000, 
	0x22111100, 
	0x22332310, 
	0x23323100, 
	0x22222310, 
	0x22222231, 
	0x22232310, 
	0x33112100, 
	0x31541000, 
	0x36564100, 
	0x66641000, 
	0x11110000, 
	0x71100000, 
	0x77100000, 
	0x81000000, 
	0x91000000, 
	0x11000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000001, 
	0x00000001, 
	0x00000113, 
	0x00001322, 
	0x00013222, 
	0x00013222, 
	0x00012222, 
	0x00133222, 
	0x00013332, 
	0x00013143, 
	0x00001146, 
	0x00000114, 
	0x00011777, 
	0x00164777, 
	0x00166188, 
	0x00111111, 
	0x00001100, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x10000000, 
	0x21000000, 
	0x23100000, 
	0x22111100, 
	0x22332310, 
	0x23323100, 
	0x22222310, 
	0x22222231, 
	0x22232310, 
	0x33112100, 
	0x31541000, 
	0x36564100, 
	0x66641000, 
	0x11110000, 
	0x77111100, 
	0x18893100, 
	0x11331000, 
	0x00110000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
	0x00000000, 
};
const u16 charTilesLen = 512;