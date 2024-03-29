#include "fight.h"

Fight::Fight(const json &element, shared_ptr<Character> enemy)
    : State(element), m_enemy(std::move(enemy)) {}

void Fight::change_armour(User &user) {
    cout << "Would you like to change armour?: y/n" << endl;
    try{
        vector<shared_ptr<Item> > armours = user.get_inventory().get_armours();
        cout << user.get_inventory().build_string_of_items(armours);
        string answer = get_string_from_user("Type y/n: ");
        while (answer != YES[0] && answer != YES[1] && answer != NO[0] && answer != NO[1]){
            cout << INVALID_INPUT;
            answer = get_string_from_user();
        }
        if (answer == YES[0] || answer == YES[1]){
            int input = get_int_from_user();
            while(input <= 0 || input > (int)armours.size()){
                cout << INVALID_INPUT;
                input = get_int_from_user();
            }
            user.modify_inventory().change_armour(armours.at(input-1));
        }
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

void Fight::change_weapon(User &user) {
    cout << "Would you like to change weapon?: y/n" << endl;
    try{
        vector<shared_ptr<Item> > weapons = user.get_inventory().get_weapons();
        cout << user.get_inventory().build_string_of_items(weapons);
        string answer = get_string_from_user("Type y/n: ");
        while (answer != YES[0] && answer != YES[1] && answer != NO[0] && answer != NO[1]){
            cout << INVALID_INPUT;
            answer = get_string_from_user();
        }
        if (answer == YES[0] || answer == YES[1]){
            int input = get_int_from_user("Type number: ");
            while(input <= 0 || input > (int)weapons.size()){
                cout << INVALID_INPUT;
                input = get_int_from_user();
            }
            user.modify_inventory().change_weapon(weapons.at(input-1));
        }
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

void Fight::use_object_on_yourself(User & user) {
    cout << "Would you like to use object?: y/n" << endl;
    try{
        vector<shared_ptr<Item> > objects = user.get_inventory().get_objects();
        cout << user.get_inventory().build_string_of_items(objects);
        string answer = get_string_from_user("Type y/n: ");
        while (answer != YES[0] && answer != YES[1] && answer != NO[0] && answer != NO[1]){
            cout << INVALID_INPUT;
            answer = get_string_from_user();
        }
        if (answer == YES[0] || answer == YES[1]){
            int input = get_int_from_user("Type number: ");
            while(input <= 0 || input > (int)objects.size()){
                cout << INVALID_INPUT;
                input = get_int_from_user();
            }
            user.use_object(objects.at(input-1));
            user.modify_inventory().delete_item(objects.at(input-1));
            for (size_t i = 1; i < 5; i++){
                cout << user.get_attributes().at(i)->build_string_of_attribute();
            }
        }
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


void Fight::run_fight( User & user) {
    try{
        user.save_attributes();
        cout << "----- Preparation before the fight -----" << endl;
        if (user.get_inventory().have_weapon_in_items()){
            change_weapon(user);
        }
        if (user.get_inventory().have_armour_in_items()){
            change_armour(user);
        }
        if (user.get_inventory().have_object_in_items()){
            use_object_on_yourself(user);
        }
        cout << "---–––––––––-- Let's fight ----–––––––––-" << endl;
        while(m_enemy->is_alive() && user.is_alive()){
            shared_ptr<Item> users_w;
            shared_ptr<Item> enemys_w;
            if (user.is_alive()){
                users_w = user_s_move(user);
            }
            if (m_enemy->is_alive()){
                enemys_w = enemy_s_move(user);
            }
            cout << build_stats_after_moves(user, users_w, enemys_w);
        }
        if (!m_enemy->is_alive() && user.is_alive()){
            user.modify_xp() += m_enemy->return_xp();
        }
        cout << build_stats_after_fight(user);
        if (user.is_alive()) user.restore_attributes();
    }
    catch (const MenuException & ex){
        throw MenuException();
    }
    catch (const CustomExceptions & ex){
        throw CustomExceptions("ERROR IN 'run_fight': ", ex);
    }
    catch (...){
        throw CustomExceptions::throw_unknown_error("run_fight");
    }
}

shared_ptr<Item> Fight::enemy_s_move(User & user) {
    try{
        int random_int = rand() % 2;
        switch (random_int){
            case 0:
                if (m_enemy->get_inventory().have_object_in_items()){
                    int random_object = rand() % max(m_enemy->get_inventory().get_capacity().get_curr(), 1);
                    while(!m_enemy->get_inventory().get_item(random_object)->is_object()){
                        random_object = rand() % max(m_enemy->get_inventory().get_capacity().get_curr(), 1);
                    }
                    shared_ptr<Item> item = m_enemy->get_inventory().get_item(random_object)->copy();
                    user.use_object(m_enemy->get_inventory().get_item(random_object));
                    m_enemy->modify_inventory().delete_item(random_object);
                    return item;
                }
                else{
                    user.use_object(m_enemy->get_inventory().get_weapon());
                    return m_enemy->get_inventory().get_weapon();
                }
            case 1:
                user.use_object(m_enemy->get_inventory().get_weapon());
                return m_enemy->get_inventory().get_weapon();
            default:
                return enemy_s_move(user);
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

shared_ptr<Item> Fight::user_s_move(User & user) {
    try{
        cout << "Would you like to attack by object or weapon? answer: o -> object, w -> weapon:\n";
        string input = get_string_from_user();
        while (input != WEAPON && input != OBJECT){
            cout << INVALID_INPUT;
            input = get_string_from_user();
        }
        if (input == WEAPON){
            m_enemy->use_object(user.get_inventory().get_weapon());
            return user.get_inventory().get_weapon();
        }
        else{
            if (user.get_inventory().have_object_in_items()){
                shared_ptr<Item> item = user.pick_object();
                shared_ptr<Item> item_copy = item->copy();
                m_enemy->use_object(item);
                user.modify_inventory().delete_item(item);
                return item_copy;
            }
            else{
                cout << "Sorry. You don't have any objects in inventory. Use weapon instead.\n";
                m_enemy->use_object(user.get_inventory().get_weapon());
                return user.get_inventory().get_weapon();
            }
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

string Fight::build_stats_after_moves(const User & user,
                                       const shared_ptr<Item> & users_item,
                                       const shared_ptr<Item> & enemys_item) const{

    string output;
    output = cover_to_length("YOU: " + user.get_custom_name()) + "ENEMY: " + m_enemy->get_name() + "\n";
    if (users_item != nullptr){
        output += cover_to_length("ATTACKED WITH: " + users_item->build_string_of_item());
    }
    else{
        output += cover_to_length(" ");
    }
    if (enemys_item != nullptr){
        output += "ATTACKED WITH: " + enemys_item->build_string_of_item();
    }
    else{
        output += "\n";
    }
    output += cover_to_length(user.get_health().build_string_of_attribute()) + m_enemy->get_health().build_string_of_attribute();
    output += "XP: " + to_string(user.get_xp()) + "\n";
    return output;
}

string Fight::build_stats_after_fight(const User &user) const {
    string output;
    output = "-------------- STATS: ------------------\n";
    if (user.is_alive())
        output += "YOU WON " + to_string(user.get_xp()) + " EXPERIENCE POINTS!\n";
    else
        output += "you died\n";
    return output;
}

string Fight::build_fight_intro(const User &user) const {
    try{
        string output = "Welcome to " + m_name + "\n" + m_text_begin + "\n\n";
        output += cover_to_length("YOU: " + user.get_custom_name()) + "ENEMY: " + m_enemy->get_name() + "\n";
        output += cover_to_length("WEAPON: " + user.get_inventory().get_weapon()->build_string_of_item())
                  + "WEAPON: " + m_enemy->get_inventory().get_weapon()->build_string_of_item();
        output += cover_to_length("ARMOUR: " + user.get_inventory().get_armour()->build_string_of_item())
                  + "ARMOUR: " + m_enemy->get_inventory().get_armour()->build_string_of_item();
        output += cover_to_length(user.get_health().build_string_of_attribute()) + m_enemy->get_health().build_string_of_attribute();
        for (size_t i = 1; i < 5; i++){
            output += cover_to_length(user.get_attributes().at(i)->build_string_of_attribute())
                      + m_enemy->get_attributes().at(i)->build_string_of_attribute();
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

void Fight::run_state(User & user) {
    try{
        clear();
        cout << build_fight_intro(user);
        sleep();
        run_fight(user);
        cout << get_outro();
        sleep();
    }
    catch (const MenuException &ex){
        throw MenuException();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

const Attribute &Fight::get_enemys_health() const {
    return m_enemy->get_health();
}

