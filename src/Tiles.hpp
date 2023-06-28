/*
 * Tiles.hpp
 *
 *  Created on: 21.06.2023
 *      Author: nickj
 */

#ifndef SRC_TILES_HPP_
#define SRC_TILES_HPP_

class Tiles {
	u16 n;
public:
	Tiles();
	u16 add(const u32 *tiles, u16 num);
	void update(u16 tid, const u32 *tiles, u16 num);
	void remove(u16 index, u16 num);
};



#endif /* SRC_TILES_HPP_ */
