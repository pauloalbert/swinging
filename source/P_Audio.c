#include "P_Audio.h"

void Audio_Init()
{
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_BARTMAN);
	//Load effects
	mmLoadEffect(SFX_BOOP);
	mmLoadEffect(SFX_WEBSHOOT);
}

void Audio_PlaySoundEX( int i , u8 volume, u8 panning)
{
	//Declare a sound effect
	mm_sound_effect effect;
	//Set the id of the sound effect with the input parameter
	effect.id = i;
	//Set the rate to the default one (1024)
	effect.rate = 1024;
	//Set the volume to the maximum (range 0...255)
	effect.volume = volume;
	//Set the panning depending on the effect (0-left....255-right)
	effect.panning = panning;

	//Play the effect using the sound structure
	mmEffectEx(&effect);
}


void Audio_PlayMusic()
{
	//Start playing music in a loop
		mmStart(MOD_BARTMAN,MM_PLAY_LOOP);
		//Set module volume (range 0...1024)
		mmSetModuleVolume(128);
}

void SlowTempo()
{
mmSetModuleTempo(300);
mmSetModulePitch(300);
}

void NormalTempo()
{
mmSetModuleTempo(1024);
mmSetModulePitch(1024);
}
