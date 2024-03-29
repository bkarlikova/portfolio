/**
 * @file state.h
 * @brief Defines the State class, a base class for game states.
 */

#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "item.h"
#include "usersCharacter.h"
#include "nonPlayingChar.h"
#include "menuException.h"

using namespace std;
using json = nlohmann::json;

/**
 * @class State
 * @brief Represents a game state.
 *
 * The State class is a base class for game states. It provides common properties and behaviors for different game states.
 * It defines pure virtual functions that are implemented in derived classes.
 */
class State {
public:
    /**
     * @brief Default constructor.
     */
    State() = default;
    /**
     * @brief Constructor with JSON element.
     * @param element The JSON element containing specific keys and value for initializing state.
     */
    explicit State(const json & element);
    /**
     * @brief Virtual destructor.
     */
    virtual ~State() = default;
    /**
     * @brief Runs the state, Fight or Event.
     * @param user The user representing the player.
     */
    virtual void run_state(User & user) = 0;
    /**
     * @brief Retrieves the ind of the state.
     * @return The ind of the state.
     */
    [[nodiscard]] virtual size_t get_id() const;
    /**
     * @brief Retrieves the name of the state.
     * @return The const reference to the name of the state.
     */
    [[nodiscard]] virtual string get_name() const;
    /**
     * @brief This function is returning the attribute representing enemy's health. Function is used only for testing
     * @return The attribute representing enemy's health
     */
    virtual const Attribute& get_enemys_health() const;
protected:
    /**
     * @brief Retrieves the introductory text for the state.
     * @return The introductory text for the state.
     */
    [[nodiscard]] virtual string get_intro() const;
    /**
     * @brief Retrieves the concluding text for the state.
     * @return The concluding text for the state.
     */
    [[nodiscard]] virtual string get_outro() const;
    /**
     * @brief The id of the state.
     */
    size_t m_id;
    /**
     * @brief The name of the state.
     */
    string m_name;
    /**
     * @brief The introductory text for the state.
     */
    string m_text_begin;
    /**
     * @brief The ending text for the state.
     */
    string m_text_end;
};


