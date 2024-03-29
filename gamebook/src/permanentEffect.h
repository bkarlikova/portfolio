/**
 * @file permanentEffect.h
 * @brief Defines the PermanentEffect class, a subclass of Effect.
 */

#pragma once
#include "effect.h"

/**
 * @class PermanentEffect
 * @brief Represents a permanent effect in the game.
 *
 * The PermanentEffect class is a subclass of Effect and represents a permanent effect in the game.
 * It inherits the properties and behaviors from the Effect class and provides additional functionality specific to permanent effects.
 */
class PermanentEffect : public Effect {
public:
    /**
     * @brief Constructor with parameters.
     * @param block The JSON block containing effect data.
     * @param att_name The name of the attribute associated with the effect.
     */
    explicit PermanentEffect(const json & block, string att_name);
    /**
     * @brief Builds a string representation of the effect.
     * @return A string representation of the effect.
     */
    string build_string_of_effect() const override;
    /**
     * @brief Checks if the effect is permanent.
     * @return True if the effect is instance of PermanentEffect.
     */
    bool is_permanent() const override;
};
