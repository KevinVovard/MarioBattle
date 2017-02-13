#pragma once
#include "Character.h"
//abstract class from which spiny and turtle inherites
class Enemy:public Character
{
public:
	Enemy(void);

	~Enemy(void);

	void VerifyMapCollision(Map* map);

    void SetPositionAndOrientation(int x, int y, Orientation orientation);
};

