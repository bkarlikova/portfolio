/**
 * @file userInitializer.h
 * @brief This class defines a user initializer.
 */
#pragma once
#include "usersCharacter.h"

using namespace std;

/**
 * @brief The class responsible for initializing a user character.
 * This class provides several function were user can choose the character, weapon, armour value of his attributes and 3 objects.
 */
class UserInit {
public:
    /**
     * @brief Default constrictor
     */
    UserInit() = default;
    /**
     * @brief Constructor for UserInit.
     * @param characters A vector of available user characters for player.
     * @param weapons A vector of available weapons.
     * @param armours A vector of available armours.
     * @param items A vector of available items.
     * @param units The number of attribute units available for distribution into attributes.
     */
    UserInit(const vector<User>& characters,
              const vector<shared_ptr<Item>>& weapons,
              const vector<shared_ptr<Item>>& armours,
              const vector<shared_ptr<Item>>& items,
              int units);
    /**
    * @brief Default destructor for UserInit.
    */
    ~UserInit() = default;
    /**
     * @brief Initializes and returns a user character.
     * @return The initialized user character.
     */
    [[nodiscard]] User init_user() const;
private:
    /**
     * @brief This function lets the user to write his name for the character.
     * @return The chosen name.
     */
    [[nodiscard]] string choose_name() const;
    /**
     * @brief This function lets the user to choose a character from the given offer.
     * @return The chosen character.
     */
    [[nodiscard]] User choose_character() const;
    /**
     * @brief This function lets the user to choose a weapon from the given offer.
     * @return The chosen weapon.
     */
    [[nodiscard]] shared_ptr<Item> choose_weapon() const;
    /**
     * @brief This function lets the user to choose an armor from the given offer.
     * @return The chosen armor.
     */
    [[nodiscard]] shared_ptr<Item> choose_armour() const;
    /**
     * @brief This function lets the given offer. the user to choose items from the given offer.
     * @return The chosen items.
     */
    [[nodiscard]] vector<shared_ptr<Item>> choose_items() const;
    /**
     * @brief Let user to distribute the given units between his attributes
     * @param units The number of attribute units to distribute.
     * @param user The user character to set the values for.
     */
    void set_values_of_attributes(int units, User& user) const;
    /**
     * @brief Builds the offer string for selecting an armor.
     * @return The armor offer string.
     */
    [[nodiscard]] string build_armour_offer() const;
    /**
     * @brief Builds the offer string for selecting a weapon.
     * @return The weapon offer string.
     */
    [[nodiscard]] string build_weapon_offer() const;
    /**
     * @brief Builds the offer string for selecting items.
     * @return The items offer string.
     */
    [[nodiscard]] string build_items_offer() const;
    /**
     * @brief Builds the offer string for selecting characters.
     * @return The characters offer string.
     */
    [[nodiscard]] string build_characters_offer() const;
    /**
     * @brief Vector of all offered characters
     */
    vector<User> m_characters;
    /**
     * @brief Vector of all offered weapons
     */
    vector<shared_ptr<Item>> m_weapons;
    /**
     * @brief Vector of all offered armours
     */
    vector<shared_ptr<Item>> m_armours;
    /**
     * @brief Vector of all offered items
     */
    vector<shared_ptr<Item>> m_items;
    /**
     * @brief the number of units given to user for his attributes
     */
    int m_units;
};

