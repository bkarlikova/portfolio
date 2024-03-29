/**
 * @file attributes.h
 * @brief Defines the Attributes class and its member functions.
 */
 #pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "effect.h"
#include "menuException.h"
#include "constants.h"

using namespace std;
using json = nlohmann::json;

/**
 * @class Attribute
 * @brief A class representing an attribute
 *
 * This class provides functionality to manage attributes and perform operations on them.
 */
class Attribute{
public:
    /**  @brief default constructor
     */
    Attribute() = default;
    /**
     * @brief constructor initializing the attribute with json bloc.
     * @throw json::exception If there is missing some key from json block, json parsing libs throws exceptions"
     * @param element
     */
    Attribute(size_t id, string name, int value, int max);
    /**
     * @brief constructor initializing the attribute with the provided values.
     * @param id
     * @param name
     * @param value
     * @param max
     */
    explicit Attribute(const json & element);
    /**
     * @brief default destructor
    */
    ~Attribute() = default;
    /**
     * @brief operator== for two shared pointers of Attribute
     * @return True when all variables equals;
     */
    friend bool operator==(const shared_ptr<Attribute> &lhs, const shared_ptr<Attribute> & rhs);
    /**@brief check if the current value of attribute is positive
     * @return bool
     */
    [[nodiscard]] bool is_positive() const;
    /**
     * @brief getter for @param m_max
     */
    [[nodiscard]] int get_max() const;
    /**
    * @brief getter for @param m_current
     */
    [[nodiscard]] int get_curr() const;
    /**
     * @brief getter for @param m_name
     */
    [[nodiscard]] string get_name() const;
    /**
     * @brief setter for @param m_current and @param m_max. If the values are not negative, they are set to 0.
     * @throw CustomExceptions
     * @param curr
     * @param max
     */
    void set_curr_max(int curr, int max);
    /**
     * @brief setter for @param m_current
     * @param curr
     */
    void set_curr(int curr);
    /**
     * @brief function for building a output string of any attribute
     */
    [[nodiscard]] string build_string_of_attribute() const;
    /**
     * @brief function that updates attribute by given operation. If the operation is
     * @throw CustomExceptions if there is given unsupported operation, function will throw exception
     * @param operation
     * @param value
     * @return
     */
    Attribute& update_attribute(char operation, int value);
    /**
     * @brief function for making a deep copy of each attribute in map attribute
     * @param other
     * @return
     */
    static map<size_t, shared_ptr<Attribute>> copy_attributes(const map<size_t, shared_ptr<Attribute>>  & other);
private:
    /**
     * @brief The id of this attribute
     */
    size_t m_id;
    /**
     * @brief The human readable name of this attribute
     */
    string m_name;
    /**
     * @brief The current value of the attribute
     */
    int m_current;
    /**
     * @brief The max value of the attribute
     */
    int m_max;
};