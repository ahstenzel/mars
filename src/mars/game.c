#include "mars/game.h"

Game* _mars_g_game = NULL;

void CreateGame(const char* _name) {
	MARS_RETURN_CLEAR;
	
	// Allocate game state
	MARS_DEBUG_LOG("===Creating game state===");
	MARS_GAME = MARS_CALLOC(1, sizeof(*MARS_GAME));
	if (!MARS_GAME) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate game state!");
		goto create_game_failed;
	}

	// Load settings
	MARS_DEBUG_LOG("===Loading settings===");
	MARS_SETTINGS = GenerateDefaultSettings();

	// Initialize renderer
	MARS_DEBUG_LOG("===Creating display===");
	DisplayDesc displayDesc = {
		.name = _name
	};
	MARS_DISPLAY = _CreateDisplay(displayDesc);
	if (!MARS_DISPLAY) {
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
		_DestroyDisplay(MARS_DISPLAY);
		MARS_FREE(MARS_GAME);
		MARS_GAME = NULL;
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