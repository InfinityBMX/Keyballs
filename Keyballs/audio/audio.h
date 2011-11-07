#ifndef _AUDIO_H
#define _AUDIO_H

// Audio Return Codes
typedef short ARESULT;
#define AUDIO_SUCCESS				(ARESULT)0
#define BUFFSETUP_FILE_OPEN_FAIL	(ARESULT)1
#define BUFFSETUP_BUFFERDESC_FAIL	(ARESULT)2
#define BUFFSETUP_CREATEBUFF_FAIL	(ARESULT)3
#define BUFFSETUP_LOCKBUFF_FAIL		(ARESULT)4
#define BUFFSETUP_RESETFILE_FAIL	(ARESULT)5
#define BUFFSETUP_READFILE_FAIL		(ARESULT)6
#define BUFFSETUP_UNLOCKBUFF_FAIL	(ARESULT)7
#define AUDIOINIT_FAIL				(ARESULT)8

//SoundObject Class
class SoundObject
{
public:
	//Methods
	SoundObject();
	SoundObject(LPWSTR lpfilename);
	void kill();
	void play();
	void playOnce();
	void stop();

private:
	//Data Members
	LPDIRECTSOUNDBUFFER soundBuffer;
	//string filename;
	int volume;
	bool fadeDown;
	bool fadeUp;
	bool playing;

	//Private Methods
//	ARESULT loadFileIntoBuffer( LPWSTR filename, LPDIRECTSOUNDBUFFER &directSoundBuffer, LPDIRECTSOUND8 &directSoundDevice );
	ARESULT loadFile(LPWSTR lpfilename);
};

// Function prototypes
ARESULT InitSound(HWND hWnd);
//Move to class private
ARESULT loadFileIntoDefaultBuffer( LPWSTR filename, LPDIRECTSOUNDBUFFER &directSoundBuffer, LPDIRECTSOUND8 &directSoundDevice );
WAVEFORMATEX getDefaultWaveFormat();
ARESULT getBufferDescForFilesize(DWORD filesize, DSBUFFERDESC* dsbd, WAVEFORMATEX* wfx);

// Audio Variables
extern LPDIRECTSOUND8 directSoundDevice;
#endif