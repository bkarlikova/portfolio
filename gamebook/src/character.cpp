#include "character.h"

Character::Character(const json &block,
                       const map<size_t, shared_ptr<Attribute>> &attributes,
                       const vector<shared_ptr<Effect> > &effects,
                       const vector<shared_ptr<Item> > &items,
                       shared_ptr<Item> weapon,
                       shared_ptr<Item> armour,
                       int capacity)
                       :
                       m_id(block.at("id")),
                       m_name(block.at("name")),
                       m_attributes(attributes),
                       m_health(),
                       m_effects(effects),
                       m_inventory(weapon,
                                   armour,
                                   Attribute(ID_INVENTORY, string(NAME_INV), items.size(), capacity),
                                   items)
                       {
    if (!block.contains("healthCurr")){
        m_health = Attribute(ID_HEALTH, string(NAME_HEALTH), block.at("healthMax"), block.at("healthMax"));
    }
    else{
        m_health = Attribute(ID_HEALTH, string(NAME_HEALTH), block.at("healthCurr"), block.at("healthMax"));
    }

                       }
Character::Character(const json &block,
                       const map<size_t,shared_ptr<Attribute>> &attributes,
                       const vector<shared_ptr<Effect>> &effects)
                       :
                       m_id(block.at("id")),
                       m_name(block.at("name")),
                       m_attributes(attributes),
                       m_health(),
                       m_effects(effects),
                       m_inventory(block.at("capacity"))
                       {
    if (!block.contains("healthCurr")){
        m_health = Attribute(ID_HEALTH, string(NAME_HEALTH), block.at("healthMax"), block.at("healthMax"));
    }
    else{
        m_health = Attribute(ID_HEALTH, string(NAME_HEALTH), block.at("healthCurr"), block.at("healthMax"));
    }
                       }

bool Character::is_alive() const {
    return m_health.is_positive();
}

size_t Character::get_id() const {
    return m_id;
}

const string& Character::get_name() const {
    return m_name;
}
const map<size_t, shared_ptr<Attribute>>& Character::get_attributes() const {
    return m_attributes;
}

const vector<shared_ptr<Effect>>& Character::get_effects() const{
    return m_effects;
}

const Attribute& Character::get_health()const {
    return m_health;
}

const Inventory& Character::get_inventory()const {
    return m_inventory;
}

map<size_t, shared_ptr<Attribute>>& Character::modify_attributes()  {
    return m_attributes;
}

Inventory& Character::modify_inventory() {
    return m_inventory;
}


string Character::build_string_of_character() const {
    string output;
    output = "NAME: " + m_name + "\n" + m_health.build_string_of_attribute();
    return output;
}

string Character::build_string_of_character_for_offer() const {
    string output;
    output = "NAME: " + m_name + "\n";
    output += m_health.build_string_of_attribute();
    output += build_string_of_effects();
    output += build_string_of_attributes();
    return output;
}


string Character::build_string_of_attributes() const {
    string output;
    output = "__ ATTRIBUTES __\n";
    for (const auto & [key, value] : m_attributes){
        output += value->build_string_of_attribute();
    }
    return output;
}

string Character::build_string_of_effects() const {
    string output = "__ EFFECTS __\n";
    if (m_effects.empty()){
        return output + string(NONE) + "\n";
    }
    for (const auto & effect : m_effects){
        output += effect->build_string_of_effect();
    }
    return output;
}



Character& Character::apply_effect(const shared_ptr<Effect> &effect) {
    if (effect->get_attribute_id() == ID_HEALTH)
        m_health.update_attribute(effect->get_operation(), effect->get_value());
    else if (effect->get_attribute_id() == ID_INVENTORY)
        m_inventory.modify_capacity().update_attribute(effect->get_operation(), effect->get_value());
    else
        m_attributes.at(effect->get_attribute_id())->update_attribute(effect->get_operation(), effect->get_value());
    return *this;
}


Character& Character::apply_effects() {
    for (const auto &effect : m_effects){
       apply_effect(effect);
    }
    m_effects.clear();
    return *this;
}

void Character::add_item_into_inventory(const shared_ptr<Item> &item) {
    if (!m_inventory.is_full()){
        m_inventory.add_to_items(item);
    }
    else{
        cout << "Your inventory is full. Item cannot be added.\n" << endl;
    }
    return;
}

int Character::return_xp() const{

    return m_health.get_max()/2
           + m_attributes.at(1)->get_curr() / 3
           + m_attributes.at(2)->get_curr() / 4
           + m_attributes.at(3)->get_curr() / 5
           + m_attributes.at(4)->get_curr() / 6;
}



