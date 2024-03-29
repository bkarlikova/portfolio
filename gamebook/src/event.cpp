#include "event.h"


Event::Event(const json & block,  vector<shared_ptr<Item> > items, shared_ptr<Item> item)
    : State(block), m_task(block.at("task")), m_correct_answer(block.at("answer")), m_items(std::move(items)), m_secret_item(item)
    {
        transform(m_correct_answer.begin(), m_correct_answer.end(), m_correct_answer.begin(), ::tolower);
    }

void Event::run_state(User & user) {
    try{
        clear();
        cout << get_intro();
        sleep();
        cout << get_task();
        sleep();
        string user_input_str = get_string_from_user();
        if (eval_answer(user_input_str)) {
            sleep();
            cout << get_items_offer();
            sleep();
            int input = get_int_from_user();
            while (input <= 0 || input > (int)m_items.size()){
                cout << INVALID_INPUT;
                input = get_int_from_user();
            }
            cout << "You picked: " << pick_item(input-1)->build_string_of_item();
            user.add_item_into_inventory(pick_item(input-1));
        }
        else{
            cout << WRONG_ANSWER;
            cout << "correct answer is: " << m_correct_answer << endl;
        }
        get_secret_item(user);
        cout << "\n" + user.get_inventory().build_string_of_inventory() + "\n";
        cout << get_outro();
        sleep();
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

bool Event::eval_answer (const string & answer) const {
    return m_correct_answer == answer;
}

shared_ptr<Item> Event::pick_item(size_t idx) const {
    return m_items.at(idx);
}

string Event::get_task() {
    return "THE RIDDLE: " + m_task + "\n";
}

string Event::get_items_offer() {
    string output = "Congratulations! Your answer is correct. Pick your prize:\n";
    for(size_t i = 0; i < m_items.size(); i ++){
        output += "NO. " + to_string(i+1) + ": " + m_items.at(i)->build_string_of_item(); //polymorphisms here!!!
    }
    return output;
}

void Event::get_secret_item(User & user) const {
    int random = (rand() % 15) + 5;
    if (random <= user.get_attributes().at(3)->get_curr()){
        cout << "You're lucky, so you're getting and secret item:\n";
        cout << m_secret_item->build_string_of_item();
        user.add_item_into_inventory(m_secret_item);
    }
}


