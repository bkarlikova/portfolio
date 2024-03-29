#include "automaton.h"

Automaton::Automaton(map<size_t, shared_ptr<State> > states,
                       set<string> alphabet,
                       map<size_t, map<string, size_t > > transitions,
                       map<size_t, shared_ptr<State> >finite_states,
                       shared_ptr<State> initial_state,
                       set<int> visited_states)
                       :
        m_states(std::move(states)),
        m_alphabet(std::move(alphabet)),
        m_transitions(std::move(transitions)),
        m_final_states(std::move(finite_states)),
        m_initial_state(initial_state),
        m_current_state(initial_state),
        m_visited_states(std::move(visited_states))
                       {
        set_finite_state_backup();
                       }

shared_ptr<State> Automaton::get_state(size_t state_id) const{
    return m_states.at(state_id);
}

shared_ptr<State> Automaton::get_current_state() const {
    return m_current_state;
}

shared_ptr<State>& Automaton::get_state_through_transition(size_t state_id, const std::string &trigger){
    return m_states.at(m_transitions.at(state_id).at(trigger));
}


vector<int> Automaton::get_visited() const {
    vector<int> output;
    for (const auto state : m_visited_states){
        output.emplace_back(state);
    }
    return output;
}


shared_ptr<State>& Automaton::modify_current_state() {
    return m_current_state;
}


void Automaton::use_finite_state_backup() {
    m_current_state = m_final_state_backup;
}

void Automaton::set_finite_state_backup() {
    m_final_state_backup = m_final_states.begin()->second;
}


void Automaton::move_to_state(const string &trigger) {
    try{
        shared_ptr<State> tmp = get_state_through_transition(m_current_state->get_id(), trigger);
        m_current_state = tmp;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}


bool Automaton::is_in_final_state() const {
    return m_final_states.find(m_current_state->get_id()) != m_final_states.end();
}

bool Automaton::is_the_rule_existing(size_t state_id, const std::string &trigger) const {
    try{
        if (m_transitions.find(state_id) != m_transitions.end())
            return m_transitions.at(state_id).find(trigger) != m_transitions.at(state_id).end();
        return false;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

string Automaton::build_transitions_offer(size_t state_id) const {
    try{
        string output;
        for (const auto & trans: m_transitions.at(state_id)){
            if (!is_visited(get_state(trans.second)->get_id())){
                output += "On '" +  trans.first + "' to room: " + to_string(get_state(trans.second)->get_id())
                          + " as " + get_state(trans.second)->get_name();
            }
        }
        return output;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}


void Automaton::add_visited_state(size_t state_id) {
    m_visited_states.emplace(state_id);
}


bool Automaton::is_visited(size_t state_id) const{
    return m_visited_states.find(state_id) != m_visited_states.end();
}
