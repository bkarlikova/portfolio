/**
 * @file weapon.h
 * @brief Defines a Weapon class inherited form Item.
 */
#pragma once
#include "item.h"

class Weapon : public Item {
public:
    /**
     * @brief Default constructor for Weapon.
     */
    Weapon() = default;
    /**
     * @brief Constructor for Weapon.
     * @param block The JSON block containing weapon data.
     */
    explicit Weapon(const json & block);
    /**
     * @brief Checks if the item is a weapon.
     * @return True, because this class id Weapon
     */
    [[nodiscard]] bool is_weapon() const override;
    /**
     * @brief Retrieves the hit points of the weapon.
     * @return The hit points of the weapon.
     */
    [[nodiscard]] int get_hit_points() const override;
    /**
     * @brief Builds a string representation of the weapon item.
     * @return The string representation of the weapon item.
     */
    [[nodiscard]] string build_string_of_item() const override;
    /**
     * @brief Creates a copy of the weapon item.
     * @return A shared pointer to the copied weapon item.
     */
    [[nodiscard]] shared_ptr<Item> copy() const override;
private:
    /**
     * @brief The number of hit points
     */
    int m_hit_points;
};
