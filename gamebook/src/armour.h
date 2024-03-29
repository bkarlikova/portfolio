/**
 * @file armour.h
 * @brief Defines the Armour class inherited from Item and their member functions.
 */
#pragma once
#include "item.h"

/**class Armour
 * @brief A class representing an armour.
 *
 * This class inherits from the Item class and provides specific functionality for armour items.
 */
class Armour : public Item {
public:
    /**
     * @brief Default constructor.
    */
    Armour() = default;
    /**
     * @brief Constructor that takes a json block to initialize the object.
     * @param block A json object containing the necessary data for initializing the armour. If there is not the key "antiHP",
     * json parsing library is throwing exception.
     */
    explicit Armour(const json & block);
    /**
     * @brief getter for m_anti_hit_points
     * @return m_anti_hit_points
     */
    [[nodiscard]] int get_anti_hit_points() const override;
    /**
     * @brief override function from parent Item checking if the Item is armour
     */
    [[nodiscard]] bool is_armour() const override;
    /**
     * @brief override function from parent Item building an output string
     */
    [[nodiscard]] string build_string_of_item() const override;
    /**
     * @brief override function from parent Item for deep copy of shared_ptr<Item>
    */
    [[nodiscard]] shared_ptr<Item> copy() const override;
private:
    /**
     * @brief integer representing anti hit points
     */
    int m_anti_hit_points;
};