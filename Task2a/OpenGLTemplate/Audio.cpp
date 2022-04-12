#include "Audio.h"

#pragma comment(lib, "lib/fmod_vc.lib")

CAudio::CAudio()
{}

CAudio::~CAudio()
{}

bool CAudio::Initialise()
{
	// Create an FMOD system
	result = FMOD::System_Create(&m_FmodSystem);
	FmodErrorCheck(result);
	if (result != FMOD_OK) 
		return false;

	// Initialise the system
	result = m_FmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	FmodErrorCheck(result);
	if (result != FMOD_OK) 
		return false;

	return true;
	
}

// Load an event sound
bool CAudio::LoadEventSound(char *filename)
{
	result = m_FmodSystem->createSound(filename, FMOD_LOOP_OFF, 0, &m_eventSound);
	FmodErrorCheck(result);
	if (result != FMOD_OK) 
		return false;

	return true;
}

// Play an event sound
bool CAudio::PlayEventSound()
{
	result = m_FmodSystem->playSound(m_eventSound, NULL, false, NULL);
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;
	return true;
}


// Load a music stream
bool CAudio::LoadMusicStream(char *filename)
{
	result = m_FmodSystem->createStream(filename, NULL | FMOD_LOOP_NORMAL, 0, &m_music);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	// create a low-pass filter DSP object
	result = m_FmodSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &m_musicFilter);

	if (result != FMOD_OK)
		return false;

	// you can start the DSP in an inactive state
	m_musicFilter->setActive(false);

	return true;
	

}

// Play a music stream
bool CAudio::PlayMusicStream()
{
	result = m_FmodSystem->playSound(m_music, NULL, false, &m_musicChannel);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	// connecting the music filter to the music stream
	// 1) Get the DSP head and it's input
	m_musicChannel->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &m_musicDSPHead);
	m_musicDSPHead->getInput(0, &m_musicDSPHeadInput, NULL);
	// 2) Disconnect them
	m_musicDSPHead->disconnectFrom(m_musicDSPHeadInput);
	// 3) Add input to the music head from the filter
	result = m_musicDSPHead->addInput(m_musicFilter);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	// 4) Add input to the filter head music DSP head input
	result = m_musicFilter->addInput(m_musicDSPHeadInput);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	// set the DSP object to be active
	m_musicFilter->setActive(true);
	// initially set the cutoff to a high value
	m_musicFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000);
	// this state is used for toggling
	m_musicFilterActive = false;

	return true;
}

// Check for error
void CAudio::FmodErrorCheck(FMOD_RESULT result)
{						
	if (result != FMOD_OK) {
		const char *errorString = FMOD_ErrorString(result);
		// MessageBox(NULL, errorString, "FMOD Error", MB_OK);
		// Warning: error message commented out -- if headphones not plugged into computer in lab, error occurs
	}
}

void CAudio::Update()
{
	m_FmodSystem->update();
}

void CAudio::ToggleMusicFilter()
{
	// called externally from Game::ProcessEvents
	// toggle the effect on/off
	m_musicFilterActive = !m_musicFilterActive;
	if (m_musicFilterActive) {
		// set the parameter to a low value
		m_musicFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 700);
	}
	else {
		// set the parameter to a high value
		// you could also use m_musicFilter->setBypass(true) instead...
		m_musicFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000);
	}
}
