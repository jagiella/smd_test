/*
 * Projectile.cpp
 *
 *  Created on: 26.06.2023
 *      Author: nickj
 */

#include "Projectile.hpp"

#include "../mushroom.h"


Projectile::Projectile(): m_sprites(nullptr), m_tiles(nullptr){

}

Projectile::Projectile(Sprites *sprites, Tiles *tiles) :
		m_sprites(sprites), m_tiles(tiles) {
	// add tiles
	m_tid = m_tiles->add(mushroomTiles, 8 * (2 * 2));

	// add sprite
	m_sid = m_sprites->add(140, 72, SPRITE_SIZE(2, 2),
			TILE_ATTR_FULL(PAL1, 1, 0, 0, m_tid));
}

Projectile::~Projectile(){
	m_tiles->remove(m_tid, 8 * (2 * 2));
	m_sprites->remove(m_sid);
}

void Projectile::update(){
	m_sprites->setX(m_sid, 80);
	m_sprites->setY(m_sid, 80);
}
