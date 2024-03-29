#include "tests.h"

char generate_random_char() {
    int randomAscii = rand() % 95 + 32;
    char randomChar = static_cast<char>(randomAscii);
    return randomChar;
}

User load_test_user(){
    Database database_test = Database(string(CONFIG_PATH) + "/attributes.json",
                                        string(CONFIG_PATH) + "/effects.json",
                                        string(CONFIG_PATH) + "/items.json",
                                        string(CONFIG_PATH) + "/npc_s.json");
    return database_test.load_user(string(TEST_PATH) + "/user_test.json");
}

bool is_eof_exception(const string & ex){
    regex eof_regex("Unexpected EOF", regex_constants::ECMAScript | std::regex_constants::icase);
    return (std::regex_search(ex, eof_regex));
}


bool test_loading_from_non_existing_file(){
    try{
        Game test_game;
        test_game.run(string("random_surely_non_existing_file"));
        return false;
    }
    catch (...){
        return true;
    }
}


bool use_object_w_temp_effect(){
    User user_test = load_test_user();
    map<size_t, shared_ptr<Attribute>> attributes_test = user_test.get_attributes();
    user_test.use_object(user_test.get_inventory().get_item_by_id(6));
    map<size_t, shared_ptr<Attribute>> saved_attributes_test = user_test.get_save_attributes();
    if (attributes_test.at(user_test.get_inventory().get_item_by_id(6)->get_effect()->get_attribute_id())
        == saved_attributes_test.at(user_test.get_inventory().get_item_by_id(6)->get_effect()->get_attribute_id()))
        return true;
    else
        return false;
}

bool use_object_w_perm_effect(){
    User user_test = load_test_user();
    map<size_t, shared_ptr<Attribute>> attributes_before = user_test.get_attributes();
    map<size_t, shared_ptr<Attribute>> saved_attributes_before = user_test.get_save_attributes();
    user_test.use_object(user_test.get_inventory().get_item_by_id(4));
    map<size_t, shared_ptr<Attribute>> attributes_after= user_test.get_attributes();
    map<size_t, shared_ptr<Attribute>> saved_attributes_after = user_test.get_save_attributes();
    if (attributes_before.at(user_test.get_inventory().get_item_by_id(6)->get_effect()->get_attribute_id())
        != attributes_after.at(user_test.get_inventory().get_item_by_id(6)->get_effect()->get_attribute_id())
        &&
        saved_attributes_before.at(user_test.get_inventory().get_item_by_id(6)->get_effect()->get_attribute_id())
        != saved_attributes_after.at(user_test.get_inventory().get_item_by_id(6)->get_effect()->get_attribute_id()) )
        return false;
    else
        return true;
}

bool test_deleting_object_from_inventory(){
    User user_test = load_test_user();
    int capacity_before = user_test.get_inventory().get_capacity().get_curr();
    user_test.modify_inventory().delete_item(user_test.get_inventory().get_item_by_id(4));
    int capacity_after_deleting = user_test.get_inventory().get_capacity().get_curr();
    if (capacity_after_deleting != capacity_before - 1)
        return false;
    user_test.modify_inventory().delete_item(user_test.get_inventory().get_item_by_id(4));
    int capacity_after_deleting_again = user_test.get_inventory().get_capacity().get_curr();
    if (capacity_after_deleting != capacity_after_deleting_again)
        return false;
    return true;
}

bool test_swapping_armour(){
    User user_test = load_test_user();
    shared_ptr<Item> armour_to_set = user_test.get_inventory().get_item_by_id(3);
    user_test.modify_inventory().change_armour(armour_to_set);
    if (armour_to_set == user_test.get_inventory().get_armour())
        return true;
    return false;
}

bool test_swapping_weapon(){
    User user_test = load_test_user();
    shared_ptr<Item> weapon_to_set = user_test.modify_inventory().get_item_by_id(2);
    user_test.modify_inventory().change_weapon(weapon_to_set);
    if (weapon_to_set == user_test.get_inventory().get_weapon())
        return true;
    return false;
}

bool test_validate_file_path(){
    string test;
    if (validate_path_string(test))
        return false;
    for (size_t i = 0; i < 20; i ++){
        test += generate_random_char();
    }
    test += "/";
    if (validate_path_string(test))
        return false;
    test.pop_back();
    test += ":";
    if (validate_path_string(test))
        return false;
    test.pop_back();
    test += "\n";
    if (validate_path_string(test))
        return false;
    return true;
}



bool test_loading_from_config_file(){
    if (!fs::exists(string(CONFIG_PATH)) || fs::is_empty(string(CONFIG_PATH))){
        return false;
    }
    try{
        const char * simulated_input = "a\ns\n";
        istringstream iss(simulated_input);
        cin.rdbuf(iss.rdbuf());
        Game test_game;
        test_game.run(string(CONFIG_PATH));
        return true;
    }
    catch (const CustomExceptions &ex){
        return (is_eof_exception(string(ex.what())));
    }
    catch (...){
        return false;
    }
}


bool test_loading_prev_game_from_existing_file(){
    try{
        const char * simulated_input = "anything\ns\n";
        istringstream iss(simulated_input);
        cin.rdbuf(iss.rdbuf());
        Game test_game;
        test_game.run(string(CONFIG_PATH));
        return true;
    }
    catch (const CustomExceptions &ex){
        return (is_eof_exception(string(ex.what())));
    }
    catch (...){
        return false;
    }
}

bool use_object_on_enemy(){
    int enemys_health_before = 0;
    int enemys_health_after = 0;
    try{
        const char * simulated_input = "n\nn\nn\nw\n";
        istringstream iss(simulated_input);
        cin.rdbuf(iss.rdbuf());
        Database database_test = Database(string(CONFIG_PATH) + "/attributes.json",
                                            string(CONFIG_PATH) + "/effects.json",
                                            string(CONFIG_PATH) + "/items.json",
                                            string(CONFIG_PATH) + "/npc_s.json");
         Automaton test_automaton = database_test.load_automaton(string(CONFIG_PATH) + "/automaton.json");
        shared_ptr< State> fight_test = test_automaton.get_state(6);
        User user_test = load_test_user();
        enemys_health_before = fight_test->get_enemys_health().get_curr();
        fight_test->run_state(user_test);
        enemys_health_after = fight_test->get_enemys_health().get_curr();
    }
    catch (const CustomExceptions & ex){
        cout << enemys_health_before << " " << enemys_health_after << endl;
        if (is_eof_exception(string(ex.what()))){
            if (enemys_health_before > enemys_health_after)
                return true;
            else
                return false;
        }
        return false;
    }
    catch (...){
        return false;
    }
    return false;
}



bool test_menu_exception(){
    try{
        const char * simulated_input = "a\ns\nmenu\n";
        istringstream iss(simulated_input);
        cin.rdbuf(iss.rdbuf());
        Game test_game;
        test_game.run(string(CONFIG_PATH));
        return false;
    }
    catch (const  MenuException &ex){
        return true;
    }
    catch (const CustomExceptions &ex){
        return (is_eof_exception(string(ex.what())));
    }
    catch(...) {
        return false;
    }
}

bool test_loading_database(bool expected_ex, const string & path){
    try{
        const char * simulated_input = "a\ns\n";
        istringstream iss(simulated_input);
        cin.rdbuf(iss.rdbuf());
        Game test_game;
        test_game.run(path);
        return !expected_ex;
    }
    catch (const CustomExceptions &ex){
        if (is_eof_exception(string(ex.what()))){
            return !expected_ex;
        }
        return expected_ex;
    }
    catch (const exception &ex){
        return expected_ex;
    }
    catch(...) {
        return !expected_ex;
    }
}
