/**
 * @file automaton.h
 * @brief Defines the Automaton class and its member functions.
 */
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include "state.h"

using namespace std;

/**
 * @class Automaton
 *
 * @brief this class is representing a deterministic finite state machine.
 * @param m_state
 * @param m_alphabet - all possible stings
 */
class Automaton{
public:
    Automaton() = default;
    ~Automaton() = default;
    /**
     * @brief Parametric constructor of Automaton. Variables are prepared in Database::load_automaton"
     * @param states
     * @param alphabet
     * @param transitions
     * @param finite_states
     * @param initial_state
     * @param visited_states
     */
    Automaton( map<size_t, shared_ptr<State> > states,
                set<string> alphabet,
                map<size_t, map<string, size_t > > transitions,
                map<size_t, shared_ptr<State> >finite_states,
                shared_ptr<State> initial_state,
                set<int> visited_states = {});
    /**
     * @brief this function is returning the shared_ptr<State> based on its id
     * @param state_id - existing id in map m_states
     */
    [[nodiscard]] shared_ptr<State> get_state (size_t state_id) const;
    /**
     * @brief this function returns a vector of visited states so far
     * @return output - can be empty
     */
    [[nodiscard]] vector<int> get_visited()const;
    /**
     * @brief getter for @param m_current_state
     */
    [[nodiscard]] shared_ptr<State> get_current_state() const;
    /**
     * @brief this function is returning state based on its transition from other state
     * @param state_id
     * @param trigger
     * @return
     */
    shared_ptr<State>& get_state_through_transition (size_t state_id, const string & trigger);
    /**
     * @brief this function is setting m_final_state_backup as a current state. It is used when all the transitions to
     * other state were used, or the states were visited.
     */
    void  use_finite_state_backup();
    /**
     * @brief his function returns a reference to current state
     */
     shared_ptr<State>& modify_current_state();
    /**
    * @brief functions that adds a state id to set of visited states
    * @param state_id
    */
    void add_visited_state(size_t state_id);
    /**
    * @brief checking if the state, based on its id, it is already visited
    * @param state_id
    * @return boolean
    */
    [[nodiscard]] bool is_visited(size_t state_id)const;
    /**
     * @brief this function checks if the current state is final or not
     */
    [[nodiscard]] bool is_in_final_state() const;
    /**
     * @brief this function checks if the there is transition using trigger command to state based on its id
     * @param state_id
     * @param trigger
     * @return boolean
     */
    [[nodiscard]] bool is_the_rule_existing(size_t state_id, const string & trigger) const;
    /**
     * @brief this function sets a current state as a state based on trigger command
     * @param trigger - existing trigger command
     */
    void move_to_state(const string & trigger);
    /**
     * @brief this function builds an output string from all possible transitions from state based on its id
     * @param state_id - id of the state from which are the transitions
     * @return string
     */
    [[nodiscard]] string build_transitions_offer(size_t state_id)const;
private:
    /**
     * @brief this function sets the first final state from map m_final_states as the finite state backup
     */
    void set_finite_state_backup();
    /**
     * @brief map representing all the existing states in automaton
     */
    map<size_t, shared_ptr<State> > m_states;
    /**
     * @brief all existing transition commands that are used in automaton
     */
    set<string> m_alphabet;
    /**
     * @brief transitions for every state to all other possible one
     */
    map<size_t , map<string, size_t> > m_transitions;
    /**
     * @brief all final states - subset of m_state
     */
    map<size_t, shared_ptr<State> >  m_final_states;
    /**
     * @brief one initial state, it is already in m_states
     */
    shared_ptr<State> m_initial_state;
    /**
     * @brief first state of map "m_final state" used when there are no transitions from current state
     */
    shared_ptr<State> m_final_state_backup;
    /**
     * @brief variable representing current state
     */
    shared_ptr<State> m_current_state;
    /**
     * @brief set of id of states, that were visited by user
     */
    set<int> m_visited_states;
};
