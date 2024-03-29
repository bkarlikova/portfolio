/**
 * @file object.h
 * @brief Defines the Object class, a inherited subclass of Item.
 */
#pragma once
#include "item.h"

/**
 * @class Object
 * @brief Represents an object in the game.
 *
 * The Object class is a subclass of Item and represents an object in the game.
 * It inherits the properties and behaviors from the Item class and provides additional functionality specific to objects.
 */
class Object : public Item {
public:
    /**
     * @brief Constructor with parameters.
     * @param block The JSON block containing object data.
     * @param effect The effect associated with the object.
     */
    Object(const json & block, shared_ptr<Effect> effect);
    /**
     * @brief Checks if the item is an object.
     * @return Returns true in this subclass.
     */
    [[nodiscard]] bool is_object() const override;
    /**
     * @brief Retrieves the effect associated with the object.
     * @return A shared pointer to the effect associated with the object.
     */
    [[nodiscard]] shared_ptr<Effect> get_effect() const override;
    /**
     * @brief Builds a string representation of the object.
     * @return A string representation of the object.
     */
    [[nodiscard]] string build_string_of_item() const override;
    /**
     * @brief Creates a copy of the object.
     * @return A shared pointer to a copy of the object.
     */
    [[nodiscard]] shared_ptr<Item> copy() const override;
private:
    /**
     * @brief The effect associated with the object.
     */
    shared_ptr<Effect> m_effect;
};
