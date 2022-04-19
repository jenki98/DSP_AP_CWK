#pragma once
#include <windows.h>									// Header File For The Windows Library
#include "./include/fmod_studio/fmod.hpp"
#include "./include/fmod_studio/fmod_errors.h"
#include "CircularBuffer.h"
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
	void Update(CCamera* camera, glm::vec3 ballpos);
	void AddCoeff();
	void RemoveCoeff();
	void set_speed(float speed) { m_speed = speed; }
	void ToFMODVector(glm::vec3& glVec3, FMOD_VECTOR* fmodVec);

private:

	FMOD_RESULT result;
	FMOD::System *m_FmodSystem;	// the global variable for talking to FMOD
	FMOD::Sound *m_eventSound;

	FMOD::Sound *m_music;
	FMOD::Channel *m_musicChannel;
	FMOD::Channel *m_eventChannel;
	FMOD::DSP *m_dsp;
	FMOD_VECTOR m_ballpos;
	FMOD_VECTOR camPos;
	FMOD_VECTOR posVel;
	float m_speed;

	
	

};
