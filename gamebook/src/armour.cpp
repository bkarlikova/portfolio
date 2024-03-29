#include "armour.h"

Armour::Armour(const json &block) : Item(block), m_anti_hit_points(block.at("antiHP")) {}

int Armour::get_anti_hit_points() const {
    return m_anti_hit_points;
}

bool Armour::is_armour() const{
    return true;
}

string Armour::build_string_of_item() const {
    return m_name + " - antiHP: " + to_string(m_anti_hit_points) + "\n";
}

shared_ptr<Item> Armour::copy() const {
    return make_shared<Armour>(*this);
}