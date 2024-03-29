/**
 * @file constants.h
 * @brief Constants used in project
 */

#pragma once

#include <array>
#include <string>

using namespace std;

/**
 * @brief Array of operations: '+', '-', '%', '*', '/'.
 * Used for attribute value updates.
 */
constexpr array OPERATIONS {'+', '-', '%', '*', '/'};

/**
 * @brief ID constant for health attribute.
 * Used to identify the health attribute in the game.
 */
constexpr size_t ID_HEALTH = 911;

/**
 * @brief ID constant for inventory's capacity representing as CAttribute.
 * Used to identify the inventory's capacity representing as CAttribute in the game.
 */
constexpr size_t ID_INVENTORY = 999;

/**
 * @brief Constant string view for inventory's capacity name.
 * Represents the name of the inventory's capacity.
 */
constexpr string_view NAME_INV = "capacity of inv.";

/**
 * @brief Constant string view for health attribute name.
 * Represents the name of the health attribute.
 */
constexpr string_view NAME_HEALTH = "health";

/**
 * @brief Constant string view for weapon type.
 * Represents the type identifier for weapons.
 */
constexpr string_view WEAPON = "w";

/**
 * @brief Constant string view for object type.
 * Represents the type identifier for objects.
 */
constexpr string_view OBJECT = "o";

/**
 * @brief Array of strings representing affirmative options.
 * Used for user input validation.
 */
constexpr array YES = {"y", "yes"};

/**
 * @brief Array of strings representing negative options.
 * Used for user input validation.
 */
constexpr array NO = {"n", "no"};

/**
 * @brief Array of strings representing start options.
 * Used for game menu options.
 */
constexpr array START = {"s", "start"};

/**
 * @brief Array of strings representing pause options.
 * Used for game menu options.
 */
constexpr array PAUSE = {"p", "pause"};

/**
 * @brief Array of strings representing save options.
 * Used for game menu options.
 */
constexpr array SAVE = {"sv", "save"};

/**
 * @brief Array of strings representing menu options.
 * Used for showing menu.
 */
constexpr array MENU = {"m", "menu"};

/**
 * @brief Array of strings representing continue options.
 * Used for game control options.
 */
constexpr array CONTINUE_P = {"c", "continue"};

/**
 * @brief Array of strings representing quit options.
 * Used for game control options.
 */
constexpr array QUIT = {"q", "quit"};

/**
 * @brief Constant for short sleep duration.
 * Used for delaying actions in the game.
 */
constexpr int SLEEP_SHORT = 0;

/**
 * @brief Constant for medium sleep duration.
 * Used for delaying actions in the game.
 */
constexpr int SLEEP_MID = 1;

/**
 * @brief Constant for long sleep duration.
 * Used for delaying actions in the game.
 */
constexpr int SLEEP_LONG = 1;

/**
 * @brief Constant string view for invalid input message.
 * Used for displaying error messages.
 */
constexpr string_view INVALID_INPUT = "Invalid input. Try again please:\n";

/**
 * @brief Constant string view for saved games directory.
 * Represents the directory name where saved games are stored.
 */
constexpr string_view SAVED_GAMES = "examples/saved_games";
/**
 * @brief Constant string view for config files.
 * Represents the directory name where the configuration files are stored.
 */
constexpr string_view CONFIG_PATH = "examples/config_files";
/**
 * @brief Constant string view for files for tests.
 * Represents the directory name where the files for tests are stored.
 */
constexpr string_view TEST_PATH = "examples/test_files";
/**
 * @brief Constant string view for invalid type error message.
 * Used for displaying error messages.
 */
constexpr string_view INVALID_TYPE = "Cannot do the operation. Invalid datatype.\n";

/**
 * @brief Constant string view for wrong answer message.
 * Used for displaying error messages.
 */
constexpr string_view WRONG_ANSWER = "Your answer is wrong\n";

/**
 * @brief Constant string view for 'none' placeholder.
 * Represents a placeholder for when no value is available.
 */
constexpr string_view NONE = "...none...";
/**
 * @brief Constant for the length of the cover.
 */
constexpr size_t LENGTH_COVER = 110;

















