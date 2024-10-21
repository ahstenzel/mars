#include "mars/game.h"

Game* _mars_g_game = NULL;

void CreateGame(const char* _name) {
	MARS_RETURN_CLEAR;
	
	// Allocate game state
	MARS_DEBUG_LOG("===Creating game state===");
	Game* game = MARS_CALLOC(1, sizeof(*game));
	if (!game) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate game state!");
		goto create_game_failed;
	}
	MARS_GAME = game;

	// Load settings
	MARS_DEBUG_LOG("===Loading settings===");
	game->_settingsList = GenerateDefaultSettings();

	// Initialize renderer
	MARS_DEBUG_LOG("===Creating display===");
	DisplayDesc displayDesc = {
		.name = _name
	};
	game->_display = _CreateDisplay(displayDesc);
	if (!game->_display) {
		MARS_ABORT(MARS_ERROR_STATUS_GENERIC, "Failed to initialize display!");
		goto create_game_failed;
	}

	return;
create_game_failed:
	DestroyGame();
	return;
}

void DestroyGame() {
	if (MARS_GAME) {
		MARS_DEBUG_LOG("===Destroying game state===");
		MARS_DEBUG_LOG("Destroying settings");
		_DestroySettings(MARS_SETTINGS);
		MARS_DEBUG_LOG("Destroying display");
		_DestroyDisplay(MARS_GAME->_display);
		MARS_FREE(MARS_GAME);
	}
}

void UpdateGame() {
	if (MARS_GAME) {
		MARS_DEBUG_LOG("===Entering update loop===");
		while(!_DisplayShouldClose(MARS_DISPLAY)) {
			// Update inputs

			// Update game state

			// Update display
			_UpdateDisplay(MARS_DISPLAY);
		}
	}
}