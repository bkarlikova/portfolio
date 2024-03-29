#include "nonPlayingChar.h"

NonPlayingCharacter::NonPlayingCharacter(const json &block,
                                           const map<size_t, shared_ptr<Attribute>> &attributes,
                                           const vector<shared_ptr<Effect> > &effects,
                                           const vector<shared_ptr<Item> > &items,
                                           shared_ptr<Item> weapon,
                                           shared_ptr<Item> armour,
                                           int capacity)
                                           :
                                           Character(block, attributes, effects, items, weapon, armour, capacity)
                                           {
    Character::apply_effects();
                                           }




string NonPlayingCharacter::build_string_of_character() const {
    string output = Character::build_string_of_character();
    output += "WEAPON: " + m_inventory.get_weapon()->build_string_of_item();
    return output;
}

void NonPlayingCharacter::use_object(const shared_ptr<Item> &item) {
    if (item->is_object()){
        if(item->get_effect()->is_permanent()){
            m_effects.push_back(item->get_effect());
            apply_effects();
        }
        m_inventory.delete_item(item);
    }
    else if(item->is_weapon()){
        if (get_inventory().get_armour())
            m_health.update_attribute('-', max(item->get_hit_points() - get_inventory().get_armour()->get_anti_hit_points(), 0));
        else
            m_health.update_attribute('-', max(item->get_hit_points(), 0));
    }
    return;
}

shared_ptr<Character> NonPlayingCharacter::copy() const {
    return make_shared<NonPlayingCharacter>(*this);
}
