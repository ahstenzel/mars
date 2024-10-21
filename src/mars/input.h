#ifndef MARS_INPUT_H
#define MARS_INPUT_H
/**
 * input.h
 * Input related functions & settings.
 */
#include "mars/common.h"

#define MAX_INPUT_SLOTS      4			// Maximum number of input slots
#define MAX_GAMEPADS         4			// Maximum number of gamepads supported
#define MAX_GAMEPAD_AXIS     8			// Maximum number of axis supported (per gamepad)
#define MAX_GAMEPAD_BUTTONS 32			// Maximum number of buttons supported (per gamepad)
#define INPUT_MASK_UP    0b00000001
#define INPUT_MASK_LEFT  0b00000010
#define INPUT_MASK_DOWN  0b00000100
#define INPUT_MASK_RIGHT 0b00001000
#define GAMEPAD_AXIS_SECTOR 0.573576f


//----------------------------------------------------------------------------------
// Enums
//----------------------------------------------------------------------------------

/// @brief Keyboard keys (US keyboard layout)
typedef enum {
	KEY_NULL            = 0,		// Key: NULL, used for no key pressed
	// Alphanumeric keys
	KEY_APOSTROPHE      = 39,		// Key: '
	KEY_COMMA           = 44,		// Key: ,
	KEY_MINUS           = 45,		// Key: -
	KEY_PERIOD          = 46,		// Key: .
	KEY_SLASH           = 47,		// Key: /
	KEY_ZERO            = 48,		// Key: 0
	KEY_ONE             = 49,		// Key: 1
	KEY_TWO             = 50,		// Key: 2
	KEY_THREE           = 51,		// Key: 3
	KEY_FOUR            = 52,		// Key: 4
	KEY_FIVE            = 53,		// Key: 5
	KEY_SIX             = 54,		// Key: 6
	KEY_SEVEN           = 55,		// Key: 7
	KEY_EIGHT           = 56,		// Key: 8
	KEY_NINE            = 57,		// Key: 9
	KEY_SEMICOLON       = 59,		// Key: ;
	KEY_EQUAL           = 61,		// Key: =
	KEY_A               = 65,		// Key: A | a
	KEY_B               = 66,		// Key: B | b
	KEY_C               = 67,		// Key: C | c
	KEY_D               = 68,		// Key: D | d
	KEY_E               = 69,		// Key: E | e
	KEY_F               = 70,		// Key: F | f
	KEY_G               = 71,		// Key: G | g
	KEY_H               = 72,		// Key: H | h
	KEY_I               = 73,		// Key: I | i
	KEY_J               = 74,		// Key: J | j
	KEY_K               = 75,		// Key: K | k
	KEY_L               = 76,		// Key: L | l
	KEY_M               = 77,		// Key: M | m
	KEY_N               = 78,		// Key: N | n
	KEY_O               = 79,		// Key: O | o
	KEY_P               = 80,		// Key: P | p
	KEY_Q               = 81,		// Key: Q | q
	KEY_R               = 82,		// Key: R | r
	KEY_S               = 83,		// Key: S | s
	KEY_T               = 84,		// Key: T | t
	KEY_U               = 85,		// Key: U | u
	KEY_V               = 86,		// Key: V | v
	KEY_W               = 87,		// Key: W | w
	KEY_X               = 88,		// Key: X | x
	KEY_Y               = 89,		// Key: Y | y
	KEY_Z               = 90,		// Key: Z | z
	KEY_LEFT_BRACKET    = 91,		// Key: [
	KEY_BACKSLASH       = 92,		// Key: '\'
	KEY_RIGHT_BRACKET   = 93,		// Key: ]
	KEY_BACKTICK        = 96,		// Key: `
	// Function keys
	KEY_SPACE           = 32,		// Key: Space
	KEY_ESCAPE          = 256,		// Key: Esc
	KEY_ENTER           = 257,		// Key: Enter
	KEY_TAB             = 258,		// Key: Tab
	KEY_BACKSPACE       = 259,		// Key: Backspace
	KEY_INSERT          = 260,		// Key: Ins
	KEY_DELETE          = 261,		// Key: Del
	KEY_RIGHT           = 262,		// Key: Cursor right
	KEY_LEFT            = 263,		// Key: Cursor left
	KEY_DOWN            = 264,		// Key: Cursor down
	KEY_UP              = 265,		// Key: Cursor up
	KEY_PAGE_UP         = 266,		// Key: Page up
	KEY_PAGE_DOWN       = 267,		// Key: Page down
	KEY_HOME            = 268,		// Key: Home
	KEY_END             = 269,		// Key: End
	KEY_CAPS_LOCK       = 280,		// Key: Caps lock
	KEY_SCROLL_LOCK     = 281,		// Key: Scroll down
	KEY_NUM_LOCK        = 282,		// Key: Num lock
	KEY_PRINT_SCREEN    = 283,		// Key: Print screen
	KEY_PAUSE           = 284,		// Key: Pause
	KEY_F1              = 290,		// Key: F1
	KEY_F2              = 291,		// Key: F2
	KEY_F3              = 292,		// Key: F3
	KEY_F4              = 293,		// Key: F4
	KEY_F5              = 294,		// Key: F5
	KEY_F6              = 295,		// Key: F6
	KEY_F7              = 296,		// Key: F7
	KEY_F8              = 297,		// Key: F8
	KEY_F9              = 298,		// Key: F9
	KEY_F10             = 299,		// Key: F10
	KEY_F11             = 300,		// Key: F11
	KEY_F12             = 301,		// Key: F12
	KEY_LEFT_SHIFT      = 340,		// Key: Shift left
	KEY_LEFT_CONTROL    = 341,		// Key: Control left
	KEY_LEFT_ALT        = 342,		// Key: Alt left
	KEY_LEFT_SUPER      = 343,		// Key: Super left
	KEY_RIGHT_SHIFT     = 344,		// Key: Shift right
	KEY_RIGHT_CONTROL   = 345,		// Key: Control right
	KEY_RIGHT_ALT       = 346,		// Key: Alt right
	KEY_RIGHT_SUPER     = 347,		// Key: Super right
	KEY_KB_MENU         = 348,		// Key: KB menu
	// Keypad keys
	KEY_KP_0            = 320,		// Key: Keypad 0
	KEY_KP_1            = 321,		// Key: Keypad 1
	KEY_KP_2            = 322,		// Key: Keypad 2
	KEY_KP_3            = 323,		// Key: Keypad 3
	KEY_KP_4            = 324,		// Key: Keypad 4
	KEY_KP_5            = 325,		// Key: Keypad 5
	KEY_KP_6            = 326,		// Key: Keypad 6
	KEY_KP_7            = 327,		// Key: Keypad 7
	KEY_KP_8            = 328,		// Key: Keypad 8
	KEY_KP_9            = 329,		// Key: Keypad 9
	KEY_KP_DECIMAL      = 330,		// Key: Keypad .
	KEY_KP_DIVIDE       = 331,		// Key: Keypad /
	KEY_KP_MULTIPLY     = 332,		// Key: Keypad *
	KEY_KP_SUBTRACT     = 333,		// Key: Keypad -
	KEY_KP_ADD          = 334,		// Key: Keypad +
	KEY_KP_ENTER        = 335,		// Key: Keypad Enter
	KEY_KP_EQUAL        = 336,		// Key: Keypad =
	// Android key buttons
	KEY_BACK            = 4,		// Key: Android back button
	KEY_MENU            = 5,		// Key: Android menu button
	KEY_VOLUME_UP       = 24,		// Key: Android volume up button
	KEY_VOLUME_DOWN     = 25,		// Key: Android volume down button
	// Control
	KEY_MAX             = 337		// Maximum enum value
} KeyboardKey;

/// @brief Mouse buttons
typedef enum {
	MOUSE_BUTTON_LEFT = KEY_MAX + 1,		// Mouse button left
	MOUSE_BUTTON_RIGHT,						// Mouse button right
	MOUSE_BUTTON_MIDDLE,					// Mouse button middle (pressed wheel)
	MOUSE_BUTTON_SIDE,						// Mouse button side (advanced mouse device)
	MOUSE_BUTTON_EXTRA,						// Mouse button extra (advanced mouse device)
	MOUSE_BUTTON_FORWARD,					// Mouse button forward (advanced mouse device)
	MOUSE_BUTTON_BACK,						// Mouse button back (advanced mouse device)
	// Control
	MOUSE_MAX								// Maximum enum value
} MouseButton;

/// @brief Mouse cursor
typedef enum {
	MOUSE_CURSOR_DEFAULT = 0,			// Default pointer shape
	MOUSE_CURSOR_ARROW,					// Arrow shape
	MOUSE_CURSOR_IBEAM,					// Text writing cursor shape
	MOUSE_CURSOR_CROSSHAIR,				// Cross shape
	MOUSE_CURSOR_POINTING_HAND,			// Pointing hand cursor
	MOUSE_CURSOR_RESIZE_EW,				// Horizontal resize/move arrow shape
	MOUSE_CURSOR_RESIZE_NS,				// Vertical resize/move arrow shape
	MOUSE_CURSOR_RESIZE_NWSE,			// Top-left to bottom-right diagonal resize/move arrow shape
	MOUSE_CURSOR_RESIZE_NESW,			// The top-right to bottom-left diagonal resize/move arrow shape
	MOUSE_CURSOR_RESIZE_ALL,			// The omnidirectional resize/move cursor shape
	MOUSE_CURSOR_NOT_ALLOWED,			// The operation-not-allowed shape
	// Control
	MOUSE_CURSOR_MAX					// Maximum enum value
} MouseCursor;

/// @brief Gamepad buttons
typedef enum {
	GAMEPAD_BUTTON_UNKNOWN = 0,			// Unknown button, just for error checking
	GAMEPAD_BUTTON_LEFT_FACE_UP,		// Gamepad left DPAD up button
	GAMEPAD_BUTTON_LEFT_FACE_RIGHT,		// Gamepad left DPAD right button
	GAMEPAD_BUTTON_LEFT_FACE_DOWN,		// Gamepad left DPAD down button
	GAMEPAD_BUTTON_LEFT_FACE_LEFT,		// Gamepad left DPAD left button
	GAMEPAD_BUTTON_RIGHT_FACE_UP,		// Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
	GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,	// Gamepad right button right (i.e. PS3: Circle, Xbox: B)
	GAMEPAD_BUTTON_RIGHT_FACE_DOWN,		// Gamepad right button down (i.e. PS3: Cross, Xbox: A)
	GAMEPAD_BUTTON_RIGHT_FACE_LEFT,		// Gamepad right button left (i.e. PS3: Square, Xbox: X)
	GAMEPAD_BUTTON_LEFT_SHOULDER_1,		// Gamepad top/back shoulder left (first), it could be a trailing button
	GAMEPAD_BUTTON_LEFT_SHOULDER_2,		// Gamepad top/back shoulder left (second), it could be a trailing button
	GAMEPAD_BUTTON_RIGHT_SHOULDER_1,	// Gamepad top/back shoulder right (first), it could be a trailing button
	GAMEPAD_BUTTON_RIGHT_SHOULDER_2,	// Gamepad top/back shoulder right (second), it could be a trailing button
	GAMEPAD_BUTTON_MIDDLE_LEFT,			// Gamepad center buttons, left one (i.e. PS3: Select)
	GAMEPAD_BUTTON_MIDDLE,				// Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
	GAMEPAD_BUTTON_MIDDLE_RIGHT,		// Gamepad center buttons, right one (i.e. PS3: Start)
	GAMEPAD_BUTTON_LEFT_THUMB,			// Gamepad joystick pressed button left
	GAMEPAD_BUTTON_RIGHT_THUMB,			// Gamepad joystick pressed button right
	// Control
	GAMEPAD_BUTTON_MAX					// Maximum enum value
} GamepadButton;

/// @brief Gamepad axis
typedef enum {
	GAMEPAD_AXIS_LEFT_STICK = GAMEPAD_BUTTON_MAX + 1,		// Gamepad left stick
	GAMEPAD_AXIS_RIGHT_STICK,								// Gamepad right stick
	GAMEPAD_AXIS_LEFT_X,									// Gamepad left stick X axis
	GAMEPAD_AXIS_LEFT_Y,									// Gamepad left stick Y axis
	GAMEPAD_AXIS_RIGHT_X,									// Gamepad right stick X axis
	GAMEPAD_AXIS_RIGHT_Y,									// Gamepad right stick Y axis
	GAMEPAD_AXIS_LEFT_TRIGGER,								// Gamepad back trigger left, pressure level: [1..-1]
	GAMEPAD_AXIS_RIGHT_TRIGGER,								// Gamepad back trigger right, pressure level: [1..-1]
	// Control
	GAMEPAD_AXIS_MAX										// Maximum enum value
} GamepadAxis;

/// @brief Device to use for input.
typedef enum {
	INPUT_MODE_NONE = 0,
	INPUT_MODE_PENDING,
	INPUT_MODE_KEYBOARD,
	INPUT_MODE_GAMEPAD
} InputMode;


//----------------------------------------------------------------------------------
// Input settings
//----------------------------------------------------------------------------------

/// @brief Source of player input to poll.
typedef struct {
	int* _keyboardMapping;
	int* _gamepadMapping;
	size_t _numInputs;
	float _gamepadAxisLatch[MAX_GAMEPAD_AXIS];
	float _gamepadDeadzone;
	int _gamepadIndex;
	InputMode _mode;
} InputSlot;

/// @brief Descriptor for creating an input slot.
typedef struct {
	int* keyboardMapping;
	int* gamepadMapping;
	size_t numInputs;
	float gamepadDeadzone;
	int gamepadIndex;
	InputMode mode;
} InputSlotDesc;

/// @brief List of all input remapping & polling settings.
typedef struct {
	InputSlot _inputs[MAX_INPUT_SLOTS];
	size_t _num;
} InputSettingsList;


//----------------------------------------------------------------------------------
// Keyboard reading
//----------------------------------------------------------------------------------

/// @brief Check if a key is pressed.
/// @param _key Keycode
/// @return True if pressed this frame
MARS_API bool IsKeyPressed(KeyboardKey _key);

/// @brief Check if a key is held down.
/// @param _key Keycode
/// @return True if pressed
MARS_API bool IsKeyDown(KeyboardKey _key);

/// @brief Check if a key is released.
/// @param _key Keycode
/// @return True if released this frame
MARS_API bool IsKeyReleased(KeyboardKey _key);

/// @brief Get last pressed keycode. Can be polled continuously for multiple queued keypressed.
/// @return Keycode (or KEY_NULL when queue is empty)
MARS_API KeyboardKey GetKeyPressed();

/// @brief Get last pressed character. Can be polled continuously for multiple queued keypressed.
/// @return Character (or 0 when queue is empty)
MARS_API char GetCharPressed();


//----------------------------------------------------------------------------------
// Gamepad reading
//----------------------------------------------------------------------------------

/// @brief Check if a gamepad is available.
/// @param _gamepad Gamepad index
/// @return True if available
MARS_API bool IsGamepadAvailable(int _gamepad);

/// @brief Get the gamepads internal name.
/// @param _gamepad Gamepad index
/// @return Name string
MARS_API const char *GetGamepadName(int _gamepad);

/// @brief Check if a gamepad button is pressed.
/// @param _gamepad Gamepad index
/// @param _button Gamepad button
/// @return True if pressed this frame
MARS_API bool IsGamepadButtonPressed(int _gamepad, GamepadButton _button);

/// @brief Check if a gamepad button is held down.
/// @param _gamepad Gamepad index
/// @param _button Gamepad button
/// @return True if pressed
MARS_API bool IsGamepadButtonDown(int _gamepad, GamepadButton _button);

/// @brief Check if a gamepad button is released.
/// @param _gamepad Gamepad index
/// @param _button Gamepad button
/// @return True if released this frame
MARS_API bool IsGamepadButtonReleased(int _gamepad, GamepadButton _button);

/// @brief Get last pressed gamepad button.
/// @return Gamepad button
MARS_API GamepadButton GetGamepadButtonPressed();

/// @brief Get the number of axes on the gamepad.
/// @param _gamepad Gamepad index
/// @return Axis number
MARS_API int GetGamepadAxisCount(int _gamepad);

/// @brief Get the value of the axis for the gamepad.
/// @param _gamepad Gamepad index
/// @param _axis Axis number
/// @return Axis value
MARS_API float GetGamepadAxisValue(int _gamepad, GamepadAxis _axis);

/// @brief Get the value of the 2 axes per stick for the gamepad.
/// @param _gamepad Gamepad index
/// @param _axis Axis number
/// @return Normalized vector
MARS_API Vector2 GetGamepadStickValue(int _gamepad, GamepadAxis _axis);


//----------------------------------------------------------------------------------
// Mouse reading
//----------------------------------------------------------------------------------

/// @brief Check if a mouse button is pressed.
/// @param _button Mouse button
/// @return True if pressed this frame
MARS_API bool IsMouseButtonPressed(MouseButton _button);

/// @brief Check if a mouse button is held down.
/// @param _button Mouse button
/// @return True if pressed
MARS_API bool IsMouseButtonDown(MouseButton _button);

/// @brief Check if a mouse button is released.
/// @param _button Mouse button
/// @return True if released this frame
MARS_API bool IsMouseButtonReleased(MouseButton _button);

/// @brief Get mouse X position.
/// @return X position (screen space)
MARS_API int GetMouseX();

/// @brief Get mouse Y position.
/// @return Y position (screen space)
MARS_API int GetMouseY();

/// @brief Get mouse position.
/// @return Vector position (screen space)
MARS_API Vector2i GetMousePosition(); // Get mouse position XY

/// @brief Get the amount the mouse moved last frame.
/// @return Vector (screen space)
MARS_API Vector2i GetMouseDelta();

/// @brief Set the position of the mouse cursor.
/// @param _x X position (screen space)
/// @param _y Y position (screen space)
MARS_API void SetMousePosition(int _x, int _y); // Set mouse position XY

/// @brief Get the amount the mouse wheel moved last frame.
/// @return Vector
MARS_API Vector2 GetMouseWheelMoveV();

/// @brief Set mouse cursor type.
/// @param _cursor Cursor enum
MARS_API void SetMouseCursor(MouseCursor _cursor); // Set mouse cursor


//----------------------------------------------------------------------------------
// Input remapping
//----------------------------------------------------------------------------------

/// @brief Describe what inputs to read when determining movement direction.
typedef struct {
	int inputs[4];
} InputDirections;

/// @brief Read all the inputs for movement and create a 2D vector showing the overall direction of movement.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _aligned If using a gamepad stick, snap the input to 8-directional if true
/// @param _directions Direction descriptor
/// @return Normalized vector
MARS_API Vector2 GetInputDirection(InputSettingsList* _list, unsigned int _slot, bool _aligned, InputDirections _directions);

/// @brief Read all the inputs for movement and create a bitfield showing the overall direction of movement.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _directions Direction descriptor
/// @return Bitfield
MARS_API unsigned char GetInputDirectionBitfield(InputSettingsList* _list, unsigned int _slot, InputDirections _directions);

/// @brief Check if the given input is pressed for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _input Input index (0 for none, -1 for any)
/// @return True if pressed on this frame
MARS_API bool GetInputPressed(InputSettingsList* _list, unsigned int _slot, int _input);

/// @brief Check if the given input is held for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _input Input index (0 for none, -1 for any)
/// @return True if held on this frame
MARS_API bool GetInputDown(InputSettingsList* _list, unsigned int _slot, int _input);

/// @brief Check if the given input is released for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _input Input index (0 for none, -1 for any)
/// @return True if released on this frame
MARS_API bool GetInputReleased(InputSettingsList* _list, unsigned int _slot, int _input);

/// @brief Get the keyboard mapping for the input flag in the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _input Input index
/// @return Keyboard scancode
MARS_API int GetKeyboardMapping(InputSettingsList* _list, unsigned int _slot, int _input);

/// @brief Set the keyboard mapping for the input flag in the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _input Input index
/// @param _mapping Keyboard scancode
MARS_API void SetKeyboardMapping(InputSettingsList* _list, unsigned int _slot, int _input, int _mapping);

/// @brief Get the gamepad mapping for the input flag in the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _input Input index
/// @return Gamepad scancode
MARS_API int GetGamepadMapping(InputSettingsList* _list, unsigned int _slot, int _input);

/// @brief Set the gamepad mapping for the input flag in the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _input Input index
/// @param _mapping Gamepad scancode
MARS_API void SetGamepadMapping(InputSettingsList* _list, unsigned int _slot, int _input, int _mapping);

/// @brief Get the axis deadzone for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @return Gamepad deadzone
MARS_API float GetGamepadDeadzone(InputSettingsList* _list, unsigned int _slot);

/// @brief Set the axis deadzone for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _deadzone Gamepad deadzone
MARS_API void SetGamepadDeadzone(InputSettingsList* _list, unsigned int _slot, float _deadzone);

/// @brief Get the input mode for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @return Input mode enum
MARS_API InputMode GetInputMode(InputSettingsList* _list, unsigned int _slot);

/// @brief Set the input mode for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _mode Input mode enum
MARS_API void SetInputMode(InputSettingsList* _list, unsigned int _slot, InputMode _mode);

/// @brief Set the gamepad index for the given input slot.
/// @param _list Input list pointer
/// @param _slot Input slot
/// @param _gamepad Gamepad index
MARS_API void SetGamepadIndex(InputSettingsList* _list, unsigned int _slot, int _gamepad);

#endif // MARS_INPUT_H