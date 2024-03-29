#include "weapon.h"


Weapon::Weapon(const json &block) : Item(block), m_hit_points(block.at("HP")){}

bool Weapon::is_weapon()const {
    return true;
}

int Weapon::get_hit_points()const {
    return m_hit_points;
}

string Weapon::build_string_of_item() const {
    return m_name + " - HP: " + to_string(m_hit_points) + "\n";
}

shared_ptr<Item> Weapon::copy() const {
    return make_shared<Weapon>(*this);
}