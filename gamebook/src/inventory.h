/**
 * @file inventory.h
 * @brief Defines the Inventory class and its member functions.
 */

#pragma once
#include "weapon.h"
#include "armour.h"
#include "object.h"
#include "attribute.h"
#include "constants.h"

using namespace std;

/**
 * @class Inventory
 * @brief Represents an inventory of items.
 *
 * The Inventory class manages a collection of items including weapons, armours, and objects.
 * It provides methods to add items, delete items, change weapon or armour, set weapons and armours, modify capacity,
 * and perform various operations on the items.
 */
class Inventory {
public:
    /**
     * @brief Default constructor.
     */
    Inventory() = default;
    /**
     * @brief Destructor.
     */
    ~Inventory() = default;
    /**
     * @brief Constructor with maximum capacity.
     * @param max_capacity The maximum capacity of the inventory.
     */
    Inventory(int max_capacity);
    /**
     * @brief Constructor with initial items and attributes.
     * @param weapon The initial weapon.
     * @param armour The initial armour.
     * @param capacity The capacity attribute.
     * @param items The initial items.
     */
    Inventory(shared_ptr<Item> weapon, shared_ptr<Item> armour, const Attribute& capacity, vector<shared_ptr<Item>> items);
    /**
     * @brief Checks if the inventory is full based on size of the vector with items and maximal capacity
     * @return True if the inventory is full, false otherwise.
     */
    bool is_full() const;
    /**
     * @brief Adds an item to the inventory, specifically into the vector of the items
     * @param item The item to add.
     */
    void add_to_items(const shared_ptr<Item>& item);
    /**
     * @brief Sets the weapon in the inventory.
     * @param weapon The weapon to set.
     */
    void set_weapon(const shared_ptr<Item>& weapon);
    /**
     * @brief Sets the armour in the inventory.
     * @param armour The armour to set.
     */
    void set_armour(const shared_ptr<Item>& armour);
    /**
     * @brief Sets the items in the inventory.
     * @param items The items to set.
     */
    void set_items(const vector<shared_ptr<Item>>& items);
    /**
     * @brief Deletes an item from the variable m_items by index.
     * @param index The index of the item to delete.
     */
    void delete_item(size_t index);
    /**
     * @brief Deletes an item from the variable m_items.
     * @param item The item to delete.
     */
    void delete_item(const shared_ptr<Item>& item);
    /**
     * @brief Retrieves the weapon from the inventory.
     * @return Value of m_weapon.
     */
    shared_ptr<Item> get_weapon() const;
    /**
     * @brief Retrieves the armour from the inventory.
     * @return Value of m_armour.
     */
    shared_ptr<Item> get_armour() const;
    /**
     * @brief Retrieves an item from the inventory by index.
     * @param idx The index of the item to retrieve.
     * @return The item at the specified index.
     */
    shared_ptr<Item> get_item(size_t idx) const;
    /**
     * @brief Retrieves an item from the inventory by its unique id.
     * @param id The id of the item to retrieve.
     * @return The item with specified id.
     */
    shared_ptr<Item> get_item_by_id(size_t id) const;
    /**
     * @brief Retrieves all items from the variable m_items.
     * @return A vector of shared pointers to the items.
     */
    vector<shared_ptr<Item>> get_items() const;
    /**
     * @brief Retrieves the capacity attribute of the inventory.
     * @return Value of current value of m_cappacity
     */
    Attribute get_capacity() const;
    /**
     * @brief Retrieves all weapons from the variable m_items.
     * @return A new vector of shared pointers to the weapons.
     */
    vector<shared_ptr<Item>> get_weapons() const;
    /**
     * @brief Retrieves all armours from the variable m_items.
     * @return A new vector of shared pointers to the armours.
     */
    vector<shared_ptr<Item>> get_armours() const;
    /**
     * @brief Retrieves all objects from the variable m_items.
     * @return A new vector of shared pointers to the objects.
     */
    vector<shared_ptr<Item>> get_objects() const;
    /**
     * @brief Return a reference to variable m_capacity -> ready to be modified
     * @return A reference to the capacity attribute.
     */
    Attribute& modify_capacity();
    /**
     * @brief Builds a string representation of vector m_items.
     * @return The string representation of the items.
     */
    string build_string_of_items() const;
    /**
     * @brief Builds a string representation of the specified vector.
     * @param items The items to include in the string representation.
     * @return The string representation of the items.
     */
    string build_string_of_items(const vector<shared_ptr<Item>>& items) const;
    /**
     * @brief Builds a string representation of the weapon in the inventory.
     * @return The string representation of the weapon.
     */
    string build_string_of_weapon() const;
    /**
     * @brief Builds a string representation of the armour in the inventory.
     * @return The string representation of the armour.
     */
    string build_string_of_armour() const;
    /**
     * @brief Builds a string representation of the entire inventory.
     * @return The string representation of the inventory.
     */
    string build_string_of_inventory() const;
    /**
     * @brief Swap the current weapon with given weapon from m_items.
     * @param item The weapon from m_items.
     */
    void change_weapon(const shared_ptr<Item>& item);
    /**
     * @brief Swap the current armour with given armour from m_items.
     * @param item The armour from m_items.
     */
    void change_armour(const shared_ptr<Item>& item);
    /**
     * @brief Checks if the variable m_items contains a weapon in its items.
     * @return True if the variable m_items contains a weapon, false otherwise.
     */
    bool have_weapon_in_items() const;
    /**
     * @brief Checks if the variable m_items contains an armour in its items.
     * @return True if the variable m_items contains an armour, false otherwise.
     */
    bool have_armour_in_items() const;
    /**
     * @brief Checks if the variable m_items contains an object in its items.
     * @return True if the variable m_items contains an object, false otherwise.
     */
    bool have_object_in_items() const;
private:
    /**
     * @brief The weapon in the inventory, only this weapon can be used in fight.
     */
    shared_ptr<Item> m_weapon;
    /**
     * @brief The armour in the inventory, only this armour is used in fight.
     */
    shared_ptr<Item> m_armour;
    /**
     * @brief The capacity attribute of the inventory.
     */
    Attribute m_capacity;
    /**
     * @brief The items in the inventory.
     */
    vector<shared_ptr<Item>> m_items;
};
