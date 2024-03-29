#include "helperFunctions.h"


void check_input_for_menu(const string & input) {
    if(input == MENU[0] || input == MENU[1]){
        throw MenuException();
    }
}

bool validate_path_string(const string & input){
    if (input.empty())
        return false;
    for (char ch : input) {
       if (!isprint(ch) || ch == '/' || ch == ':' || ch == ' ')
           return false;
    }
    return true;
}

string get_string_from_user(const string &type) {
    while (true) {
        try {
            string input;
            cout << type;
            getline(cin, input);
            if (cin.eof()) {
                throw EOF_EXCEPTION;
            }
            transform(input.begin(), input.end(), input.begin(), ::tolower);
            check_input_for_menu(input);
            return input;
        }
        catch (const MenuException &ex) {
            throw MenuException();
        }
        catch (const CustomExceptions &ex) {
            throw ex;
        }
        catch (...) {
            cout << INVALID_INPUT;
        }
    }
}

int get_int_from_user(const string &type) {
    while(true){
        try{
            string input = get_string_from_user(type);
            for (char ch : input) {
                if (!isdigit(ch))
                    throw logic_error("not one integer");
            }
            int input_int = stoi(input);
            return input_int;
        }
        catch (const MenuException &ex){
            throw MenuException();
        }
        catch (const CustomExceptions &ex){
            throw ex;
        }
        catch(...){
            cout << INVALID_INPUT;
        }
    }
}
void sleep(){
    this_thread::sleep_for(200ms);
}

void sleep_long(){
    this_thread::sleep_for(2000ms);
}

void clear(){
    cout << "\033[2J" << "\033[H";
}

void wait_and_clear(){
    clear();
    sleep();
}


string cover_to_length(const string & input){
    string output = input;
    if (output.back() == '\n'){
        output.pop_back();
    }
    for (size_t i = output.size(); i < LENGTH_COVER; i++){
        output += " ";
    }
    return output;
}