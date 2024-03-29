#include "state.h"

using namespace std;

State::State(const json &element)
    : m_id(element.at("id")),
    m_name(element.at("name")),
    m_text_begin(element.at("textBegin")),
    m_text_end(element.at("textEnd"))
    {}

size_t State::get_id() const {
    return m_id;
}

string State::get_name() const {
    return m_name + "\n";
}

string State::get_intro() const {
    return "Welcome to " + m_name + "\n" + m_text_begin + "\n";
}

string State::get_outro() const {
    return m_text_end+ "\n";
}

const Attribute &State::get_enemys_health() const {
    throw CustomExceptions("Operation is not supported in State.");
}

