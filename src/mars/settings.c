#include "mars/settings.h"

int LoadSettingsFile(char* _settingsFile, SettingsList* _targetSettingsList) {
	return 0;
}

SettingsList* GenerateDefaultSettings() {
	MARS_RETURN_CLEAR;
	SettingsList* settingsList = NULL;
	DisplaySettingsList* displaySettingsList = NULL;

	settingsList = MARS_CALLOC(1, sizeof(*settingsList));
	if (!settingsList) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate settings buffer!");
		goto generate_default_settings_fail;
	}

	// Populate input settings
	settingsList->_inputSettingsList = NULL;

	// Populate display settings
	displaySettingsList = MARS_CALLOC(1, sizeof(*displaySettingsList));
	if (!displaySettingsList) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate settings buffer!");
		goto generate_default_settings_fail;
	}
	displaySettingsList->_fullscreen = false;
	displaySettingsList->_vsync = true;
	displaySettingsList->_width = 640;
	displaySettingsList->_height = 480;
	displaySettingsList->_rendererBackend = MARS_RENDERER_BACKEND_DEFAULT;
	settingsList->_displaySettingsList = displaySettingsList;

	return settingsList;

generate_default_settings_fail:
	_DestroySettings(settingsList);
	return NULL;
}

void _DestroySettings(SettingsList* _settingsList) {
	if (_settingsList) {
		MARS_FREE(_settingsList->_inputSettingsList);
		MARS_FREE(_settingsList->_displaySettingsList);
		MARS_FREE(_settingsList);
	}
}