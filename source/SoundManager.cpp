#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::InitializeResources()
{
	m_sfMusic.setPitch(1.0);
	m_sfMusic.setVolume(100);

	m_musicEffectDictionnary[MusicEffect_BattleMode] = "Resources/sounds/BattleModeThemeExtended.ogg";

	m_soundEffectDictionnary[SoundEffect_Jump] = "Resources/sounds/jump.wav";
	m_soundEffectDictionnary[SoundEffect_PowerDown] = "Resources/sounds/PowerDown.wav";
	m_soundEffectDictionnary[SoundEffect_PowerUp] = "Resources/sounds/PowerUp.wav";
	m_soundEffectDictionnary[SoundEffect_StandardCharacterCollision] = "Resources/sounds/stomp.wav";
}

void SoundManager::ReleaseResources()
{
}

void SoundManager::AddSoundBufferForEntity(long objectId)
{
	m_soundByEntityDictionnary[objectId] = new sf::Sound();
	m_soundBufferByEntityDictionnary[objectId] = new sf::SoundBuffer();
}

int SoundManager::PlaySoundEffect(long objectId, SoundEffect soundEffect)
{
	if (soundEffect == SoundEffect::SoundEffect_NoSoundEffect)
		return 0;

	if (!m_soundBufferByEntityDictionnary[objectId]->loadFromFile(m_soundEffectDictionnary[soundEffect]))
		return -1;

	m_soundByEntityDictionnary[objectId]->setBuffer(*m_soundBufferByEntityDictionnary[objectId]);
	m_soundByEntityDictionnary[objectId]->play();

	return 0;
}

int SoundManager::PlayMusic(MusicEffect musicEffect)
{
	if (!m_sfMusic.openFromFile(m_musicEffectDictionnary[musicEffect]))
		return -1; // error

	m_sfMusic.setLoop(true);
	m_sfMusic.play();
	return 0;
}
