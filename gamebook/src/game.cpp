#include "game.h"

void Game::load_new_game() {
    try{
        if (!fs::exists(m_config_path)){
            throw CustomExceptions("config path does not exist");
        }
        if (fs::is_empty(m_config_path)){
            throw CustomExceptions("config director is empty");
        }
        vector<string > files;
        for (auto const& file : fs::directory_iterator{m_config_path}){
            files.emplace_back(file.path());
        }
        sort(files.begin(), files.end());
        cout << "LOADING CONFIGURATION FILES\n";
        m_database = Database(files[0], files[2], files[3], files[4]);
        m_automaton = m_database.load_automaton(files[1]);
        m_initializer = m_database.load_user_initializer(files[5]);
        cout << "\nGAME HAS BEEN LOADED SUCCESSFULLY!\n" << endl;
        sleep();
        wait_and_clear();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

void Game::load_prev_game(const string & path_from) {
    try{
        m_saved_game_path = string(SAVED_GAMES) + "/" + path_from;
        vector<string > files;
        if (!fs::exists(m_saved_game_path)){
            cout << "Path does not exist" << endl;
            sleep_long();
            wait_and_clear();
            throw MenuException();
        }
        if (fs::is_empty(m_saved_game_path)){
            cout << "Directory is empty" << endl;
            if (m_saved_game_path.find(SAVED_GAMES) != std::string::npos){
                fs::remove_all(m_saved_game_path);
            }
            sleep_long();
            wait_and_clear();
            throw MenuException();
        }
        for (auto const& file : fs::directory_iterator{m_saved_game_path}){
            files.emplace_back(file.path());
        }
        sort(files.begin(), files.end());
        cout << "LOADING CONFIGURATION FILES\n";
        m_database = Database(files[0], files[2], files[3], files[4]);
        m_automaton = m_database.load_automaton(files[1]);
        m_user = m_database.load_user(files[5]);
        cout << "\nGAME HAS BEEN LOADED SUCCESSFULLY!\n" << endl;
        sleep();
        wait_and_clear();
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

void Game::save_user(){
    json user;
    try{
        user["id"] = m_user.get_id();
        user["name"] = m_user.get_name();
        user["xp"] = m_user.get_xp();
        user["customName"] = m_user.get_custom_name();
        user["healthMax"] = m_user.get_health().get_max();
        user["healthCurr"] = m_user.get_health().get_curr();
        user["at1"]["curr"] = m_user.get_attributes().at(1)->get_curr();
        user["at2"]["curr"] = m_user.get_attributes().at(2)->get_curr();
        user["at3"]["curr"] = m_user.get_attributes().at(3)->get_curr();
        user["at4"]["curr"] = m_user.get_attributes().at(4)->get_curr();
        user["at1"]["max"] = m_user.get_attributes().at(1)->get_max();
        user["at2"]["max"] = m_user.get_attributes().at(2)->get_max();
        user["at3"]["max"] = m_user.get_attributes().at(3)->get_max();
        user["at4"]["max"] = m_user.get_attributes().at(4)->get_max();
        user["capacity"] = m_user.get_inventory().get_capacity().get_max();
        user["weapon"] = m_user.get_inventory().get_weapon()->get_id();
        user["armour"] = m_user.get_inventory().get_armour()->get_id();
        vector<int> tmp;
        for (const auto & eff: m_user.get_effects()){
            tmp.emplace_back(eff->get_id());
        }
        user["effects"] = tmp;
        tmp.clear();
        for (const auto & item: m_user.get_inventory().get_items()){
            tmp.emplace_back(item->get_id());
        }
        user["items"] = tmp;
    }
    catch (const exception &ex){
        throw CUSTOM_EXCEPTION_M(ex, "writing into json failed");
    }
    try{
        ofstream of(m_saved_game_path + "/userChar.json", std::ios::trunc);
        if (of.is_open()){
            string user_string = user.dump();
            of.write(user_string.c_str(), user_string.size());
        }
        else{
            throw CustomExceptions("writing into file failed");
        }
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

void Game::save_automaton() {
    try{
        json automaton = Database::get_automaton(m_config_path + "/automaton.json");
        automaton["currentState"] = m_automaton.modify_current_state()->get_id();
        automaton["visitedStates"] = m_automaton.get_visited();
        ofstream of(m_saved_game_path + "/automaton.json", std::ios::trunc);
        if (of.is_open()){
            string automaton_string = automaton.dump();
            of.write(automaton_string.c_str(), automaton_string.size());
        }
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

void Game::save_game(const string & path) {
    m_saved_game_path = path;
    m_game_is_running = false;
    try{
        if (fs::exists(m_saved_game_path)){
            fs::remove_all(m_saved_game_path);
        }
        if(!fs::exists(m_saved_game_path)){
            fs::create_directory(m_saved_game_path);
        }
        fs::copy(m_config_path + "/attributes.json", m_saved_game_path + "/attributes.json");
        fs::copy(m_config_path + "/effects.json", m_saved_game_path + "/effects.json");
        fs::copy(m_config_path + "/items.json", m_saved_game_path + "/items.json");
        fs::copy(m_config_path + "/npc_s.json", m_saved_game_path + "/npc_s.json");
        save_automaton();
        save_user();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

string Game::choose_file() const {
    cout << "Choose from following saved games:\n";
    vector<string> paths;
    for (const auto & path : fs::directory_iterator{SAVED_GAMES}){
        paths.emplace_back(path.path().filename());
    }
    for (size_t i = 0; i < paths.size(); i ++){
        cout << "NO. " << i+1 << " " << paths.at(i) << '\n';
    }
    int input = get_int_from_user("Type number: ");
    while(input <= 0 && input > (int)paths.size()){
        cout << INVALID_INPUT;
        input = get_int_from_user("Type number: ");
    }
    return paths.at(input-1);
}

bool Game::ask_about_end() {
    clear();
    string input  = get_string_from_user("\nAre you sure you want to end the game? y/n: ");
    return (input == YES[0] || input == YES[1]);
}

void Game::quit_game() {
    m_game_is_running = false;
    wait_and_clear();
    if (m_saved_game_path.find("saved_games") != std::string::npos) {
        fs::remove_all(m_saved_game_path);
    }
    cout << "\nTHANK YOU FOR SPENDING TIME WITH THIS GAME BOOK. HAVE A NICE DAY\n" << endl;
}

string Game::build_final_stats() const {
    if (m_user.is_alive()){
        string output = "\n\n Congratulations you made it to the end of the game book and still alive. \n"
                        "--------------- FINAL STATS -------------\n";
        output += m_user.build_string_of_character();
        return output;
    }
    else{
        string output = "\n\n \"Game Over!\n"
                        "Unfortunately, your adventure has come to an end.\n"
                        "--------------- FINAL STATS -------------\n";
        output += m_user.build_string_of_character();
        return output;
    }
}

void Game::run_game() {
    m_game_is_running = true;
    while(m_user.is_alive()){
        sleep();
        try{
            m_automaton.modify_current_state()->run_state(m_user);
            m_automaton.add_visited_state(m_automaton.get_current_state()->get_id());
            sleep();
            if (m_automaton.is_in_final_state()){
                cout << build_final_stats();
                string input = get_string_from_user("Press anything to quit the game: ");
                quit_game();
                return;
            }
            cout << "\nChoose command for moving into new room:\n";
            string offer =  m_automaton.build_transitions_offer(m_automaton.get_current_state()->get_id());
            if (offer.empty()){
                m_automaton.use_finite_state_backup();
                continue;
            }
            cout << offer;
            string user_input_str = get_string_from_user();
            while(!m_automaton.is_the_rule_existing(m_automaton.get_current_state()->get_id(), user_input_str)
                || !m_automaton.is_visited(m_automaton.get_current_state()->get_id())){
                cout << INVALID_INPUT;
                user_input_str = get_string_from_user();
            }
            m_automaton.move_to_state(user_input_str);
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
}


void Game::intro() {
    cout << "Welcome to the Game-Book!\n\n"
            "Embark on an epic adventure filled with perilous challenges and thrilling encounters. In this game, you will explore mysterious rooms, face formidable enemies, and tackle tricky tasks.\n"
            "But first, it's time to create your own unique character. Choose wisely, for your character's attributes, health, weapons, armor, and inventory will shape your journey.\n\n"
            "Your character comes with the following features:\n"
            "   - Four distinct attributes that will determine your abilities and skills.\n"
            "   - A health value that reflects your vitality and determines your survival.\n"
            "   - A  weapon that will aid you in combat.\n"
            "   - An armor to protect you from harm.\n"
            "   - An inventory with limited capacity to carry essential items.\n\n"
            "Now, let's begin your adventure. Prepare to make choices, face consequences, and uncover the secrets that await you.\n\n"
            "Game Instructions:\n"
            "Once you start the game, a menu will appear. You can choose actions from the menu by typing your commands into the terminal.\n"
            "At any point in the game, if you wish to access the menu, simply type 'm' or 'menu'. The menu will provide you with various options and choices.\n"
            "To save your progress, go to the menu and type 'save' or 'sv'. Please note that saving is only available after your character is initialized.\n\n"
            "Are you ready to begin your adventure?\n" << endl;
    get_string_from_user("Type anything to continue: ");
}


string Game::choose_from_menu() {
    clear();
    cout << "-------MENU------\n\n" << "choose from following commands:\n\n"
        << "    * START NEW GAME -> press 's' or 'start'\n"
        << "    * QUIT -> press 'q' or 'quit'\n";;
    if (!fs::is_empty(SAVED_GAMES))
        cout << "    * CONTINUE PREVIOUS GAME -> press 'c' or 'continue'\n";
    if (m_game_is_running) {
        cout << "    * PAUSE -> press 'p' or 'pause'\n";
        cout << "    * SAVE -> press 'sv' or 'save'\n";
    }
    cout << "\n";
    while(true){
        string input = get_string_from_user();
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (!check_menu_choice(input)){
            cout << INVALID_INPUT;
            continue;
        }
        return input;
    }
}


bool Game::check_menu_choice(const string &menu_output) const{
    if (menu_output == START[0] || menu_output == START[1])
        return true;
    else if (menu_output == QUIT[0] || menu_output == QUIT[1])
        return true;
    else if (m_game_is_running && (menu_output == PAUSE[0] || menu_output == PAUSE[1]))
        return true;
    else if (m_game_is_running && (menu_output == SAVE[0] || menu_output == SAVE[1]))
        return true;
    else if ((menu_output == CONTINUE_P[0] || menu_output == CONTINUE_P[1]) && !fs::is_empty(SAVED_GAMES))
        return true;
    else
        return false;
}

void Game::run_menu(const string &menu_output) {
    try{
        if (menu_output == START[0] || menu_output == START[1]){
            wait_and_clear();
            load_new_game();
            m_user = m_initializer.init_user();
            wait_and_clear();
            run_game();
        }
        else if (menu_output == QUIT[0] || menu_output == QUIT[1]){
            wait_and_clear();
            if (ask_about_end())
                quit_game();
            else {
                clear();
                run_menu(choose_from_menu());
            }
        }
        else if (m_game_is_running && (menu_output == SAVE[0] || menu_output == SAVE[1])){
            wait_and_clear();
            cout << "Type a name of the directory you would like to save the game to: " << endl;
            string path_to = get_string_from_user();
            while(!validate_path_string(path_to)){
                cout << INVALID_INPUT;
                path_to = get_string_from_user();
            }
            save_game(string(SAVED_GAMES) + "/" + path_to + "/");
        }
        else if (m_game_is_running && (menu_output == PAUSE[0] || menu_output == PAUSE[1])){
            wait_and_clear();
            cout << "----- PAUSE -----\n";
            while(true){
                string input = get_string_from_user("Type 'c' or 'continue' for continue the game: ");
                if (input == CONTINUE_P[0] || input == CONTINUE_P[1])
                    run_game();
            }
        }
        else if ((menu_output == CONTINUE_P[0] || menu_output == CONTINUE_P[1]) && !fs::is_empty(SAVED_GAMES)){
            wait_and_clear();
            load_prev_game(choose_file());
            run_game();
        }
        else{
            throw CustomExceptions("Invalid input in 'run_menu' -> previous functions have not fulfilled the purpose");
        }
    }
    catch (const MenuException & ex){
        return run_menu(choose_from_menu());
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

void Game::run(const string & config_files) {
    m_config_path = config_files;
    try{
        intro();
        clear();
        run_menu(choose_from_menu());
    }
    catch (const MenuException & ex) {
        return run_menu(choose_from_menu());
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}