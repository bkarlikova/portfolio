#include "tests.h"
#include <cassert>

using namespace std;
using json = nlohmann::json;

using namespace std;

int main(){
#ifdef TEST
    assert(use_object_w_perm_effect());
    assert(use_object_w_temp_effect());
    assert(test_deleting_object_from_inventory());
    assert(test_swapping_weapon());
    assert(test_swapping_armour());
    assert(test_validate_file_path());
    assert(use_object_on_enemy());
    assert(test_loading_from_config_file());
    assert(test_loading_from_non_existing_file());
    assert(test_loading_prev_game_from_existing_file());
    assert(test_menu_exception());
    assert(test_loading_database(true, string(TEST_PATH) + "/init_state_equals_final_state"));
    assert(test_loading_database(true, string(TEST_PATH) + "/missing_attribute2"));
    assert(test_loading_database(true, string(TEST_PATH) + "/missing_transition_to_state_6"));
    assert(test_loading_database(true, string(TEST_PATH) + "/using_non_existing_item"));
    clear();
    cout << "\nALL TESTS PASSED!" << endl;
    sleep_long();
    return EXIT_SUCCESS;
#else
    try{
        system("clear");
        Game game;
        game.run(string(CONFIG_PATH));
        return EXIT_SUCCESS;
    }
#ifdef DEBUG
    catch (const exception &ex) {
        cout << "\nCAUGHT EXCEPTION IN: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
#else
    catch (const exception &ex) {
        cout << "\nCAUGHT EXCEPTION. IF YOU WANT TO SEE WHERE IS THE PROBLEM, COMPILE WITH -DDEBUG" << endl;
        return EXIT_FAILURE;
    }
#endif
    catch (...) {
        cout << "\nUNKNOWN EXCEPTION CAUGHT" << endl;
        return EXIT_FAILURE;
    }
#endif
}

