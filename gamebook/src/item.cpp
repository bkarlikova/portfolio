#include "item.h"

Item::Item(const json & block) : m_id(block["id"]), m_name(block["name"]) {}

const string& Item::get_name() const {
    return m_name;
}

bool Item::is_armour() const {
    return false;
}

bool Item::is_object() const {
    return false;
}

bool Item::is_weapon() const {
    return false;
}

int Item::get_hit_points() const {
    throw CustomExceptions("Get HP is not supported in Item.");
}

shared_ptr<Effect> Item::get_effect() const {
    throw CustomExceptions("Get effect is not supported in Item.");
}

int Item::get_anti_hit_points() const {
    throw CustomExceptions("Get anti HP is not supported in Item.");
}

size_t Item::get_id() const {
    return m_id;
}


