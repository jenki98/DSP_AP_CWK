#include "Audio.h"

#pragma comment(lib, "lib/fmod_vc.lib")

/*
I've made these two functions non-member functions
*/

// Check for error

typedef struct
{
	float volume_linear;
	float* buffer;
} mydsp_data_t;


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


FMOD_RESULT F_CALLBACK DSPCallbackDelay(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer,
	unsigned int length, int inchannels, int* outchannels)
{
	for (unsigned int samp = 0; samp < length; samp++)
	{
		for (int chan = 0; chan < *outchannels; chan++)
		{
			outbuffer[(samp * *outchannels) + chan] = 0.5 * buffer.AtPosition(samp * inchannels + chan) +
				0.5 * inbuffer[samp * inchannels + chan];
			// store incoming new sample into circular buffer
			buffer.Put(outbuffer[(samp * *outchannels) + chan]);
		}
	}
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK DSPCallbackAveragingFIR(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer,
	unsigned int length, int inchannels, int* outchannels)
{
	for (unsigned int samp = 0; samp < length; samp++)
	{
		for (int chan = 0; chan < *outchannels; chan++)
		{
			buffer.Put(inbuffer[samp * inchannels + chan]);
			outbuffer[(samp * *outchannels) + chan] =
				(inbuffer[samp * inchannels + chan] +
					buffer.AtPosition(samp - 1 * inchannels + chan) +
					buffer.AtPosition(samp - 2 * inchannels + chan) +
					buffer.AtPosition(samp - 3 * inchannels + chan)) / 4.f;
		}
	}
	return FMOD_OK;
}

// DSP callback
//FMOD_RESULT F_CALLBACK DSPCallback(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels)
//{
//	FMOD::DSP* thisdsp = (FMOD::DSP*)dsp_state->instance;
//
//	for (unsigned int samp = 0; samp < length; samp++)
//	{
//		for (int chan = 0; chan < *outchannels; chan++)
//		{
//			/*
//			This DSP filter just halves the volume!
//			Input is modified, and sent to output.
//			*/
//			outbuffer[(samp * *outchannels) + chan] = inbuffer[(samp * inchannels) + chan] * 0.2f;
//		}
//	}
//
//	return FMOD_OK;
//}


FMOD_RESULT F_CALLBACK myDSPCallback(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer,
	unsigned int length, int inchannels, int* outchannels)
{
	mydsp_data_t* data = (mydsp_data_t*)dsp_state->plugindata;

	for (unsigned int samp = 0; samp < length; samp++)
	{
		for (int chan = 0; chan < *outchannels; chan++)
		{
			data->buffer[(samp * *outchannels) + chan] =
				inbuffer[(samp * inchannels) + chan] * data->volume_linear;
			outbuffer[(samp * inchannels) + chan] = data->buffer[(samp * *outchannels) + chan];
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
		dspdesc.read = DSPCallbackAveragingFIR;

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

	return true;
	

}

// Play a music stream
bool CAudio::PlayMusicStream()
{
	result = m_FmodSystem->playSound(m_music, NULL, false, &m_musicChannel);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	m_musicChannel->addDSP(0, m_dsp);

	return true;
}

void CAudio::Update()
{
	m_FmodSystem->update();
}