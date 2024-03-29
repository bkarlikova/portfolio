#include "effect.h"

Effect::Effect(size_t id, string name, size_t att_id, string att_name, size_t value, char operation) :
        m_id(id),
        m_name(std::move(name)),
        m_att_id(att_id),
        m_att_name(std::move(att_name)),
        m_value(value),
        m_operation(operation)
        {}

Effect::Effect(const json &element, string att_name) :
    m_id(element["id"]),
    m_name(element["name"]),
    m_att_id(element["attribute_id"]),
    m_att_name(std::move(att_name)),
    m_value(element["value"]),
    m_operation(element["operation"].get<string>().front())
    {}


size_t Effect::get_id() const {
    return m_id;
}

string Effect::get_name() const {
    return m_name;
}

int Effect::get_value() const {
    return m_value;
}

char Effect::get_operation() const {
    return m_operation;
}

size_t Effect::get_attribute_id() const {
    return m_att_id;
}

string Effect::get_attribute_name() const {
    return m_att_name;
}

string Effect::build_string_of_effect() const {
    return m_name + " : " +  build_string_of_operation();
}

string Effect::build_string_of_operation() const {
    return m_att_name + " " + m_operation + to_string(m_value);
}

bool Effect::is_permanent() const {
    return false;
}

bool Effect::is_temporary() const {
    return false;
}