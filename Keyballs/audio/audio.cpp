#include "../main/includes.h"

LPDIRECTSOUND8 directSoundDevice;
SoundObject* soundObjectArray;

ARESULT InitSound(HWND hWnd)
{
	if FAILED(DirectSoundCreate8(NULL, &directSoundDevice, NULL))
		return AUDIOINIT_FAIL;
	if FAILED(directSoundDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY))
		return AUDIOINIT_FAIL;
	return AUDIO_SUCCESS;
}

//loadFileIntoDefaultBuffer(L"../media/sample.wav", dsbSample, directSoundDevice)
//Constructor
SoundObject::SoundObject(LPWSTR lpfilename){
	soundBuffer = NULL;
	fadeDown = false;
	fadeUp = false;
	playing = false;
	volume = DSBVOLUME_MAX;
	loadFile(lpfilename);
	this->soundBuffer->SetVolume(volume);
}

ARESULT SoundObject::loadFile(LPWSTR filename){
	// Open wave file
	CWaveFile* waveFile = new CWaveFile();
	waveFile->Open(filename, NULL, WAVEFILE_READ);
	if(waveFile->GetSize() == 0){return BUFFSETUP_FILE_OPEN_FAIL;}

	//TODO Add code to set up buffer description
	
	// Set up Wave format description
	WAVEFORMATEX wfx = getDefaultWaveFormat();
	
	// Set up Buffer description based on the file
	DSBUFFERDESC dsbd;
	if(getBufferDescForFilesize(waveFile->GetSize(), &dsbd, &wfx) != AUDIO_SUCCESS){return BUFFSETUP_BUFFERDESC_FAIL;}

	// Create buffer from decription
	if FAILED(directSoundDevice->CreateSoundBuffer(&dsbd, &soundBuffer, NULL)){return BUFFSETUP_CREATEBUFF_FAIL;}

	// Lock the buffer
	VOID* buffer = NULL;
	DWORD bufferSize = 0;
	if FAILED(soundBuffer->Lock(0,waveFile->GetSize(),&buffer,&bufferSize,NULL,NULL,DSBLOCK_ENTIREBUFFER)){return BUFFSETUP_LOCKBUFF_FAIL;}

	// Reset file to begining
	if FAILED(waveFile->ResetFile()){return BUFFSETUP_RESETFILE_FAIL;}

	// Read file into buffer
	DWORD bytesRead = 0;
	if FAILED(waveFile->Read((BYTE*) buffer, bufferSize, &bytesRead)){return BUFFSETUP_READFILE_FAIL;}
	
	// Unlock the buffer
	if(soundBuffer->Unlock(buffer,bufferSize,NULL,NULL)){return BUFFSETUP_UNLOCKBUFF_FAIL;}

	delete waveFile;

	return AUDIO_SUCCESS;
}

ARESULT loadFileIntoDefaultBuffer(LPWSTR filename, LPDIRECTSOUNDBUFFER &directSoundBuffer, LPDIRECTSOUND8 &directSoundDevice)
{
	//TODO return codes

	// Open wave file
	CWaveFile *waveFile = new CWaveFile();
	waveFile->Open(filename, NULL, WAVEFILE_READ);
	if(waveFile->GetSize() == 0){return BUFFSETUP_FILE_OPEN_FAIL;}

	//TODO Add code to set up buffer description
	
	// Set up Wave format description
	WAVEFORMATEX wfx = getDefaultWaveFormat();
	
	// Set up Buffer description based on the file
	DSBUFFERDESC dsbd;
	if(getBufferDescForFilesize(waveFile->GetSize(), &dsbd, &wfx) != AUDIO_SUCCESS){return BUFFSETUP_BUFFERDESC_FAIL;}

	// Create buffer from decription
	if FAILED(directSoundDevice->CreateSoundBuffer(&dsbd, &directSoundBuffer, NULL)){return BUFFSETUP_CREATEBUFF_FAIL;}

	// Lock the buffer
	VOID* buffer = NULL;
	DWORD bufferSize = 0;
	if FAILED(directSoundBuffer->Lock(0,waveFile->GetSize(),&buffer,&bufferSize,NULL,NULL,DSBLOCK_ENTIREBUFFER)){return BUFFSETUP_LOCKBUFF_FAIL;}

	// Reset file to begining
	if FAILED(waveFile->ResetFile()){return BUFFSETUP_RESETFILE_FAIL;}

	// Read file into buffer
	DWORD bytesRead = 0;
	if FAILED(waveFile->Read((BYTE*) buffer, bufferSize, &bytesRead)){return BUFFSETUP_READFILE_FAIL;}
	
	// Unlock the buffer
	if(directSoundBuffer->Unlock(buffer,bufferSize,NULL,NULL)){return BUFFSETUP_UNLOCKBUFF_FAIL;}

	delete waveFile;

	return AUDIO_SUCCESS;
}

void SoundObject::kill(){
	this->soundBuffer->Release();
}

void SoundObject::playLooping(){
	if(!playing){
		soundBuffer->Play(0,0,DSBPLAY_LOOPING);
		playing = true;
	}
}

void SoundObject::stop(){
	if(playing){
		soundBuffer->Stop();
		playing = false;
	}
}

void SoundObject::playOnce(){
	soundBuffer->Play(0,0,0);
}

void SoundObject::process(){
	// fadeDown to -10000
	if(this->fadeDown)
	{
		this->fadeUp = false;
		if(this->volume == DSBVOLUME_MIN)
			this->fadeDown = false;
		else
			this->volume--;
	}
	// fadeUp to 0
	if(this->fadeUp)
	{
		this->fadeDown = false;
		if(this->volume == DSBVOLUME_MAX)
			this->fadeUp = false;
		else
			this->volume++;
	}
	this->soundBuffer->SetVolume(this->volume);
}

void SoundObject::fadeOut(){
	this->fadeDown = true;
	this->fadeUp = false;
}

void SoundObject::fadeIn(){
	this->fadeUp = true;
	this->fadeDown = false;
}

void SoundObject::reset(){
	this->soundBuffer->Stop();
	this->soundBuffer->SetCurrentPosition(0);
	this->playing = false;
}

WAVEFORMATEX getDefaultWaveFormat(){
	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = (WORD) WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
	wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);
	return wfx;
}

ARESULT getBufferDescForFilesize(DWORD filesize, DSBUFFERDESC* dsbd, WAVEFORMATEX* wfx){
	ZeroMemory(dsbd, sizeof(DSBUFFERDESC));
	dsbd->dwSize = sizeof(DSBUFFERDESC);
	dsbd->dwFlags = DSBCAPS_CTRLVOLUME;
	dsbd->dwBufferBytes = filesize;
	dsbd->guid3DAlgorithm = GUID_NULL;
	dsbd->lpwfxFormat = wfx;
	return AUDIO_SUCCESS;
}

void RenderSound(){

}