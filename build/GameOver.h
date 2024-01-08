
//{{BLOCK(GameOver)

//======================================================================
//
//	GameOver, 256x192@4, 
//	Transparent color : FF,FF,FF
//	+ palette 256 entries, not compressed
//	+ 203 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 6496 + 1536 = 8544
//
//	Time-stamp: 2024-01-08, 17:13:07
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAMEOVER_H
#define GRIT_GAMEOVER_H

#define GameOverTilesLen 6496
extern const unsigned int GameOverTiles[1624];

#define GameOverMapLen 1536
extern const unsigned short GameOverMap[768];

#define GameOverPalLen 512
extern const unsigned short GameOverPal[256];

#endif // GRIT_GAMEOVER_H

//}}BLOCK(GameOver)
