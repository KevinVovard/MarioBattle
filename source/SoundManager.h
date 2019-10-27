#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
enum SoundEffect
{
	SoundEffect_NoSoundEffect,
	SoundEffect_Jump,
	SoundEffect_PowerDown,
	SoundEffect_PowerUp,
	SoundEffect_StandardCharacterCollision
};

enum MusicEffect { MusicEffect_BattleMode };

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void InitializeResources();
	void ReleaseResources();
	void AddSoundBufferForEntity(long objectId);
	int PlaySoundEffect(long objectId, SoundEffect soundEffect);
	int PlayMusic(MusicEffect musicEffect);

private:
	sf::Music m_sfMusic;
	std::unordered_map<long, sf::Sound*> m_soundByEntityDictionnary;
	std::unordered_map<long, sf::SoundBuffer*> m_soundBufferByEntityDictionnary;
	std::unordered_map<SoundEffect, std::string> m_soundEffectDictionnary;
	std::unordered_map<MusicEffect, std::string> m_musicEffectDictionnary;
};

