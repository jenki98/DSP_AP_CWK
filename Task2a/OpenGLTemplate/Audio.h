#pragma once
#include <windows.h>									// Header File For The Windows Library
#include "./include/fmod_studio/fmod.hpp"
#include "./include/fmod_studio/fmod_errors.h"
#include "./include/glm/gtc/type_ptr.hpp"
#include "Camera.h"


class CAudio
{
public:
	CAudio();
	~CAudio();
	bool Initialise();
	bool LoadEventSound(char *filename);
	bool PlayEventSound();
	bool LoadMusicStream(char *filename);
	bool PlayMusicStream();
	void ToggleMusicFilter();
	void UpdateMusicPosition();
	void Update(CCamera* cam , glm::vec3 ballpos);
	void set_doppler(float doppler) { m_doppler = doppler; }
	void set_speed(float speed) { m_speed = speed; }


private:
		
	void FmodErrorCheck(FMOD_RESULT result);
	void ToFMODVector(glm::vec3& glVec3, FMOD_VECTOR* fmodVec);


	FMOD_RESULT result;
	FMOD::System *m_FmodSystem;	// the global variable for talking to FMOD
	FMOD::Sound *m_eventSound;
	FMOD::Channel* m_eventChannel;
	
	FMOD::Sound *m_music;
	FMOD::DSP *m_musicFilter;
	bool m_musicFilterActive;
	FMOD::Channel* m_musicChannel;
	FMOD::DSP *m_musicDSPHead;
	FMOD::DSP *m_musicDSPHeadInput;
	float m_musicVolume = 0.2f;
	FMOD_VECTOR m_ballpos;
	FMOD_VECTOR camPos;
	FMOD_VECTOR posVel;
	float m_doppler;
	float m_speed;
	glm::vec3 vel = glm::vec3(4.f);
};
