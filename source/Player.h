#pragma once
#include "Character.h"
#include "InputDevice.h"

//abstract class from which Spawn inherites
class Player : public Character
{
public:
	Player(InputDevice* inputDevice);
	
	~Player(void);	
	
	void Player::ProcessInput(float dt);

	void Player::UpdateTile(float dt);
	
	void CollisionEffect(Character* character);

	// returns the player speed on the X axis
	double GetSpeedX();

	void UpdatePosition();

	// Set Is Jumped On
	void SetIsJumpedOn(bool isJumpedOn);

	void SetIsJumping(bool isJumping);

	void SetIsPushed(bool isPushed);

	//void SetIsBig(bool isBig);

protected:
	InputDevice* m_inputDevice;
	double m_lastSpeedX;
	double m_lastSpeedY;
	double m_mass = 70.0;
	bool m_isJumpedOn = false;
	int m_isJumpedOnFrameCount = 0;
	bool m_isPushed = false;
	int m_isPushedFrameCount = 0;
	//bool m_isInjured = false;
	//int m_isInjuredFrameCount = 0;
	//bool m_isKilled = false;
	//int m_isKilledFrameCount = 0;
	//bool m_isBig = true;
};

