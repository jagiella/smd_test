/*
 * Projectile.hpp
 *
 *  Created on: 26.06.2023
 *      Author: nickj
 */

#ifndef SRC_PROJECTILE_HPP_
#define SRC_PROJECTILE_HPP_

extern "C" {
#include "genesis.h"
}
#include "Sprites.hpp"
#include "Tiles.hpp"
#include "fix16.hpp"

#include <array>

class Projectile {
	std::array<FixPoint,2> m_pos;
	u16 m_hflip;
	Sprites *m_sprites;
	Tiles *m_tiles;
	u16 m_tid;
	u16 m_sid;
public:
	Projectile();
	Projectile(s16 x, s16 y, u16 hflip, Sprites *sprites, Tiles *tiles);
	~Projectile();
	void update();
};

#endif /* SRC_PROJECTILE_HPP_ */
