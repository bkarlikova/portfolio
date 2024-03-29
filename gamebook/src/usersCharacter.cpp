#include "usersCharacter.h"

User::User(const json &block,
             const map<size_t, shared_ptr<Attribute>> &attributes,
             const vector<shared_ptr<Effect> > &effects,
             const vector<shared_ptr<Item> > &items,
             shared_ptr<Item> weapon,
             shared_ptr<Item> armour,
             int capacity)
             : Character(block, attributes, effects, items, weapon, armour, capacity),
                m_saved_attributes(attributes)
             {
    if (block.contains("customName")){
        m_custom_name = std::move(block.at("customName"));
    }
             }

User::User(const json &block,
             const map<size_t,shared_ptr<Attribute>> &attributes,
             const vector<shared_ptr<Effect>> &effects)
             :
             Character(block, attributes, effects){
    if (block.at("capacity") < 3){
        throw CustomExceptions("ERROR in 'User ctor': invalid capacity, it has to >= 3");
    }
}


void User::use_object(const shared_ptr<Item> &item) {
    if (item->is_object()){
        if(item->get_effect()->is_permanent()){
            restore_attributes();
            m_effects.push_back(item->get_effect());
            apply_effects();
            save_attributes();
        }
        else{
            save_attributes();
            m_effects.push_back(item->get_effect());
            apply_effects();
        }
    }
    else if(item->is_weapon()){
        if (get_inventory().get_armour())
            m_health.update_attribute('-', max(item->get_hit_points() - get_inventory().get_armour()->get_anti_hit_points(), 0));
        else
            m_health.update_attribute('-', max(item->get_hit_points(), 0));
    }
}


string User::build_string_of_character() const{
    string output = "YOUR NAME: " + m_custom_name + " (" + m_name + ")\n" + m_health.build_string_of_attribute();
    output += "XP: " + to_string(m_xp) + "\n";
    output += build_string_of_attributes();
    output += get_inventory().build_string_of_inventory();
    return output;
}



size_t& User::modify_xp() {
    return m_xp;
}

size_t User::get_xp() const{
    return m_xp;
}

void User::set_name(string name) {
    m_custom_name = std::move(name);
}

const string& User::get_custom_name() const {
    return m_custom_name;
}


shared_ptr<Item> User::pick_object()const {
    try{
        cout << "Pick object:\n";
        vector<shared_ptr<Item> > objects = m_inventory.get_objects();
        cout << m_inventory.build_string_of_items(objects);
        int input = get_int_from_user();
        while(input <= 0 || input > (int)objects.size()){
            cout << INVALID_INPUT;
            input = get_int_from_user();
        }
        return objects.at(input-1);
    }
    catch(const MenuException & ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}



void User::save_attributes() {
    m_saved_attributes = Attribute::copy_attributes(m_attributes);
}

void User::restore_attributes() {
    m_attributes = Attribute::copy_attributes(m_saved_attributes);
}

shared_ptr<Character> User::copy() const {
    return make_shared<User>(*this);
}

const map<size_t, shared_ptr<Attribute>> &User::get_save_attributes() const {
    return m_saved_attributes;
}