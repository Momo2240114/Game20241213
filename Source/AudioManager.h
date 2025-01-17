#pragma once

#include "System/Audio.h"
#include <string>

class AudioManager
{
public:
	static AudioManager& Instance() {
		static AudioManager ins;
		return ins;
	}

	void LoadFile();

	void AddBGM(AudioSource* source);
	void AddSE(AudioSource* source);

	void PlayBGM(size_t index) const;
	void SetVolumeBGM(size_t index, float Volume) const;
	void PlaySE(size_t index) const;
	void StopBGM();
	void StopSE();

	void RemoveBGM(size_t index);

	void ClearAll();

private:
	~AudioManager() {};
	AudioManager() {};
	AudioManager(const AudioManager* i) {};
	AudioManager operator= (const AudioManager* i) {};

	std::vector<AudioSource*> audio_BGMs; // AudioSourceのBGM管理用ベクター
	std::vector<AudioSource*> audio_SEs; // AudioSourceのSE管理用ベクター
};

enum BGM_Number {
	Title = 0,
	Selet = 1,
	Game = 2,
	Over = 3,
	Clear = 4
};

enum SE_Number {
	GoTitleToSelect = 0,
	DonotPutBlock = 1,
	Swipe = 2,
};