/**
 * @file fight.h
 * @brief Defines the Fight class and its member functions.
 */

#pragma once
#include "state.h"

/**
 * @class Fight
 * @brief Represents a fight state in the game.
 *
 * The Fight class is a derived class of State and represents a fight state in the game.
 * It inherits the run_state function from the State class and provides additional member functions for handling fight-specific operations.
 */
class Fight : public State {
public:
    /**
     * @brief Constructor for creating a fight state.
     * @param element The JSON element containing fight properties.
     * @param enemy A shared pointer to the enemy character in the fight.
     */
    Fight(const json& element, shared_ptr<Character> enemy);
    /**
     * @brief Runs the fight state.
     * @throw MenuException is rethrown when MenuException is caught
     * @throw CUSTOM_EXCEPTION is throw when some know exception is caught
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs, but there shouldn't be any
     * @param user The user object representing the player.
     */
    void run_state(User& user) override;
    /**
     * @brief This function is returning the attribute representing enemy's health. Function is used only for testing
     * @return The attribute representing enemy's health
     */
    const Attribute& get_enemys_health() const override;
private:
    /**
     * @brief This function lets user to attack the m_enemy in fight
     * @throw MenuException is rethrown when MenuException is caught
     * @throw CUSTOM_EXCEPTION is throw when some know exception is caught
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs, but there shouldn't be any
     * @param user The user object representing the player.
     * @return A shared pointer to the item used by the user.
     */
    shared_ptr<Item> user_s_move(User& user);
    /**
     * @brief This function makes m_enemy's move. There is randomly chosen if the enemy attacks with weapon or item and which item.
     * @param user The user object representing the player.
     * @return A shared pointer to the item used by the enemy.
     */
    shared_ptr<Item> enemy_s_move(User& user);
    /**
     * @brief Changes the user's weapon if user want to.
     * This function gives user a chance to change his weapon before the fight starts. This function runs only if user have some weapon in his inventory, specifically in m_items
     * @throw MenuException is rethrown when MenuException is caught, because of there is user's input
     * @throw CUSTOM_EXCEPTION is throw when some know exception is caught
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs, but there shouldn't be any
     * @param user The user object representing the player.
     */
    void change_weapon(User& user);
    /**
     * @brief Changes the user's armour if user want to.
     * This function gives user a chance to change his armour before the fight starts. This function runs only if user have some armour in his inventory, specifically in m_items
     * @throw MenuException is rethrown when MenuException is caught, because of there is user's input
     * @throw CUSTOM_EXCEPTION is throw when some know exception is caught
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs, but there shouldn't be any
     * @param user The user object representing the player.
     */
    void change_armour(User& user);
    /**
     * @brief This function lets player to use some object from his inventory on himself before the fight.
     * @throw MenuException is rethrown when MenuException is caught, because of there is user's input
     * @throw CUSTOM_EXCEPTION is throw when some know exception is caught
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs, but there shouldn't be any
     * @param user The user representing the player.
     */
    void use_object_on_yourself(User& user);
    /**
     * @brief Executes the fight logic between the user and the enemy.
     * This function runs while loop till user or enemy is dead. It let player make his move and than it runs enemys_move if the enemy is not dead already.
     * @throw MenuException is rethrown when MenuException is caught, because of there is user's input
     * @throw CUSTOM_EXCEPTION is throw when some know exception is caught
     * @throw UNKNOWN_EXCEPTION is throw just in case when any other undefined exception occurs, but there shouldn't be any
     * @param user The user object representing the player.
     */
    void run_fight(User& user);
    /**
     * @brief Builds the string representation of the stats after user and enemy make one move each in the fight.
     * @param user The user object representing the player.
     * @param users_item A shared pointer to the item used by the user.
     * @param enemys_item A shared pointer to the item used by the enemy.
     * @return The string representation of the user's stats after moves.
     */
    string build_stats_after_moves(const User& user, const shared_ptr<Item>& users_item, const shared_ptr<Item>& enemys_item) const;
    /**
     * @brief Builds the string representation of the user's stats after the fight.
     * @param user The user object representing the player.
     * @return The string representation of the user's stats after the fight.
     */
    string build_stats_after_fight(const User& user) const;
    /**
     * @brief Builds the introductory string for the fight.
     * @param user The user object representing the player.
     * @return The introductory string for the fight.
     */
    string build_fight_intro(const User& user) const;
    /**
     * @brief A shared pointer to the enemy character in the fight.
     */
    shared_ptr<Character> m_enemy;
};
