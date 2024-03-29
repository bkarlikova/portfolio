/**
 * @file usersCharacter.h
 * @brief Defines the User class and its methods.
 */
#pragma once
#include "character.h"

class User : public Character {
public:
    /**
     * @brief Default constructor for User.
     */
    User() = default;

    /**
     * @brief Constructor for User.
     * @param block The JSON block containing user data.
     * @param attributes The map of attributes.
     * @param effects The vector of effects for the user.
     * @param items The vector of items for the user.
     * @param weapon The weapon for the user.
     * @param armour The armour for the user.
     * @param capacity The capacity of the user's inventory.
     */
    User(const json &block,
          const map<size_t, shared_ptr<Attribute>> &attributes,
          const vector<shared_ptr<Effect> > &effects,
          const vector<shared_ptr<Item> > &items,
          shared_ptr<Item> weapon,
          shared_ptr<Item> armour,
          int capacity);
    /**
     * @brief Constructor for User.
     * @param block The JSON block containing user data.
     * @param attributes The map of attributes for the user.
     * @param effects The vector of effects for the user.
     */
    User(const json &block,
          const map<size_t, shared_ptr<Attribute>> &attributes,
          const vector<shared_ptr<Effect> > &effects);
    /**
     * @brief Creates a copy of the user's character.
     * @return A shared pointer to the copied user's character.
     */
    [[nodiscard]] shared_ptr<Character> copy() const override;
    /**
     * @brief Uses an object on user from the his inventory.
     * @param object The object to use.
     */
    void use_object(const shared_ptr<Item> &object) override;
    /**
     * @brief Builds a string representation of the user character.
     * @return The string representation of the user character.
     */
    [[nodiscard]] string build_string_of_character() const override;
    /**
     * @brief Let user to pick an object from his inventory, specially from m_items
     * @return The picked object.
     */
    [[nodiscard]] shared_ptr<Item> pick_object() const;
    /**
     * @brief Saves the m_attributes into m_saved_attributes.
     */
    void save_attributes();
    /**
     * @brief Restores m_saved_attributes into m_attributes.
     */
    void restore_attributes();
    /**
     * @brief Returns a reference to m_xp for modifying.
     * @return A reference to the xp value.
     */
    size_t &modify_xp();
    /**
     * @brief Sets the custom name of the user character.
     * @param name The custom name to set.
     */
    void set_name(string name);
    /**
     * @brief Retrieves the experience points (XP) of the user character.
     * @return The XP value.
     */
    [[nodiscard]] size_t get_xp() const;
    /**
     * @brief Returns a const reference to the saved attributes of user.
     * @return The user's saved attributes.
     */
    [[nodiscard]] virtual const map<size_t, shared_ptr<Attribute>>& get_save_attributes() const;
    /**
     * @brief Retrieves the custom name of the user character.
     * @return The custom name.
     */
    const string& get_custom_name() const;
private:
    map<size_t, shared_ptr<Attribute>> m_saved_attributes;   // Saved attributes of the user character
    string m_custom_name;                                     // Custom name of the user character
    size_t m_xp = 0;
};
