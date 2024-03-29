/**
 * @file helperFunctions.h
 * @brief Contains various helper functions used in the application.
 */

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <iostream>
#include <array>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "constants.h"
#include "menuException.h"

using namespace std;

/**
 * @brief Checks the input if is "m" or "menu".
 * @throw MenuException is thrown when the input is equal to "m" or "menu"
 * @param input The input string to check.
 */
void check_input_for_menu(const string& input);
/**
 * @brief Validates a path string if it contains only printable characters using isprintable().
 * @param input The path string to validate.
 * @return True if the path string is valid, false otherwise.
 */
bool validate_path_string(const string& input);
/**
 * @brief Gets a string input from the user.
 * @param type String representing a call for player to type.
 * @return The string input provided by the user.
 */
string get_string_from_user(const string& type = "Type here: ");
/**
 * @brief Gets an integer input from the user.
 * @param type String representing a call for player to type.
 * @return The integer input provided by the user.
 */
int get_int_from_user(const string& type = "Type number: ");
/**
 * @brief Waits for 200ms and clears the console screen.
 */
void wait_and_clear();

/**
 * @brief Clears the console screen.
 */
void clear();

/**
 * @brief Pauses the program execution for 200ms.
 */
void sleep();

/**
 * @brief Pauses the program execution for 2000ms.
 */
void sleep_long();

/**
 * @brief Covers a string with a specific length using padding.
 * @param input The input string to cover.
 * @return The covered string with the specified length which equals to constant LENGTH_COVER.
 */
string cover_to_length(const string& input);
