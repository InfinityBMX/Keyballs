#include "../main/includes.h"

LPDIRECTSOUND8 directSoundDevice;

ARESULT InitSound(HWND hWnd)
{
	if FAILED(DirectSoundCreate8(NULL, &directSoundDevice, NULL))
		return AUDIOINIT_FAIL;
	if FAILED(directSoundDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY))
		return AUDIOINIT_FAIL;
	return AUDIO_SUCCESS;
}

ARESULT loadFileIntoDefaultBuffer( LPWSTR filename, LPDIRECTSOUNDBUFFER &directSoundBuffer, LPDIRECTSOUND8 &directSoundDevice )
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

	return AUDIO_SUCCESS;
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
	dsbd->dwFlags = 0;
	dsbd->dwBufferBytes = filesize;
	dsbd->guid3DAlgorithm = GUID_NULL;
	dsbd->lpwfxFormat = wfx;
	return AUDIO_SUCCESS;
}