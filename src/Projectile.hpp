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

class Projectile {
	Sprites *m_sprites;
	Tiles *m_tiles;
	u16 m_tid;
	u16 m_sid;
public:
	Projectile();
	Projectile(Sprites *sprites, Tiles *tiles);
	~Projectile();
	void update();
};

#endif /* SRC_PROJECTILE_HPP_ */
