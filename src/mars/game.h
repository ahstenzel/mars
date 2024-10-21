#ifndef MARS_GAME_H
#define MARS_GAME_H
/**
 * game.h
 * Top level container for game state.
*/
#include "mars/common.h"
#include "mars/settings.h"
#include "mars/display.h"

typedef struct {
	SettingsList* _settingsList;
	Display* _display;
} Game;

extern Game* _mars_g_game;

/// @brief Create the game instance.
/// @param _name Window title bar
MARS_API void CreateGame(const char* _name);

/// @brief Deallocate the game instance.
MARS_API void DestroyGame();

/// @brief Update the game instance.
MARS_API void UpdateGame();

#define MARS_GAME _mars_g_game
#define MARS_SETTINGS _mars_g_game->_settingsList
#define MARS_DISPLAY _mars_g_game->_display
#define MARS_WINDOW _mars_g_game->_display->_window

#endif // MARS_GAME_H