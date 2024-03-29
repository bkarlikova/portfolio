/**
 * @file database.h
 *
 */
#pragma once
#include "usersCharacter.h"
#include "nonPlayingChar.h"
#include "object.h"
#include "weapon.h"
#include "armour.h"
#include "permanentEffect.h"
#include "temporaryEffect.h"
#include "automaton.h"
#include "event.h"
#include "fight.h"
#include "customExceptions.h"
#include "userInitializer.h"
#include "constants.h"
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <iostream>
#include <array>
#include <iterator>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;
using json = nlohmann::json;

/**
 * @class Database
 * @brief Class for handling database operations and parsing from JSON files.
 *
 * The Database class provides functionality for handling database operations
 * and parsing data from JSON files into relevant objects.
 */
class Database{
public:
    /**
     * @brief Default constructor for Database.
     */
    Database() = default;

    /**
     * @brief Constructor for Database that initializes the database with the provided files.
     * @param file_attributes The file path for the attributes JSON file.
     * @param file_effects The file path for the effects JSON file.
     * @param file_items The file path for the items JSON file.
     * @param file_NPCs The file path for the NPCs JSON file.
     */
    Database(const string &file_attributes,
              const string &file_effects,
              const string &file_items,
              const string &file_NPCs);

    /**
     * @brief Default destructor for Database.
     */
    ~Database() = default;
    /**
     * @brief Static function to get the parse automaton from a JSON file.
     * @throw CUSTOM_EXCEPTION - when the parsing failed
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the automaton JSON file.
     * @return JSON object representing the automaton.
     */
    static json get_automaton(const string & file_name);
    /**
     * @brief Load and return an automaton object from the specified JSON file.
     * @throw CUSTOM_EXCEPTION - when the data in JSON are not valid or usable for the game
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the automaton JSON file.
     * @return The loaded automaton object.
     */
    Automaton load_automaton(const string & file_name);
    /**
     * @brief Load and return a user initializer object from the specified JSON file.
     * @throw CUSTOM_EXCEPTION - when the data in JSON are not valid or usable for the game
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the user characters JSON file.
     * @return The loaded user initializer object.
     */
    UserInit load_user_initializer(const string & file_name) const;
    /**
     * @brief Load and return a user object from the specified JSON file.
     * @throw CUSTOM_EXCEPTION - when the data in JSON are not valid or usable for the game
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the user  JSON file.
     * @return The loaded user object.
     */
    User load_user(const string & file_name) const;
private:
    /**
     * @brief This function returns name of the attribute base on its id.
     * @param att_id The id of the attribute.
     * @return The name of the attribute.
     */
    string get_att_name(int att_id) const;
    /**
     * @brief Get a non-playing character (NPC) object based on its index.
     * @param idx The index of the NPC.
     * @return The non-playing character object.
     */
    shared_ptr<NonPlayingCharacter> get_npc(int idx) const;
    /**
     * @brief Get an item object based on its index.
     * @param idx The index of the item.
     * @return The item object.
     */
    shared_ptr<Item> get_item(int idx) const;
    /**
     * @brief This function creates a vector of items based on their indices.
     * @param idx The indices of the items.
     * @return A vector of item objects.
     */
    vector<shared_ptr<Item>> get_items(const vector<int> &idx) const;
    /**
     * @brief Create a vector of weapon objects based on their indices.
     * @param idx The indices of the weapons.
     * @return A vector of weapon objects.
     */
    vector<shared_ptr<Item>> get_weapons(const vector<int> &idx) const;
    /**
     * @brief Create a vector of armour objects based on their indices.
     * @param idx The indices of the armours.
     * @return A vector of armour objects.
     */
    vector<shared_ptr<Item>> get_armours(const vector<int> &idx) const;
    /**
     * @brief Get an effect object based on its id.
     * @param id The id of the effect.
     * @return The effect.
     */
    shared_ptr<Effect> get_effect(int id) const;
    /**
     * @brief Creates a vector of effect objects associated with a character based on their indices.
     * @param idx The indices of the effects.
     * @return A vector of effect objects associated with a character.
     */
    vector<shared_ptr<Effect>> get_effects_for_char(const vector<int> &idx) const;
    /**
     * @brief Load and return a JSON object from the specified file.
     * @throw CUSTOM_EXCEPTION - exception while parsing in JSON or invalid path
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the JSON file.
     * @return The loaded JSON object.
     */
    static json load_json(const string &file_name);
    /**
     * @brief Load and return a map of attributes from the specified JSON file.
     * @throw CUSTOM_EXCEPTION - when the data in JSON are invalid(missing required keys or values) or unusable for the game
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the attributes JSON file.
     * @return A map of attributes.
     */
    map<size_t, shared_ptr<Attribute>> load_attributes(const string &file_name) const;
    /**
     * @brief Load and return a map of effects from the specified JSON file.
     * @throw CUSTOM_EXCEPTION - when the data in JSON are invalid(missing required keys or values) or unusable for the game
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the effects JSON file.
     * @return A map of effects.
     */
    map<size_t, shared_ptr<Effect>> load_effects(const string &file_name) const;
    /**
     * @brief Load and return a map of items from the specified JSON file.
     * @throw CUSTOM_EXCEPTION - when the data in JSON are invalid(missing required keys or values) or unusable for the game
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the items JSON file.
     * @return A map of items.
     */
    map<size_t, shared_ptr<Item>> load_items(const string &file_name) const;
    /**
     * @brief Load and return a map of non-playing characters (NPCs) from the specified JSON file.
     * @throw CUSTOM_EXCEPTION - when the data in JSON are invalid(missing required keys or values) or unusable for the game
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param file_name The file path for the NPCs JSON file.
     * @return A map of non-playing characters (NPCs).
     */
    map<size_t, shared_ptr<NonPlayingCharacter>> load_NPCs(const string &file_name) const;
    /**
     * @brief Load and return a map of attributes for a character from the specified JSON block.
     * This function creates a new instance of m_attributes and load the values from json block loaded in function load_user_initializer
     * @throw CUSTOM_EXCEPTION - when the json block does not have all four required attributes or the values are not positive
     * @throw UNKNOWN_EXCEPTION - any other exception
     * @param block The JSON block containing the attributes.
     * @return A map of attributes for a character.
     */
    map<size_t, shared_ptr<Attribute>> load_attributes_for_character(const json &block) const;
    /**
     * @brief Map storing the attributes. There are always keys 1, 2, 3, and 4
     */
    map<size_t, shared_ptr<Attribute>> m_attributes;
    /**
     * @brief Map storing the effects.
     */
    map<size_t, shared_ptr<Effect>> m_effects;
    /**
     * @brief Map storing the items.
     */
    map<size_t, shared_ptr<Item>> m_items;
    /**
     * @brief Map storing the non-playing characters (NPCs).
     */
    map<size_t, shared_ptr<NonPlayingCharacter>> m_NPSs;
};
