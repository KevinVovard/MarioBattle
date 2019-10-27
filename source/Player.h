#pragma once
#include "Character.h"
#include "InputDevice.h"

//abstract class from which Spawn inherites
class Player : public Character
{
public:
	Player(InputDevice* inputDevice);
	
	~Player(void);	
	
	void ProcessInput(float dt, bool isGameFreezed);

	void UpdateTile(float dt, bool isGameFreezed);

	int nextWalkingTile(float dt, float threshold);

	int nextInjuredTile(float dt);
	
	void CollisionEffect(Character* character);

	// returns the player speed on the X axis
	double GetSpeedX();

	bool GetIsFreezed() { return m_isInjured || m_isGrowing || m_isDying; }

	void UpdatePosition();

	// Set Is Jumped On
	void SetIsJumpedOn(bool isJumpedOn);

	void SetIsJumping(bool isJumping);

	void SetIsPushed(bool isPushed);

	void SetIsBig(bool isBig);

protected:
	InputDevice* m_inputDevice;
	double m_lastSpeedX;
	double m_lastSpeedY;
	double m_mass = 70.0;

	bool m_isJumpedOn = false;
	int m_isJumpedOnFrameCount = 0;

	bool m_isPushed = false;
	int m_isPushedFrameCount = 0;

	bool m_isInjured = false;
	int m_isInjuredFrameCount = 0;
	double m_elapsedTimeInjuredCount = 0;
	int m_injuredCount = 0;

	bool m_isDying = false;
	int m_isDyingFrameCount = 0;

	bool m_isTemporarilyInvincible = false;
	int m_isTemporarilyInvincibleFrameCount = 0;

	bool m_isBig = true;
	bool m_isGrowing = false;
};

