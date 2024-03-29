/**
 * @file character.h
 * @brief Defines the Character class and its member functions.
 */

#pragma once

#include <string>
#include <map>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "attribute.h"
#include "inventory.h"
#include "permanentEffect.h"
#include "constants.h"
#include "helperFunctions.h"

using json = nlohmann::json;
using namespace std;

/**
 * @class Character
 * @brief Represents a character in the game.
 */
class Character{
public:
    /**
      * @brief Default constructor.
      */
    Character() = default;
    /**
      * @brief Default destructor.
      */
    virtual ~Character() = default;
    /**
     * @brief Constructor based on parameters generated in CDatabase::load_user_offer or CDatabase::load_NPCs or CDatabase::load_user.
     * @param block The JSON block containing character data.
     * @param attributes The map of character attributes.
     * @param effects The vector of permanent effects.
     * @param items The vector of items.
     * @param weapon The character's weapon.
     * @param armour The character's armor.
     * @param capacity The character's inventory capacity.
     * @throw json::exception when a required key is missing in the JSON block.
     */
    Character(const json &block,
               const map<size_t, shared_ptr<Attribute>> &attributes,
               const vector<shared_ptr<Effect> > &effects,
               const vector<shared_ptr<Item> > &items,
               shared_ptr<Item> weapon,
               shared_ptr<Item> armour,
               int capacity);
    /**
     * @brief Constructor using a JSON block.
     * @param block The JSON block containing character data.
     * @param attributes The map of character attributes.
     * @param effects The vector of permanent effects.
     * @throw json::exception when a required key is missing in the JSON block.
     */
    Character(const json &block,
               const map<size_t, shared_ptr<Attribute> > &attributes,
               const vector<shared_ptr<Effect> > &effects);
    /**
     * @brief Checks if the character is alive.
     * @return True if the character's health is positive, false otherwise.
     */
    [[nodiscard]] bool is_alive() const;
    /**
     * @brief Pure virtual method that uses an item (inherited CWeapon or CObject) and modifies the character's attributes,
     * health, or inventory capacity.
     * @param item The item to use.
     */
    virtual void use_object(const shared_ptr<Item> &item) = 0;
    /**
     * @brief Creates a deep copy of the shared_ptr<Character>.
     * @return A new instance of shared_ptr<Character>.
     */
    [[nodiscard]] virtual shared_ptr<Character> copy() const = 0;
    /**
     * @brief Applies an effect on the character, modifying its attributes, health, or capacity.
     * @param effect The effect to apply.
     * @return A reference to the modified character instance.
     */
    virtual Character& apply_effect(const shared_ptr<Effect> &effect);
    /**
     * @brief Applies all effects in m_effects in a single operation.
     * @return A reference to the modified character instance.
     */
    virtual Character& apply_effects();
    /**
     * @brief Adds an item to the character's inventory if there is space, otherwise displays an error message.
     * @param item The item to add.
     */
    virtual void add_item_into_inventory(const shared_ptr<Item> &item);
    /**
     * @brief Builds a printable string of the character's name and health.
     * @return A string representing the character.
     */
    [[nodiscard]] virtual string build_string_of_character() const;
    /**
     * @brief Builds a printable string of the character offered to the user at the beginning of the game,
     * including name, effects, attributes, and health.
     * @return A string representing the character's offer.
     */
    [[nodiscard]] virtual string build_string_of_character_for_offer() const;
    /**
     * @brief Builds a string of all attributes in m_attributes, including their current value, max value,
     * operation, and its value.
     * @return A string representing the character's attributes.
     */
    [[nodiscard]] virtual string build_string_of_attributes() const;
    /**
     * @brief Builds a string of all effects in m_effects.
     * @return A string representing the character's effects.
     */
    [[nodiscard]] virtual string build_string_of_effects() const;
    /**
     * @brief Returns the character's ID.
     * @return The character's ID.
     */
    [[nodiscard]] virtual size_t get_id() const;
    /**
     * @brief Returns a const reference to the character's name.
     * @return The character's name.
     */
    [[nodiscard]] virtual const string& get_name() const;
    /**
     * @brief Returns a const reference to the character's attributes.
     * @return The character's attributes.
     */
    [[nodiscard]] virtual const map<size_t, shared_ptr<Attribute>>& get_attributes() const;
    /**
     * @brief Returns a const reference to the character's health attribute.
     * @return The character's health attribute.
     */
    [[nodiscard]] virtual const Attribute& get_health() const;
    /**
     * @brief Returns a const reference to the character's effects.
     * @return The character's effects.
     */
    [[nodiscard]] virtual const vector<shared_ptr<Effect>>& get_effects() const;
    /**
     * @brief Returns a const reference to the character's inventory.
     * @return The character's inventory.
     */
    [[nodiscard]] virtual const Inventory& get_inventory() const;
    /**
     * @brief Returns a reference to the character's attributes.
     * @return The character's attributes.
     */
    [[nodiscard]] virtual map<size_t, shared_ptr<Attribute>>& modify_attributes();
    /**
     * @brief Returns a reference to the character's inventory.
     * @return The character's inventory.
     */
    [[nodiscard]] virtual Inventory& modify_inventory();
    /**
     * @brief Calculates experience points after a fight between the user and an enemy.
     * The points are calculated based on the attributes and health max values of the defeated enemy.
     * @return The experience points gained.
     */
    [[nodiscard]] virtual int return_xp() const;
protected:
    /**
      * @brief The unique ID of the character.
      */
    size_t m_id;
    /**
      * @brief The name of the character.
      */
    string m_name;
    /**
      * @brief The map of character attributes with keys 1, 2, 3, 4 correlating with their IDs.
      */
    map<size_t, shared_ptr<Attribute>> m_attributes;
    /**
      * @brief The attribute representing health, with ID_HEALTH.
      */
    Attribute m_health;
    /**
      * @brief The vector of permanent basal effects.
      */
    vector<shared_ptr<Effect> > m_effects;
    /**
      * @brief The character's inventory.
      */
    Inventory m_inventory;
};
