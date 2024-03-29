/**
 * @file game.h
 * @brief Defines the Game class and its member functions.
 */

#pragma once
#include "database.h"
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::__fs::filesystem;

/**
 * @class Game
 * @brief Represents the game.
 *
 * The Game class handles the execution of the game. It provides member functions for
 * initializing the game, loading a new  or a previous game, running the menu, running the game,
 * saving the game, quitting the game, and more.
 */
class Game {
public:
    /**
     * @brief Default constructor for Game.
     */
    Game() = default;
    /**
     * @brief Default destructor for Game.
     */
    ~Game() = default;
    /**
     * @brief Runs the game.
     * @param config_files The path to load the game from.
     */
    void run(const string& config_files);
private:
    /**
     * @brief Loads a new game from m_config file.
     * @throw CUSTOM_EXCEPTION is thrown if any function from Database throws exception when the content is not usable for the game
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.
     */
    void load_new_game();
    /**
     * @brief Loads a previous game.
     * @param path_from The path to load the game from.
     * @throw MenuException is rethrown when MenuException is caught, because of user's input function
     * @throw CUSTOM_EXCEPTION is thrown if any function from Database throws exception when the content is not usable for the game
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.
     */
    void load_prev_game(const string& path_from);
    /**
     * @brief Shows the game menu and get string from player
     * @param menu_output Players input string.
     */
    void run_menu(const string& menu_output);
    /**
     * @brief Runs the game.
     * @throw MenuException is rethrown when MenuException is caught, because of user's input function
     * @throw CUSTOM_EXCEPTION is thrown when some exception inherited from std::exception occurs
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.
     */
    void run_game();
    /**
     * @brief Displays the game introduction.
     */
    void intro();
    /**
     * @brief Saves the game into specific json files into given directory
     * @param path The path to save the game to
     * @throw CUSTOM_EXCEPTION is thrown when some exception inherited from std::exception occurs
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.
     */
    void save_game(const string& path);
    /**
     * @brief Saves the automaton into json file
     * @throw CUSTOM_EXCEPTION is thrown when some exception inherited from std::exception occurs
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.
     */
    void save_automaton();
    /**
     * @brief Saves the user into json file
     * @throw CUSTOM_EXCEPTION is thrown when some exception inherited from std::exception occurs
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.     */
    void save_user();
    /**
     * @brief Chooses a file from given offer
     * @throw MenuException is rethrown when MenuException is caught, because of user's input function
     * @throw CUSTOM_EXCEPTION is thrown when some exception inherited from std::exception occurs
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.
     * @return The chosen file path by player.
     */
    [[nodiscard]] string choose_file() const;
    /**
     * @brief Asks the user about quiting the game after he tried to quit the game.
     * @throw MenuException is rethrown when MenuException is caught, because of there is user's input
     * @return True if the user confirms ending the game, false otherwise.
     */
    bool ask_about_end();
    /**
     * @brief Quits the game and terminate the whole program.
     */
    void quit_game();
    /**
     * @brief Builds the final statistics string.
     * @return The final statistics string.
     */
    [[nodiscard]] string build_final_stats() const;
    /**
     * @brief This function is letting player to choose an option from the menu.
     * @throw MenuException is rethrown when MenuException is caught, because of user's input function
     * @throw CUSTOM_EXCEPTION is thrown when some exception inherited from std::exception occurs
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs.
     * @return String of th chosen option.
     */
    string choose_from_menu();
    /**
     * @brief Checks if the menu choice is valid and the game can continue
     * @param menu_output The menu output string.
     * @return True if the menu choice is valid, false otherwise.
     */
    [[nodiscard]] bool check_menu_choice(const string& menu_output) const;
    /**
     * @brief The user is a character controlled by player
     */
    User m_user;
    /**
     * @brief The database of all existing items, effects, NPC and attributes in the game
     */
    Database m_database;
    /**
     * @brief The automaton representing all states and transitions in the game.
     */
    Automaton m_automaton;
    /**
     * @brief Class initializer used for initialization the players character(user)
     */
    UserInit m_initializer;
    /**
     * @brief Flag indicating if the game is running or not.
     */
    bool m_game_is_running = false;
    /**
     * @brief The path to configuration files.
     */
    string m_config_path;
    /**
     * @brief The path to file with all saved game.
     */
    string m_saved_game_path;
};
