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
	result = m_FmodSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_CHANNEL_LOWPASS | FMOD_INIT_3D_RIGHTHANDED, 0);
	FmodErrorCheck(result);
	if (result != FMOD_OK) 
		return false;

	// Set 3D settings
	result = m_FmodSystem->set3DSettings(1.f, 1.f, 0.1f);
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;
	// Set Occlusion settings
	result = m_FmodSystem->setGeometrySettings(1000); //resize world to occlusion
	if (result != FMOD_OK)
		return false;


	return true;
}

void CAudio::SetupGeometry(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 vertices[]) {
	//position a object(for now)
	ToFMODVector(pos, &objectPos); //position objectgeometry
	ToFMODVector(forward, &objectForward); 
	ToFMODVector(up, &objectUp);

	FMOD_VECTOR wall[8];
	for (unsigned int i = 0; i < 8; i++) {
		ToFMODVector(vertices[i], &wall[i]);
	}
	
	result = m_FmodSystem->createGeometry(1, 8, &object_geometry); //create geometry to occlusion
	object_geometry->setPosition(&objectPos); //using to position object geometry
	object_geometry->setRotation(&objectForward, & objectUp); //using to rotation object geometry

	int index = 0; //index to occlusion 
	result = object_geometry->addPolygon(0.9, 1, true, 8, wall, &index); // add polygon to object geometry 

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
	result = m_FmodSystem->playSound(m_eventSound, NULL, false, &m_eventChannel);
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;
	// play through 3D channel
	m_eventChannel->setMode(FMOD_3D);

	// set the position to be the horse's position
	FMOD_VECTOR pos_sound = { 0, 0, 0 };
	FMOD_VECTOR vel_sound = { 0, 0, 0 };
	result = m_eventChannel->set3DAttributes(&pos_sound, &vel_sound);
	//sound position
	
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

	// Set the volume lower
	result = m_musicChannel->setVolume(m_musicVolume);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	// connecting the music filter to the music stream
	// Get the DSP head and it's input
	m_musicChannel->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &m_musicDSPHead);
	m_musicDSPHead->getInput(0, &m_musicDSPHeadInput, NULL);
	// Disconnect them
	m_musicDSPHead->disconnectFrom(m_musicDSPHeadInput);
	// Add input to the music head from the filter
	result = m_musicDSPHead->addInput(m_musicFilter);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	// Add input to the filter head music DSP head input
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

void CAudio::Update(CCamera *camera)
{

	FMOD_VECTOR vel_listener = { 0, 0, 0 };
	FMOD_VECTOR forward_listener = { 0, 0, -1 };
	FMOD_VECTOR up_listener;
	ToFMODVector(camera->GetUpVector(), &up_listener);
	//ToFMODVector(camera->, &up_listener);
	// 5) update the listener's position with the camera position
	ToFMODVector(camera->GetPosition(), &camPos);
	//result = m_FmodSystem->set3DListenerAttributes(0, &camPos, NULL, NULL, NULL);
	result = m_FmodSystem->set3DListenerAttributes(0, &camPos, &vel_listener, &forward_listener, &up_listener);
	FmodErrorCheck(result);
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

void CAudio::IncreaseMusicVolume()
{
	// called externally from Game::ProcessEvents
	// increment the volume
	m_musicVolume += 0.05f;
	if (m_musicVolume > 1)
		m_musicVolume = 1.0f;
	m_musicChannel->setVolume(m_musicVolume);
}

void CAudio::DecreaseMusicVolume()
{
	// called externally from Game::ProcessEvents
	// deccrement the volume
	m_musicVolume -= 0.05f;
	if (m_musicVolume < 0)
		m_musicVolume = 0.0f;
	m_musicChannel->setVolume(m_musicVolume);
}

void CAudio::ToFMODVector(glm::vec3 &glVec3, FMOD_VECTOR *fmodVec)
{
	fmodVec->x = glVec3.x;
	fmodVec->y = glVec3.y;
	fmodVec->z = glVec3.z;
}