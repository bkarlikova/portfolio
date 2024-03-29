/**
 * @file event.h
 * @brief Defines the Event class and its member functions.
 */

#pragma once
#include "state.h"

/**
 * @class Event
 * @brief Represents an event state in the game.
 *
 * The Event class is a derived class of State and represents an event state in the game.
 * It inherits the run_state function from the State class and provides additional member functions
 * for handling event-specific operation.
 */
class Event : public State {
public:
    /**
     * @brief Constructor for creating an event state.
     * @param block The JSON block containing event properties.
     * @param items A vector of shared pointers to items available in the event.
     * @param item A shared pointer to a secret item in the event.
     */
    Event(const json& block, vector<shared_ptr<Item>> items, shared_ptr<Item> item);
    /**
     * @brief Runs the event state. It handles all actions from printing intro to closing the state, including player's input and its validation.
     * @throw MenuException When user writes into terminal "menu" or "m", this function is throwing an exception, which calls menu function
     * @param user The user object representing the player.
     */
    void run_state(User& user) override;
private:
    /**
     * @brief Picks an item from the available items based on the index.
     * @param idx The index of the item to pick.
     * @return A shared pointer to the picked item.
     */
    [[nodiscard]] shared_ptr<Item> pick_item(size_t idx) const;
    /**
     * @brief Gets the secret item when users current value of third attribute is higher than random generated number and adds it to the user's inventory.
     * @param user The user object representing the player.
     */
    void get_secret_item(User& user) const;
    /**
     * @brief Evaluates the player's answer.
     * @param answer The player's answer to evaluate.
     * @return True if the answer is correct, false otherwise.
     */
    bool eval_answer(const string& answer) const;
    /**
     * @brief Gets the task description for the event.
     * @return The task description as a string.
     */
    string get_task();
    /**
     * @brief Gets the offer for available items in the event.
     * @return The items offer as a string.
     */
    string get_items_offer();
    /**
     * @brief The task description, mostly representing as a question
     */
    string m_task;
    /**
     * @brief The correct answer to the task
     */
    string m_correct_answer;
    /**
     * @brief vector of item that user can pick after quesing the task correctely
     */
    vector<shared_ptr<Item>> m_items;
    /**
     * @brief An secret item, that can user ger, if his current value of third attribute (User - m_attribute) is more than random generated number in interval
     */
    shared_ptr<Item> m_secret_item;
};
