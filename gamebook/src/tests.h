#pragma once
#include "game.h"
#include "fight.h"
#include <iostream>
#include <sstream>
#include <regex>


/**
 * @brief This function loads testing version of user stored in directory test_files in file test_user.json
 * @return User for tests
 */
User load_test_user();
/**
 * @brief This function generate random character
 * @return One random character
 */
char generate_random_char();
/**
 * @brief This function checks if the exception contains Unexpected EOF or not.
 */
 bool is_eof_exception(const string& ex);
//do not need input
/**
 * @brief This function test using object on User with temporary effect
 * This function uses a object defined in database as well as in users inventory. It checks it the attributes after usage are different, because the temporary objects should temporarily change the value of attributes, but the should not change the value of saved attributes.
 * @return True it he attributes are different, False otherwise.
 */
bool use_object_w_temp_effect();

/**
 * @brief This function test using object on  User with permanent effect
 * This function is checking if the usage of object with permanent effect effects attribute as well as the saved attributes and if both of them are equal
 * @return True if attributes before are different that after as well as the saved attributes
 */
bool use_object_w_perm_effect();
/**
 * @brief Testing if deleting object from inventory works on users testing instance
 * @return True if the object is no longer in users inventory
 */
bool test_deleting_object_from_inventory();
/**
 * @brief Testing if the weapon swapping works on users testing instance
 * @return True if the current testing instance have the new weapon from its inventory
 */
bool test_swapping_weapon();
/**
 * @brief Testing if the armour swapping works on users testing instance
 * @return True if the current testing instance have the new armour from its inventory
 */
bool test_swapping_armour();
/**
 * @brief Testing is the path validation works
 * @return True if all problematic cases were validate as false
 */
bool test_validate_file_path();

/**
 * @brief Test if the configuration file exists and it is possible for the game to be loaded from it
 * @return True if no error or exception shows
 */
bool test_loading_from_config_file();
/**
 * @brief Test if the non existing file does not exist and it is not possible for the game to be loaded from it
 * @return True when error or exeption occurs
 */
bool test_loading_from_non_existing_file();
bool test_loading_prev_game_from_existing_file();
bool use_object_on_enemy();
bool test_menu_exception();
bool test_loading_database(bool expected, const string & path);