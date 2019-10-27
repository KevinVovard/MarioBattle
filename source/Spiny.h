#pragma once
#include "enemy.h"
class Spiny : public Enemy
{
public:
	Spiny(void);
	~Spiny(void);
	void ProcessInput(float dt, bool isGameFreezed);
	void UpdateTile(float dt, bool isGameFreezed);
	int nextWalkingTile(float dt,float threshold);
	int nextTurningTile(float dt);
	void VerifyMapCollision(Map* map);
	void CollisionEffect(Character* character);

protected:
	bool m_isFalling;
	bool m_isTurning;
	bool m_hasChangedOrientationDuringTurning;
	int m_turningCount;
	double m_elapsedTimeTurningCount;
};

