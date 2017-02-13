#include "Player.h"
#include "Spiny.h"

Player::Player(InputDevice* inputDevice)
{
	m_inputDevice = inputDevice;
}

Player::~Player(void)
{
}

void Player::ProcessInput(float dt)
{
	//bouton de deplacement vers la gauche enfoncé
	if (m_inputDevice->IsSelected(PlayerAction::MoveToTheLeft))
	{
		//bouton d'acceleration enfoncé
		if (m_inputDevice->IsSelected(PlayerAction::Accelerate))
		{
			targetSpeedX = -maxSuperSpeed;
			m_isRunning = true;
			m_isWalking = false;
		}
		else
		{
			//vitesse normal
			targetSpeedX = -maxSpeed;
			m_isWalking = true;
			m_isRunning = false;
		}
	}
	//bouton de déplacement vers la droite enfoncé
	else if (m_inputDevice->IsSelected(PlayerAction::MoveToTheRight))
	{
		//bouton d'accélération enfoncé
		if (m_inputDevice->IsSelected(PlayerAction::Accelerate))
		{
			targetSpeedX = maxSuperSpeed;
			m_isRunning = true;
			m_isWalking = false;
		}
		else
		{
			//vitesse normal
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

void Player::UpdateTile(float dt)
{
	if (m_isJumping && m_isDown)
	{
		m_currentTile = 5;
	}
	else if (m_isJumping) 
	{ 
		m_currentTile = 3; 
	}
	else if (m_isDown)
	{
		m_currentTile = 5;
	}
	else if (m_isRunning)
	{
		m_currentTile = nextWalkingTile(dt, 0.05);
	}
	else if (m_isWalking)
	{
		m_currentTile = nextWalkingTile(dt, 0.1);
	}
	else if (m_isJumpedOn)
	{
		m_currentTile = 7;
		this->m_isJumpedOnFrameCount++;
		if (m_isJumpedOnFrameCount > 15)
		{
			m_isJumpedOn = false;
			m_isJumpedOnFrameCount = 0;
		}
	}
	else if (m_isPushed)
	{
		m_currentTile = 4;
		this->m_isPushedFrameCount++;
		if (m_isPushedFrameCount > 10)
		{
			m_isPushed = false;
			m_isPushedFrameCount = 0;
		}
	}
	else
	{
		m_currentTile = 0;
	}
}

void Player::CollisionEffect(Character* character)
{
	Spiny* spiny = dynamic_cast<Spiny*>(character);
	if (spiny != NULL)
	{
		this->m_y = 0;
		this->m_x = 200;
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

		//this->speedY = (this->m_lastSpeedY * (this->m_mass - player->m_mass) + player->m_lastSpeedY * 2.0 * player->m_mass) / (player->m_mass + this->m_mass);
		//player->speedY = (this->m_lastSpeedY * 2.0 * this->m_mass + player->m_lastSpeedY * (player->m_mass - this->m_mass)) / (player->m_mass + this->m_mass);
		
		// Looking for the player above
		Player* playerAbove = NULL;
		Player* playerBelow = NULL;

		playerAbove = this->m_y <= player->m_y ? this : player;
		playerBelow = this->m_y <= player->m_y ? player : this;

		if (0.95 * (playerAbove->m_y + m_heightTile)<playerBelow->m_y)
			{
				//TODO: replace by IsJumping?
				playerAbove->speedY = -2.0;
				//playerAbove->SetIsJumping(true);
				playerBelow->SetIsJumpedOn(true);
			}
		else
		{
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
	}	
}

void Player::SetIsPushed(bool isPushed)
{
	this->m_isPushed = true;
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