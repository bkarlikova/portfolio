#include "attribute.h"


Attribute::Attribute(size_t id, string name, int value, int max)
    : m_id(id),
    m_name(std::move(name)),
    m_current(value),
    m_max(max)
    {}


Attribute::Attribute(const json &element)
    : m_id(element["id"]),
    m_name(element["name"]),
    m_current(0),
    m_max(0)
    {}


bool Attribute::is_positive() const {
    return m_current > 0;
}

bool operator==(const shared_ptr<Attribute> &lhs, const shared_ptr<Attribute> & rhs) {
    return (tie(lhs->m_id, lhs->m_name, lhs->m_current, lhs->m_max) ==
            tie(rhs->m_id, rhs->m_name, rhs->m_current, rhs->m_max));
}


Attribute& Attribute::update_attribute(char operation, int value) {
    switch(operation){
        case '+':
            m_current += value;
            break;
        case '-':
            m_current -= value;
            break;
        case '/':
            m_current /= value;
            break;
        case '*':
            m_current *= value;
            break;
        case '%':
            m_current = m_current % value;
            break;
        default:
            throw CustomExceptions("invalid operation in 'update_attribute'");
    }
    if (m_current > m_max)
        m_current = m_max;
    if (m_current < 0)
        m_current = 0;
    return *this;
}

int Attribute::get_max() const {
    return m_max;
}

int Attribute::get_curr() const{
    return m_current;
}

string Attribute::get_name() const {
    return m_name;
}

string Attribute::build_string_of_attribute() const {
    string output = m_name + ": " + to_string(m_current) + "/" + to_string(m_max);
    if (m_id == ID_HEALTH && m_current == 0){
        output += " DEAD";
    }
    output += "\n";
    return output;
}

void Attribute::set_curr_max(int curr, int max) {
    if (max < curr)
        throw CustomExceptions("ERROR IN 'set_curr_max': max value is less than current value");
    if (max < 0)
        max = 0;
    m_max = max;
    m_current = curr;
}

void Attribute::set_curr(int curr) {
    if (m_max < curr)
        m_current = m_max;
    else{
        m_current = curr;
    }
}

map<size_t, shared_ptr<Attribute>> Attribute::copy_attributes(const map<size_t, shared_ptr<Attribute>> &other) {
    map<size_t, shared_ptr<Attribute>> new_attributes;
    for (const auto & att : other){
        new_attributes[att.first] = make_shared<Attribute>(*att.second);
    }
    return new_attributes;
}

