/**
 * @file item.h
 * @brief Defines the Item class and its member functions.
 */

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "effect.h"
#include "customExceptions.h"

using namespace std;

/**
 * @class Item
 * @brief Represents an item in the game.
 *
 * The Item class is an abstract class that represents an item in the game.
 * It provides common functionality and attributes for items such as name, ID, hit points,
 * anti-hit points, and effects. Subclasses of Item can specialize the behavior and properties
 * of specific types of items such as weapons, armours, and objects.
 */
class Item {
public:
    /**
     * @brief Default constructor.
     */
    Item() = default;
    /**
     * @brief Virtual default destructor.
     */
    virtual ~Item() = default;
    /**
     * @brief Constructor with JSON block.
     * @param block The JSON block defining the item containing keys: id, name
     */
    explicit Item(const json& block);

    /**
     * @brief Retrieves the name of the item.
     * @return A reference to the name of the item.
     */
    [[nodiscard]] virtual const string& get_name() const;

    /**
     * @brief Retrieves the m_id of the item.
     * @return The id of the item.
     */
    [[nodiscard]] virtual size_t get_id() const;

    /**
     * @brief Retrieves the hit points of the item.
     * @throw CCustomException - this function is not supported here, only for Items child CWeapon
     */
    [[nodiscard]] virtual int get_hit_points() const;

    /**
     * @brief Retrieves the anti-hit points of the item.
     * @throw CCustomException - this function is not supported here, only for Items child CArmour
     */
    [[nodiscard]] virtual int get_anti_hit_points() const;

    /**
     * @brief Retrieves the effect of the item.
     * @throw CCustomException - this function is not supported here, only for Items child CObject
     */
    [[nodiscard]] virtual shared_ptr<Effect> get_effect() const;

    /**
     * @brief Checks if the item is a datatype CWeapon.
     * @return Returns false as default.
     */
    virtual bool is_weapon() const;

    /**
     * @brief Checks if the item is a datatype CArmour.
     * @return Returns false defaulted.
     */
    virtual bool is_armour() const;

    /**
     * @brief Checks if the item is a datatype CObject.
     * @return Returns false defaulted.
     */
    virtual bool is_object() const;

    /**
     * @brief Builds a string representation of the item.
     * @return The string representation of the item.
     */
    [[nodiscard]] virtual string build_string_of_item() const = 0;

    /**
     * @brief Creates a copy of the item.
     * @return A shared pointer to the copied item.
     */
    virtual shared_ptr<Item> copy() const = 0;

protected:
    /**
     * @brief The unique id of the item.
     */
    size_t m_id;
    /**
     * @brief The name of the item
     */
    string m_name;
};
