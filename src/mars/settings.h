#ifndef MARS_SETTINGS_H
#define MARS_SETTINGS_H
/**
 * settings.h
 * Top level game settings management.
 */
#include "mars/common.h"
#include "mars/display.h"
#include "mars/input.h"

/// @brief Structure containing all game settings
typedef struct {
	InputSettingsList* _inputSettingsList;		// List of all input settings
	DisplaySettingsList* _displaySettingsList;	// List of all video settings
} SettingsList;

/// @brief Populate the given settings structures with values from an INI file.
/// @param _settingsFile Settings file name
/// @param _targetSettingsList Pointer to destination settings list structure
MARS_API int LoadSettingsFile(char* _settingsFile, SettingsList* _targetSettingsList);

/// @brief Create a settings structure populated with default values.
/// @return Pointer to setting structure
MARS_API SettingsList* GenerateDefaultSettings();

/// @brief Deallocate the settings structure.
void _DestroySettings(SettingsList* _settingsList);

#endif // MARS_SETTINGS_H