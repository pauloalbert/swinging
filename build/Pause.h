
//{{BLOCK(Pause)

//======================================================================
//
//	Pause, 256x192@4, 
//	Transparent color : FF,FF,FF
//	+ palette 256 entries, not compressed
//	+ 172 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 5504 + 1536 = 7552
//
//	Time-stamp: 2024-01-08, 17:13:07
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PAUSE_H
#define GRIT_PAUSE_H

#define PauseTilesLen 5504
extern const unsigned int PauseTiles[1376];

#define PauseMapLen 1536
extern const unsigned short PauseMap[768];

#define PausePalLen 512
extern const unsigned short PausePal[256];

#endif // GRIT_PAUSE_H

//}}BLOCK(Pause)
