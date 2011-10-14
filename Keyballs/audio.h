#ifndef AUDIO_H
#define AUDIO_H

#define BUFFSETUP_SUCCESS 0
#define BUFFSETUP_FILE_OPEN_FAIL 1

int loadFileIntoDefaultBuffer( LPWSTR filename, LPDIRECTSOUNDBUFFER &directSoundBuffer, LPDIRECTSOUND8 &directSoundDevice );
WAVEFORMATEX getDefaultWaveFormat();
void getBufferDescForFilesize(DWORD filesize, DSBUFFERDESC &dbsc);
#endif