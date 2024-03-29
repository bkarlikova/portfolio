/**
 * @file nonPlayingCharacter.h
 * @brief Defines the NonPlayingCharacter class, a subclass of Character.
 */

#pragma once
#include "character.h"

/**
 * @class NonPlayingCharacter
 * @brief Represents a non-playing character in the game.
 *
 * The NonPlayingCharacter class is a subclass of Character and represents a non-playing character in the game.
 * It inherits the attributes, effects, items, weapon, armour, and capacity from the Character class.
 * It provides additional functionality specific to non-playing characters.
 */
class NonPlayingCharacter : public Character {
public:
    /**
     * @brief Default constructor.
     */
    NonPlayingCharacter() = default;

    /**
     * @brief Constructor with parameters.
     * @param block The JSON block containing non-playing character's data.
     * @param attributes The attributes of the non-playing character.
     * @param effects The effects that will be applied to the non-playing character.
     * @param items The items the NPC possesses.
     * @param weapon The weapon of the non-playing character.
     * @param armour The armour of the non-playing character.
     * @param capacity The capacity of the non-playing character's inventory.
     */
    NonPlayingCharacter(const json &block,
                         const map<size_t, shared_ptr<Attribute>> &attributes,
                         const vector<shared_ptr<Effect> > &effects,
                         const vector<shared_ptr<Item> > &items,
                         shared_ptr<Item> weapon,
                         shared_ptr<Item> armour,
                         int capacity);

    /**
     * @brief Builds a string representation of the non-playing characterC.
     * @return A string representation of the non-playing character.
     */
    [[nodiscard]] string build_string_of_character() const override;

    /**
     * @brief Creates a copy of a shared pointer of non-playing character.
     * @return A shared pointer to a copy of the non-playing character.
     */
    [[nodiscard]] shared_ptr<Character> copy() const override;

    /**
     * @brief Uses an object on non-playing character representing this class from its inventory.
     * @param item The item to be used.
     */
    void use_object(const shared_ptr<Item> &item) override;
};
