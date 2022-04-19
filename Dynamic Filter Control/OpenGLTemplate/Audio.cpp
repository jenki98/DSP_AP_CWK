#include "Audio.h"
#include <vector>

#pragma comment(lib, "lib/fmod_vc.lib")

/*
I've made these two functions non-member functions
*/

// Check for error
std::vector <float> coeff = { 1.5f, 1.5f, 1.5f, 1.5f };
float speed;

//typedef struct
//{
//	float *buffer;
//	float volume_linear;
//	int   length_samples;
//	int   channels;
//	
//} mydsp_data_t;



void FmodErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		const char *errorString = FMOD_ErrorString(result);
		// MessageBox(NULL, errorString, "FMOD Error", MB_OK);
		// Warning: error message commented out -- if headphones not plugged into computer in lab, error occurs
	}
}
int numChannels = 2;

CCircularBuffer buffer(4096 * numChannels);




FMOD_RESULT F_CALLBACK DSPCallbackFIR(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer,
	unsigned int length, int inchannels, int* outchannels)
{

	//mydata->coeff.size()
	for (unsigned int samp = 0; samp < length; samp++)
	{
		for (int chan = 0; chan < *outchannels; chan++)
		{
			buffer.Put(inbuffer[samp * inchannels + chan]);
			
			float ans = 0;
			for (int i = 0; i < coeff.size(); i++) {

				ans += buffer.AtPosition(samp-i * inchannels + chan) * coeff[i] * speed;
			}
			

			outbuffer[(samp * *outchannels) + chan] = ans;
		}
	}
	return FMOD_OK;
}




// DSP callback
FMOD_RESULT F_CALLBACK DSPCallback(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels)
{
	FMOD::DSP* thisdsp = (FMOD::DSP*)dsp_state->instance;

	for (unsigned int samp = 0; samp < length; samp++)
	{
		for (int chan = 0; chan < *outchannels; chan++)
		{
			/*
			This DSP filter just halves the volume!
			Input is modified, and sent to output.
			*/
			outbuffer[(samp * *outchannels) + chan] = inbuffer[(samp * inchannels) + chan] * 0.2f;
		}
	}

	return FMOD_OK;
}





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



	// Create the DSP effect
	{
		FMOD_DSP_DESCRIPTION dspdesc;
		memset(&dspdesc, 0, sizeof(dspdesc));

		strncpy_s(dspdesc.name, "My first DSP unit", sizeof(dspdesc.name));
		dspdesc.numinputbuffers = 4;
		dspdesc.numoutputbuffers = 4;
		dspdesc.read = DSPCallbackFIR;

		result = m_FmodSystem->createDSP(&dspdesc, &m_dsp);
		FmodErrorCheck(result);

		if (result != FMOD_OK)
			return false;
	}
	

	return true;
	
}

// Load an event sound
bool CAudio::LoadEventSound(char *filename)
{
	result = m_FmodSystem->createSound(filename, FMOD_LOOP_NORMAL, 0, &m_eventSound);
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
	m_eventChannel->setMode(FMOD_3D);
	result = m_eventChannel->set3DAttributes(&m_ballpos, 0 ,0);
	result = m_FmodSystem->set3DSettings(0.f, 1.f, 1.0f);
	m_eventChannel->addDSP(0, m_dsp);

	return true;
}


// Load a music stream
bool CAudio::LoadMusicStream(char *filename)
{
	result = m_FmodSystem->createStream(filename, NULL | FMOD_LOOP_NORMAL, 0, &m_music);
	FmodErrorCheck(result);

	if (result != FMOD_OK) 
		return false;

	return true;
	

}

// Play a music stream
bool CAudio::PlayMusicStream()
{
	result = m_FmodSystem->playSound(m_music, NULL, false, &m_musicChannel);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	

	return true;
}

void CAudio::Update(CCamera* camera, glm::vec3 ballpos)
{
	ToFMODVector(ballpos, &m_ballpos);
	ToFMODVector(camera->GetPosition(), &camPos);
	result = m_FmodSystem->set3DListenerAttributes(0, &camPos, NULL, NULL, NULL);
	result = m_eventChannel->set3DAttributes(&m_ballpos, 0, 0);
	speed = m_speed;
	m_FmodSystem->update();
	
}



void CAudio::AddCoeff()
{
	coeff.push_back(1.5f);
	

}
void CAudio::RemoveCoeff() {
	coeff.pop_back();
}



void CAudio::ToFMODVector(glm::vec3& glVec3, FMOD_VECTOR* fmodVec)
{
	fmodVec->x = glVec3.x;
	fmodVec->y = glVec3.y;
	fmodVec->z = glVec3.z;
}