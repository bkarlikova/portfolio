/**
 * @file temporaryEffect.h
 * @brief Defines the TemporaryEffect class.
 */

#pragma once
#include "effect.h"

/**
 * @class TemporaryEffect
 * @brief Represents a temporary effect in the game.
 *
 * The TemporaryEffect class is a derived class of Effect that represents a temporary effect in the game.
 * It provides functionality to build a string representation of the effect and check if the effect is temporary.
 */
class TemporaryEffect : public Effect {
public:
    /**
     * @brief Constructor with parameters.
     * @param block The JSON block containing effect data.
     * @param att_name The name of the attribute associated with the effect.
     */
    explicit TemporaryEffect(const json & block, string att_name);
    /**
     * @brief Builds a string representation of the effect.
     * @return A string representation of the effect.
     */
    string build_string_of_effect() const override;
    /**
     * @brief Checks if the effect is temporary.
     * @return True for instance of TemporaryEffect.
     */
    bool is_temporary() const override;
};
