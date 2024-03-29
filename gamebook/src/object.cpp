#include "object.h"

Object::Object(const json &block, shared_ptr<Effect> effect) : Item(block), m_effect(effect){}

bool Object::is_object()const {
    return true;
}

shared_ptr<Effect> Object::get_effect()const {
    return m_effect;
}

string Object::build_string_of_item() const {
    return m_name + " - effect: " + m_effect->build_string_of_effect();
}

shared_ptr<Item> Object::copy() const {
    return make_shared<Object>(*this);
}