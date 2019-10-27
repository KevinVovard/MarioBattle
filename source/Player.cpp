#include "Player.h"
#include "Spiny.h"

const double InjuredTimeThreshold = 0.1;

Player::Player(InputDevice* inputDevice) : m_inputDevice(inputDevice)
{
}

Player::~Player(void)
{
	delete m_inputDevice;
}

void Player::ProcessInput(float dt, bool isGameFreezed)
{
	if (isGameFreezed)
		return;

	// Left arrow button pressed
	if (m_inputDevice->IsSelected(PlayerAction::MoveToTheLeft))
	{
		// Speed button pressed
		if (m_inputDevice->IsSelected(PlayerAction::Accelerate))
		{
			targetSpeedX = -maxSuperSpeed;
			m_isRunning = true;
			m_isWalking = false;
		}
		else
		{
			// regular speed
			targetSpeedX = -maxSpeed;
			m_isWalking = true;
			m_isRunning = false;
		}
	}
	// right arrow button pressed
	else if (m_inputDevice->IsSelected(PlayerAction::MoveToTheRight))
	{
		// Speed button pressed
		if (m_inputDevice->IsSelected(PlayerAction::Accelerate))
		{
			targetSpeedX = maxSuperSpeed;
			m_isRunning = true;
			m_isWalking = false;
		}
		else
		{
			// regular speed
			targetSpeedX = maxSpeed;
			m_isWalking = true;
			m_isRunning = false;
		}
	}
	else
	{
		//zeroing of the horizontal acceleration
		targetSpeedX = 0;
		m_isWalking = false;
		m_isRunning = false;
	}


	///////down button is pressed//////////////
	if (m_inputDevice->IsSelected(PlayerAction::GetDown))
	{

		if (!m_isJumping)
		{
			m_isDown = true;
			targetSpeedX = 0;
			factorAcceleration = 0.08;
		}
	}
	else
	{
		if (!m_isJumping)
		{
			m_isDown = false;
		}

		factorAcceleration = 0.1;
	}

	////jump button pressed//////////////////////
	if (m_inputDevice->IsSelected(PlayerAction::Jump))
	{
		//if not already jumping or falling
		if (speedY == 0)
		{
			wasHeld = true;
			countJump = 0;
			m_isJumping = true;
			m_characterEvent = CharacterEvent_StartJumping;
		}

		//algo multiple jump
		if (wasHeld && countJump < 25)
		{
			speedY = -2;
			countJump++;
		}
	}
	else
	{
		wasHeld = false;
	}

	//algo acceleration
	speedX = factorAcceleration * targetSpeedX + (1.0 - factorAcceleration) * speedX;
	if (abs(speedX) < 0.01)
	{
		speedX = 0.0;
	}

	speedY += acceleY * dt;

	//m_lastSpeedX = this->speedX;
	//m_lastSpeedY = this->speedY;
}

void Player::UpdateTile(float dt, bool isGameFreezed)
{
	if (isGameFreezed)
	{
		if (m_isInjured)
		{
			this->SetCurrentTile(m_currentTile, nextInjuredTile(dt));
			this->m_isInjuredFrameCount++;
			if (m_isInjuredFrameCount > 50)
			{
				m_isInjured = false;
				m_isTemporarilyInvincible = true;
				m_isInjuredFrameCount = 0;
			}
		}
		else if (m_isDying)
		{

		}
	}
	else
	{
		if (m_isJumping && m_isDown)
		{
			this->SetCurrentTile(m_currentTile, m_isBig ? 5 : 11);
		}
		else if (m_isJumping)
		{
			this->SetCurrentTile(m_currentTile, m_isBig ? 3 : 11);
		}
		else if (m_isDown)
		{
			this->SetCurrentTile(m_currentTile, m_isBig ? 5 : 9);
		}
		else if (m_isRunning)
		{
			this->SetCurrentTile(m_currentTile, nextWalkingTile(dt, 0.05));
		}
		else if (m_isWalking)
		{
			this->SetCurrentTile(m_currentTile, nextWalkingTile(dt, 0.1));
		}
		else if (m_isJumpedOn)
		{
			this->SetCurrentTile(m_currentTile, m_isBig ? 7 : 13);
			this->m_isJumpedOnFrameCount++;
			if (m_isJumpedOnFrameCount > 15)
			{
				m_isJumpedOn = false;
				m_isJumpedOnFrameCount = 0;
			}
		}
		else if (m_isPushed)
		{
			// TODO: Just set if diretion going against speed direction?
			this->SetCurrentTile(m_currentTile, m_isBig ? 4 : 12);
			this->m_isPushedFrameCount++;
			if (m_isPushedFrameCount > 10)
			{
				m_isPushed = false;
				m_isPushedFrameCount = 0;
			}
		}
		else
		{
			this->SetCurrentTile(m_currentTile, m_isBig ? 0 : 9);
		}
	}

	// this logic should potentially be moved to another (new method)
	if (m_isTemporarilyInvincible)
	{
		// all similar logic should probably be based on the elapsed time and a time limit
		// rather than on a frame count, otherwise the logic will depend on the frame rate.
		this->m_isTemporarilyInvincibleFrameCount++;
		if (m_isTemporarilyInvincibleFrameCount > 50)
		{
			m_isTemporarilyInvincible = false;
			m_isTemporarilyInvincibleFrameCount = 0;
		}
	}
}

int Player::nextWalkingTile(float dt, float threshold)
{
	m_elapsedTimeWalkingCount += dt;
	if (m_elapsedTimeWalkingCount > threshold)
	{
		m_walkingCount++;
		if (m_isBig)
		{
			m_walkingCount %= 3;
		}
		else
		{
			m_walkingCount %= 2;
		}
		m_elapsedTimeWalkingCount = 0;
	}
	return (m_isBig ? m_walkingCount : m_walkingCount + 9);
}

int Player::nextInjuredTile(float dt)
{
	m_elapsedTimeInjuredCount += dt;
	if (m_elapsedTimeInjuredCount > InjuredTimeThreshold)
	{
		m_injuredCount++;
		m_injuredCount %= 2;
		m_elapsedTimeInjuredCount = 0;
	}

	return m_injuredCount == 0 ? 6 : 9;
}

void Player::CollisionEffect(Character* character)
{
	// temporarily untouchable after injury
	if (m_isTemporarilyInvincible)
		return;

	if (dynamic_cast<Spiny*>(character))
	{
		if (m_isBig)
		{
			m_isInjured = true;
			this->SetIsBig(false);
		}
		else
		{
			// dying animation
			this->SetIsBig(true);
			this->m_y = 0;
			this->m_x = 200;
		}
	}

	Player* player = dynamic_cast<Player*>(character);
	if (player != NULL)
	{
		// elastic collision in one dimension
		// https://www.boundless.com/physics/textbooks/624/linear-momentum-and-collisions-7/collisions-70/elastic-collisions-in-one-dimension-298-6220/
		//v1f = (v1i * (m1 - m2) + v2i * 2.0 * m2) / (m2 + m1);
		//v2f = (v1i * 2.0 * m1 + v2i * (m2 - m1) / (m2 + m1);

		this->speedX = (this->m_lastSpeedX * (this->m_mass - player->m_mass) + player->m_lastSpeedX * 2.0 * player->m_mass) / (player->m_mass + this->m_mass);
		player->speedX = (this->m_lastSpeedX * 2.0 * this->m_mass + player->m_lastSpeedX * (player->m_mass - this->m_mass)) / (player->m_mass + this->m_mass);

		// TODO : REMOVE?
		//this->speedY = (this->m_lastSpeedY * (this->m_mass - player->m_mass) + player->m_lastSpeedY * 2.0 * player->m_mass) / (player->m_mass + this->m_mass);
		//player->speedY = (this->m_lastSpeedY * 2.0 * this->m_mass + player->m_lastSpeedY * (player->m_mass - this->m_mass)) / (player->m_mass + this->m_mass);
		
		// Looking for the player above
		Player* playerAbove = NULL;
		Player* playerBelow = NULL;

		playerAbove = this->m_y <= player->m_y ? this : player;
		playerBelow = this->m_y <= player->m_y ? player : this;

		if (0.95 * (playerAbove->m_y + this->GetTileHeight())<playerBelow->m_y)
			{
				//TODO: replace by IsJumping?
				playerAbove->speedY = -2.0;
				//playerAbove->SetIsJumping(true);
				playerBelow->SetIsJumpedOn(true);
			}
		else
		{
			//TODO: just change tile when speed is against direction
			//TODO: find wich player is pushed
			//?->SetIsPushed(true);
		}
	}
}

void Player::SetIsJumpedOn(bool isJumpedOn)
{
	this->m_isJumpedOn = isJumpedOn;
	if (isJumpedOn)
	{
		this->speedX /= 2.0;
		this->m_characterEvent = CharacterEvent_GotJumpedOn;
	}	
}

void Player::SetIsPushed(bool isPushed)
{
	this->m_isPushed = true;
}

void Player::SetIsBig(bool isBig)
{
	this->m_isBig = isBig;
	this->m_characterEvent = isBig ? CharacterEvent_StartGrowing : CharacterEvent_GotInjured;
}

void Player::SetIsJumping(bool isJumping)
{
	this->m_isJumping = isJumping;
}

void Player::UpdatePosition()
{
	// Call base class method first
	Character::UpdatePosition();

	m_lastSpeedX = this->speedX;
	m_lastSpeedY = this->speedY;
}

double Player::GetSpeedX()
{
	return this->speedX;
}