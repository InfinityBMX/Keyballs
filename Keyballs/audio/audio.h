#ifndef _AUDIO_H
#define _AUDIO_H
// Includes
#include <dsound.h>
#include <string>
//#include "../Include/DXUT.h" // might need
#include "SDKwavefile.h"
#include "../graphics/printableObject.h"

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

#define FADE_SPEED					(long)1

//SoundObject Class
class SoundObject
{
public:
	//Methods
	SoundObject();
	SoundObject(LPWSTR lpfilename);
	~SoundObject();
	PrintableObject getVolumeText();
	PrintableObject getMuteText();
	PrintableObject	getPrintText();
	LPWSTR getTextOnly();
	void kill();
	void playLooping();
	void playOnce();
	void stop();
	void process();
	void fadeIn();
	void fadeOut();
	void reset();
	void mute();
	void unmute();
	void updateText();
private:
	//Data Members
	LPDIRECTSOUNDBUFFER soundBuffer;
	PrintableObject* volumeText;
	PrintableObject* muteText;
	PrintableObject* printText;
	//string filename;
	long volume;
	bool muted;
	bool fadeDown;
	bool fadeUp;
	bool playing;
	long lastVolume;

	//Private Methods
	ARESULT loadFile(LPWSTR lpfilename);
};

// Function prototypes
ARESULT InitSound(HWND hWnd);
//Move to class private
ARESULT loadFileIntoDefaultBuffer( LPWSTR filename, LPDIRECTSOUNDBUFFER &directSoundBuffer, LPDIRECTSOUND8 &directSoundDevice );
WAVEFORMATEX getDefaultWaveFormat();
ARESULT getBufferDescForFilesize(DWORD filesize, DSBUFFERDESC* dsbd, WAVEFORMATEX* wfx);
void RenderSound();

// Audio Variables
extern LPDIRECTSOUND8 directSoundDevice;
#endif