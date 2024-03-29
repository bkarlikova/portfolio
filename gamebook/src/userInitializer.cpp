#include "userInitializer.h"

UserInit::UserInit(const vector<User> &characters,
                     const vector<shared_ptr<Item>> &weapons,
                     const vector<shared_ptr<Item>> &armours,
                     const vector<shared_ptr<Item>> &items,
                     int units) :
                     m_characters(characters),
                     m_weapons(weapons),
                     m_armours(armours),
                     m_items(items),
                     m_units(units)
                     {}

string UserInit::build_armour_offer() const {
    try {
        string output;
        for (size_t i = 0; i < m_armours.size(); i++){
            output += to_string(i+1)  + " : " + m_armours.at(i)->build_string_of_item();
        }
        return output;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

string UserInit::build_weapon_offer() const {
    try {
        string output;
        for (size_t i = 0; i < m_weapons.size(); i++) {
            output += to_string(i + 1) + " : " + m_weapons.at(i)->build_string_of_item();
        }
        return output;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

string UserInit::build_items_offer() const {
    try {
        string output;
        for (size_t i = 0; i < m_items.size(); i++) {
            output += to_string(i + 1) + " : " + m_items.at(i)->build_string_of_item();
        }
        return output;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

string UserInit::build_characters_offer() const {
    try {
        string output;
        for (size_t i = 0; i < m_characters.size(); i++) {
            output += "\nNO: " + to_string(i + 1) + ".\n" + m_characters.at(i).build_string_of_character_for_offer();
        }
        return output;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

string UserInit::choose_name() const {
    try {
        cout << "\nWrite your name: ";
        string input;
        try{
            getline(cin, input);
            transform(input.begin(), input.end(), input.begin(), ::tolower);
            if(input == MENU[0] || input == MENU[1]){
                throw MenuException();
            }
            return input;
        }
        catch (const MenuException &ex){
            throw MenuException();
        }
        catch(...){
            cout << INVALID_INPUT;
            return choose_name();
        }
    }
    catch (const MenuException & ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

void UserInit::set_values_of_attributes(int units, User & user)const {
    cout << "Now you have: " + to_string(units)
            + " units. Choose value for each attribute (write one number at a time). Choose wisely. Their sum cannot be bigger than "
            + to_string(units) + ".\n";
    for (size_t i = 1; i < 5; i ++){
        cout << "ATTRIBUTE no. " + to_string(i) + ": " + user.get_attributes().at(i)->get_name() + " _ / "
                + to_string(user.get_attributes().at(i)->get_max()) + "\n";
    }
    for (size_t i = 1; i < 5; i ++){
        if (units > 0){
            int input = get_int_from_user("Enter one integer for attribute no. " + to_string(i) + " : ");
            if (input > user.modify_attributes().at(i)->get_max()){
                input = user.modify_attributes().at(i)->get_max();
            }
            if (units - input > 0){
                units -= input;
            }
            else {
                input = units;
                units = 0;
            }
            user.modify_attributes().at(i)->set_curr(input);
            cout << "Rest of the units: " + to_string(units) + "\n";
        }
    }
    cout << "\nYour attributes:\n";
    for (size_t i = 1; i < 5; i ++){
        cout << "ATTRIBUTE no. " + to_string(i) + ": " + user.get_attributes().at(i)->get_name() + " "
                + to_string(user.get_attributes().at(i)->get_curr()) + " / "
                + to_string(user.get_attributes().at(i)->get_max()) + "\n";
    }
    user.save_attributes();
}

User UserInit::choose_character() const {
    try{
        cout << "\nWrite a number of character: \n";
        cout << build_characters_offer();
        int input = get_int_from_user();
        while (input <= 0 || input > (int)m_characters.size()){
            cout << INVALID_INPUT;
            input = get_int_from_user();
        }
        return m_characters.at(input-1);
    }
    catch (const MenuException & ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

shared_ptr<Item> UserInit::choose_armour() const {
    try{
        cout << "\nWrite a number of armour: \n";
        cout << build_armour_offer();
        int input = get_int_from_user();
        while (input <= 0 || input > (int)m_armours.size()){
            cout << INVALID_INPUT;
            input = get_int_from_user();
        }
        cout << "You picked: " + m_armours.at(input-1)->build_string_of_item();
        return m_armours.at(input-1);
    }
    catch (const MenuException & ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

shared_ptr<Item> UserInit::choose_weapon() const {
    try{
        cout << "\nWrite a number of weapon: \n";
        cout << build_weapon_offer();
        int input = get_int_from_user();
        while (input <= 0 || input > (int)m_weapons.size()){
            cout << INVALID_INPUT;
            input = get_int_from_user();
        }
        cout << "You picked: " + m_weapons.at(input-1)->build_string_of_item();
        return m_weapons.at(input-1);
    }
    catch (const MenuException & ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

vector<shared_ptr<Item> > UserInit::choose_items() const {
    try{
        cout << "\nWrite 3 numbers of 3 items. You cannot pick one thing twice: \n";
        cout << build_items_offer();
        vector<shared_ptr<Item> > items;
        vector<int> inputs;
        for (size_t i = 0; i < 3; i ++){
            int input = get_int_from_user("Type " + to_string(i+1) + ". number: ");
            while (input <= 0 || input > (int)m_items.size() || (find(inputs.begin(), inputs.end(), input) != inputs.end())){
                cout << INVALID_INPUT;
                input = get_int_from_user();
            }
            inputs.emplace_back(input);
            sort(inputs.begin(), inputs.end());
            items.emplace_back(m_items.at(input-1));
        }
        for (size_t i = 0; i < 3; i ++) {
            cout << "You picked: " + items.at(i)->build_string_of_item();
        }
        return items;
    }
    catch (const MenuException & ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}


User UserInit::init_user()const {
    try{
        string name = choose_name();
        User new_user = choose_character();
        set_values_of_attributes(m_units, new_user);
        new_user.modify_inventory().set_armour(choose_armour());
        new_user.modify_inventory().set_weapon(choose_weapon());
        new_user.modify_inventory().set_items(choose_items());
        new_user.set_name(name);
        new_user.apply_effects();
        sleep();
        wait_and_clear();
        cout << new_user.build_string_of_character();
        get_string_from_user("Press anything to continue: ");
        return new_user;
    }
    catch (const MenuException & ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}