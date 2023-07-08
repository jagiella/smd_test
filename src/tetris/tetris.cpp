/*
 * tetris.cpp
 *
 *  Created on: 08.07.2023
 *      Author: nick
 */

extern "C" {
#include "genesis.h"
}


int main(int hardReset) {
	VDP_setScreenWidth320();
	SYS_disableInts();



	SYS_enableInts();


#pragma GCC unroll 1
	while (true) {
		// nothing to do here
		// ...





		// always call this method at the end of the frame
		SYS_doVBlankProcess();
	}

	return 0;
}
