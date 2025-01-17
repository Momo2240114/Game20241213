#include "AudioManager.h"

#include "System/Audio.h"

void AudioManager::LoadFile()
{
	std::vector<std::string> fileNames_BGM = {
		"Data/Sound/Title.wav",
		"Data/Sound/Title.wav",
		"Data/Sound/Game.wav",
	};

	for (const auto& fileName : fileNames_BGM) {
		AudioSource* Load = Audio::Instance().LoadAudioSource(fileName.c_str());
		if (!Load) {continue;}
		AddBGM(Load);
	}	
	std::vector<std::string> fileNames_SE = {
		"Data/Sound/GoNextScene.wav",
		"Data/Sound/NotPutBlock.wav",
		"Data/Sound/SwipePage.wav",
	};

	for (const auto& fileName : fileNames_SE) {
		AudioSource* Load = Audio::Instance().LoadAudioSource(fileName.c_str());
		if (!Load) {continue;}
		AddSE(Load);
	}
}

void AudioManager::AddBGM(AudioSource* source)
{
	if (std::find(audio_BGMs.begin(), audio_BGMs.end(), source) == audio_BGMs.end()) {
		audio_BGMs.push_back(source);
	}
}

void AudioManager::AddSE(AudioSource* source)
{
	if (std::find(audio_SEs.begin(), audio_SEs.end(), source) == audio_SEs.end()) {
		audio_SEs.push_back(source);
	}
}

void AudioManager::PlayBGM(size_t index) const
{
	if (index >= audio_BGMs.size()) {
		return;
	}
	audio_BGMs[index]->Play(true);
}

void AudioManager::SetVolumeBGM(size_t index,float Volume) const
{
	if (index >= audio_BGMs.size()) {
		return;
	}
	audio_BGMs[index]->SetVolume(Volume);
}

void AudioManager::PlaySE(size_t index) const
{
	if (index >= audio_SEs.size()) {
		return;
	}
	audio_SEs[index]->Play(false);
}

void AudioManager::StopBGM()
{
	for (auto& audioBGM : audio_BGMs) {
		if (audioBGM) {
			audioBGM->Stop(); // AudioSource の Stop メソッドを呼び出す
		}
	}
}

void AudioManager::StopSE()
{
	for (auto& audioSE : audio_SEs) {
		if (audioSE) {
			audioSE->Stop(); // AudioSource の Stop メソッドを呼び出す
		}
	}
}

void AudioManager::RemoveBGM(size_t index)
{
	if (index < audio_BGMs.size()) {
		audio_BGMs.erase(audio_BGMs.begin() + index);
	}
}

void AudioManager::ClearAll()
{
	// すべてのオーディオを停止
	StopBGM();

	audio_BGMs.clear();
	audio_SEs.clear();

}
