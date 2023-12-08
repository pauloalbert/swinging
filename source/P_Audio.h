#pragma once

#include <nds.h>

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

void Audio_Init();
void Audio_PlaySoundEX( int i , u8 volume, u8 panning);
void Audio_PlayMusic();
