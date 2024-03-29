/**
 * @file effect.h
 * @brief Defines the Effect class and its member functions.
 */

#pragma once
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

/**
 * @class Effect
 * @brief Represents an effect that can be applied to a character or an item.
 *
 * The Effect class represents an effect that can be applied to a character or an item. Effects can modify the current value of attributes, health and the capacity of inventory
 * It stores information such as the effect ID, name, associated attribute ID and name, value, and operation type.
 */
class Effect {
public:
    /**
     * @brief Default constructor.
     */
    Effect() = default;
    /**
     * @brief Constructor with specified effect properties.
     * @param id The effect ID.
     * @param name The effect name.
     * @param att_id The associated attribute ID.
     * @param att_name The associated attribute name.
     * @param value The value of the effect.
     * @param operation The arithmetic operation used with the value.
     */
    Effect(size_t id, string name, size_t att_id, string att_name, size_t value, char operation);
    /**
     * @brief Constructor with specified effect properties from JSON element.
     * @param element The JSON element containing effect properties.
     * @param att_name The associated attribute name.
     */
    explicit Effect(const json& element, string att_name);
    /**
     * @brief Default destructor.
     */
    virtual ~Effect() = default;
    /**
     * @brief Get the effect ID.
     * @return The effect ID.
     */
    [[nodiscard]] virtual size_t get_id() const;
    /**
     * @brief Get the effect name.
     * @return The effect name.
     */
    [[nodiscard]] virtual string get_name() const;
    /**
     * @brief Get the associated attribute ID.
     * @return The associated attribute ID.
     */
    [[nodiscard]] virtual size_t get_attribute_id() const;
    /**
     * @brief Get the associated attribute name.
     * @return The associated attribute name.
     */
    [[nodiscard]] virtual string get_attribute_name() const;
    /**
     * @brief Get the effect value.
     * @return The effect value.
     */
    [[nodiscard]] virtual int get_value() const;
    /**
     * @brief Get the operation type of the effect.
     * @return The operation type of the effect.
     */
    [[nodiscard]] virtual char get_operation() const;
    /**
     * @brief Build a string representation of the effect.
     * @return A string representation of the effect.
     */
    [[nodiscard]] virtual string build_string_of_effect() const;
    /**
     * @brief Build a string representation of the effect's operation.
     * @return A string representation of the effect's operation.
     */
    [[nodiscard]] virtual string build_string_of_operation() const;
    /**
     * @brief Check if the effects datatype is CTemporary.
     * @return Returns false defaulted.
     */
    [[nodiscard]] virtual bool is_temporary() const;
    /**
     * @brief Check if the effects datatype is CPermanent.
     * @return Returns false defaulted.
     */
    [[nodiscard]] virtual bool is_permanent() const;

protected:
    /**
     * @brief The id of the effect
     */
    size_t m_id;
    /**
     * @brief the name of the effect
     */
    string m_name;
    /**
     * @brief id of the attribute that the effect is associated with
     */
    size_t m_att_id;
    /**
     * @brief name of the attribute that the effect is associated with
     */
    string m_att_name;
    /**
     * @brief value that is applyied by m_operation on the associated attribute
     */
    int m_value;
    /**
     * @brief character representing the operation that is used on the associated attribute
     */
    char m_operation;
};
