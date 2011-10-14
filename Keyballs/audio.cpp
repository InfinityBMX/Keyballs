#include "includes.h"

int loadFileIntoDefaultBuffer( LPWSTR filename, LPDIRECTSOUNDBUFFER &directSoundBuffer, LPDIRECTSOUND8 &directSoundDevice )
{
	//TODO return codes
	//TODO Add code to set up wave format
	HRESULT hr = NULL;
	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));

	wfx.wFormatTag = (WORD) WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
	wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

	//TODO Add code to set up buffer description
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = 0;
	dsbd.dwBufferBytes = 1330000;
	dsbd.guid3DAlgorithm = GUID_NULL;
	dsbd.lpwfxFormat = &wfx;

	// Create buffer from decription
	hr = directSoundDevice->CreateSoundBuffer(&dsbd, &directSoundBuffer, NULL);
	if FAILED(hr){return false;}

	//Set up wave file
	//TODO Switch code to dynamically create buffer based on filesize
	CWaveFile *waveFile = new CWaveFile();
	waveFile->Open(filename, NULL, WAVEFILE_READ);
	if(waveFile->GetSize() == 0){return false;}

	VOID* buffer = NULL;
	DWORD bufferSize = 0;
	hr = directSoundBuffer->Lock(0,waveFile->GetSize(),&buffer,&bufferSize,NULL,NULL,DSBLOCK_ENTIREBUFFER);
	if FAILED(hr){return false;}

	DWORD bytesRead = 0;
	waveFile->ResetFile();
	hr = waveFile->Read((BYTE*) buffer, bufferSize, &bytesRead);
	if FAILED(hr){return false;}

	hr = directSoundBuffer->Unlock(&buffer,bufferSize,NULL,NULL);
	if FAILED(hr){return false;}

	return true;
}