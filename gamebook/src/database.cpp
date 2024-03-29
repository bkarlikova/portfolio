#include "database.h"


Database::Database(const string &file_attributes,
                     const string &file_effects,
                     const string &file_items,
                     const string &file_NPCs)
                     :
    m_attributes(load_attributes(file_attributes)),
    m_effects(load_effects(file_effects)),
    m_items(load_items(file_items)),
    m_NPSs(load_NPCs(file_NPCs))
    {}

vector<shared_ptr<Effect>> Database::get_effects_for_char(const vector<int> &idx) const {
    vector<shared_ptr<Effect>> output;
    try {
        for (const auto &i: idx) {
            if (m_effects.at(i)->is_permanent())
                output.emplace_back(m_effects.at(i));
            else{
                throw CustomExceptions("effect for character needs to be permanent");
            }
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

vector<shared_ptr<Item>> Database::get_items(const vector<int> &idx) const {
    vector<shared_ptr<Item>> output;
    try{
        for (const auto & i : idx){
            try {
                output.emplace_back(m_items.at(i));
            }
            catch (const exception &ex) {
                throw CustomExceptions("invalid item or index");
            }
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

vector<shared_ptr<Item>> Database::get_weapons(const vector<int> &idx) const {
    vector<shared_ptr<Item>> output;
    try {
        for (const auto &i: idx) {
            if (m_items.at(i)->is_weapon())
                output.emplace_back(m_items.at(i));
            else {
                throw CustomExceptions("item must be type weapon, but it is not");
            }
        }
        return output;
    }
    catch (const exception &ex) {
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...) {
        throw UNKNOWN_EXCEPTION;
    }
}

vector<shared_ptr<Item>> Database::get_armours(const vector<int> &idx) const {
    vector<shared_ptr<Item>> output;
    try{
        for (const auto & i : idx){
            if (m_items.at(i)->is_armour())
                output.emplace_back(m_items.at(i));
            else{
                throw CustomExceptions("item must be type armour, but it is not");
            }
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

shared_ptr<NonPlayingCharacter> Database::get_npc(int idx) const {
    try{
        return m_NPSs.at(idx);
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION_M(ex, "index " + to_string(idx) + " failed");
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

shared_ptr<Effect> Database::get_effect (int id) const {
    try{
        return m_effects.at(id);
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION_M(ex, "index " + to_string(id) + " failed");
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

shared_ptr<Item> Database::get_item (int idx) const {
    try{
        return m_items.at(idx);
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION_M(ex, "index " + to_string(idx) + " failed");
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

string Database::get_att_name(int att_id) const {
    try{
        if (att_id == ID_HEALTH)
            return string(NAME_HEALTH);
        if (att_id == ID_INVENTORY)
            return string(NAME_INV);
        else
            return m_attributes.at(att_id)->get_name();
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

json Database::load_json(const string &file_name) {
    json output;
    try {
        ifstream ifs(file_name);
        output = json::parse(ifs);
        cout << file_name << " . . . loaded" << endl;
        sleep();
        return output;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}
map<size_t,shared_ptr<Attribute>> Database::load_attributes(const string & file_name) const {
    map<size_t, shared_ptr<Attribute>> attributes;
    json data = load_json(file_name);
    if (!data.contains("attributes"))
        throw CustomExceptions("ERROR IN 'load_attributes': missing block ");
    if (data.at("attributes").size() != 4)
        throw CustomExceptions("ERROR IN 'load_attributes': there are not four attributes");
    try{
        int att_check = 0;
        for (const auto & elem : data.at("attributes")){
            att_check++;
            if (att_check != elem.at("id"))
                throw CustomExceptions("invalid attribute id");
            attributes[elem.at("id")] = make_shared<Attribute>(elem);
        }
        return attributes;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

map<size_t, shared_ptr<Effect>> Database::load_effects(const string & file_name) const {
    map<size_t, shared_ptr<Effect>> effects;
    set <size_t> id_check;
    json data = load_json(file_name);
    if (!data.contains("effects"))
        throw CustomExceptions("ERROR IN 'load_items': missing block ");
    try{
        for (const auto & elem : data.at("effects")){
            if (id_check.find(elem.at("id")) != id_check.end())
                throw CustomExceptions("already existing id");
            id_check.emplace(elem.at("id"));
            if (find(OPERATIONS.begin(), OPERATIONS.end(), elem.at("operation").get<string>().front()) == OPERATIONS.end()){
                throw CustomExceptions("non existing operation");
            }
            if (elem.at("type") == "permanent"){
                effects[elem.at("id")] = (make_shared<PermanentEffect>(elem, get_att_name(elem.at("attribute_id"))));
            }
            else if (elem.at("type") == "temporary"){
                if (elem.at("attribute_id") == ID_HEALTH){
                    throw CustomExceptions("effect to health has to be permanent");
                }
                effects[elem.at("id")] = (make_shared<TemporaryEffect>(elem, get_att_name(elem.at("attribute_id"))));
            }
            else
                throw CustomExceptions("invalid effect type");
        }
        return effects;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

map<size_t, shared_ptr<Item>> Database::load_items(const string & file_name) const {
    map<size_t, shared_ptr<Item> > items;
    set <size_t> id_check;
    json data = load_json(file_name);
    if (!data.contains("items"))
        throw CustomExceptions("ERROR IN 'load_items': missing block ");
    try{
        for (const auto & elem : data.at("items")){
            try{
                if (id_check.find(elem.at("id")) != id_check.end())
                    throw CustomExceptions("already existing id");
                id_check.emplace(elem.at("id"));
                if (elem.at("type") == "weapon"){
                    if (elem.at("HP") < 1)
                        throw CustomExceptions("not positive HP points for " + to_string(elem.at("name")));
                    items[elem.at("id")] = make_shared<Weapon>(elem);
                }
                else if (elem.at("type") == "armour"){
                    if (elem.at("antiHP") < 1)
                        throw CustomExceptions("not positive anti HP points " + to_string(elem.at("name")));
                    items[elem.at("id")] = make_shared<Armour>(elem);
                }
                else if (elem.at("type") == "object"){
                    items[elem.at("id")] = make_shared<Object>(elem, get_effect(elem.at("effect")));
                }
                else
                    throw CustomExceptions("invalid item type");
            }
            catch (const exception &ex) {
                throw CUSTOM_EXCEPTION_M(ex, "loading items failed");
            }
        }
        return items;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

map<size_t, shared_ptr<Attribute> > Database::load_attributes_for_character(const json & block) const {
    map<size_t, shared_ptr<Attribute>> attributes = Attribute::copy_attributes(m_attributes);
    if (!block.contains("at1")|| !block.contains("at2") || !block.contains("at3") || !block.contains("at4")){
        throw CustomExceptions("empty attribute");
    }
    for (const auto & att : block.items()){
        for (const auto & value : att.value()){
            if (value < 0)
                throw CustomExceptions("negative value for " + att.key());
        }
    }
    try{
        attributes.at(1)->set_curr_max(block.at("at1").at("curr"), block.at("at1").at("max"));
        attributes.at(2)->set_curr_max(block.at("at2").at("curr"), block.at("at2").at("max"));
        attributes.at(3)->set_curr_max(block.at("at3").at("curr"), block.at("at3").at("max"));
        attributes.at(4)->set_curr_max(block.at("at4").at("curr"), block.at("at4").at("max"));
        return attributes;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

map<size_t, shared_ptr<NonPlayingCharacter>> Database::load_NPCs(const string & file_name) const {
    json data = load_json(file_name);
    map<size_t, shared_ptr<NonPlayingCharacter>> NPCs;
    set <size_t> id_check;
    if (!data.contains("NPCs"))
        throw CustomExceptions("ERROR IN 'load_NPCs': missing block ");
    try{
        for (const auto & elem : data.at("NPCs")){
            try {
                if (id_check.find(elem.at("id")) != id_check.end())
                    throw CustomExceptions("already existing id");
                if (elem.at("capacity").get<size_t>() < elem.at("items").get<vector<int>>().size()){
                    throw CustomExceptions("more items than capacity");
                }
                id_check.emplace(elem.at("id"));
                NPCs[elem.at("id")] = make_shared<NonPlayingCharacter>(elem,
                                                                        load_attributes_for_character(elem),
                                                                        get_effects_for_char(elem.at("effects").get<vector<int>>()),
                                                                        get_items(elem.at("items").get<vector<int>>()),
                                                                        get_item(elem.at("weapon")),
                                                                        get_item(elem.at("armour")),
                                                                        elem.at("capacity").get<int>());
            }
            catch (const exception & ex) {
                CUSTOM_EXCEPTION_M(ex, "loading NPCs failed");
            }
        }
        return NPCs;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

UserInit Database::load_user_initializer(const string & file_name) const {
    json data = load_json(file_name);
    vector<User> users_offer;
    if (!data.contains("characters") || !data.contains("items") || !data.contains("weapons") || !data.contains("armours"))
        throw CustomExceptions("ERROR IN 'load_user_initializer': missing block ");
    try{
        for (const auto & elem : data.at("characters")) {
            try {
                users_offer.emplace_back(User(elem,
                                               load_attributes_for_character(elem),
                                               get_effects_for_char(elem.at("effects").get<vector<int>>())));
            }
            catch (const exception & ex) {
                throw CustomExceptions("loading characters failure: ", ex);
            }
        }
        try{
            UserInit output = UserInit(users_offer,
                                         get_weapons(data.at("weapons")),
                                         get_armours(data.at("armours")),
                                         get_items(data.at("items")),
                                         data.at("units"));
            return output;
        }
        catch(const exception &ex){
            throw CUSTOM_EXCEPTION_M(ex, "loading initializer failed");
        }
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}


Automaton Database::load_automaton(const string & file_name) {
    json data = load_json(file_name);
    if (data.is_null())
        throw CustomExceptions("ERROR IN 'load_automaton': missing block ");
    map<size_t, shared_ptr<State> > states;
    set<string> alphabet;
    map<size_t, map<string, size_t> > transitions;
    map<size_t, shared_ptr<State> > final_states;
    set<size_t> id_check;
    set<size_t> check_trans_to;
    set<int> visited_states = {};
    try{
        for (const auto &state : data.at("states")) {
            if (id_check.find(state.at("id")) != id_check.end())
                throw CustomExceptions("already existing id");
            id_check.emplace(state.at("id"));
            if (state.at("type") == "event") {
                states[state.at("id")] = make_shared<Event>(state, get_items(state.at("items")), get_item(state.at("secItem")));
            } else if (state.at("type") == "fight") {
                states[state.at("id")] = make_shared<Fight>(state, get_npc(state.at("enemy"))->copy());
            } else
                throw CustomExceptions("invalid state type");
        }
    }
    catch (const CustomExceptions & ex){
        throw CUSTOM_EXCEPTION_M(ex, "loading states failed");
    }
    try{
        for (const auto &str: data.at("alphabet"))
            alphabet.insert(str);
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION_M(ex, "loading alphabet failed");
    }
    try{
        for (const auto &elem: data.at("transitions")) {
            if (id_check.find(elem.at("id")) == id_check.end())
                throw CustomExceptions("non existing id");
            set<size_t> check_trans;
            for (const auto &tr: elem.at("tr").items()){
                if (check_trans.find(tr.value()) != check_trans.end())
                    throw CustomExceptions("transition to state, that was already initialized");
                check_trans.emplace(tr.value());
                if (elem.at("id") == tr.value())
                    throw CustomExceptions("transition to same state: " + to_string(elem.at("id")) + " to " + to_string(tr.value()));
                if (tr.value() == data.at("initState"))
                    throw CustomExceptions("transition to init state: " + to_string(elem.at("id")) + " to " + to_string(tr.value()));
                if (id_check.find(tr.value()) == id_check.end())
                    throw CustomExceptions("transition to non existing state");
                string tmp_key = tr.key();
                transform(tmp_key.begin(), tmp_key.end(),tmp_key.begin(), ::tolower);
                transitions[elem.at("id")].emplace(tmp_key, tr.value());
                check_trans_to.emplace(tr.value());
            }
        }
        for (const auto & id : id_check){
            if (transitions.find(id) == transitions.end()){
                throw CustomExceptions("missing transition from state " + to_string(id));
            }
            if (check_trans_to.find(id) == check_trans_to.end() && id != data.at("initState")){
                throw CustomExceptions("missing transition to state " + to_string(id));
            }
        }
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION_M(ex, "loading transitions failed");
    }
    try{
        if (data.at("finalState").is_null())
            throw CustomExceptions("missing final state(s)");
        for (const auto &state: data.at("finalState")) {
            final_states[state] = states.at(state);
        }
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION_M(ex, "loading final states failed");
    }
    try{
        if(data.contains("visitedStates")){
            for (const auto &state: data.at("visitedStates")) {
                visited_states.emplace(state);
            }
        }
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION_M(ex, "loading visited states failed");
    }
    try{
        if (data.contains("currentState"))
            return {states, alphabet, transitions, final_states, states.at(data.at("currentState")), visited_states};
        else
            return {states, alphabet, transitions, final_states, states.at(data.at("initState")), visited_states};

    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}


User Database::load_user(const std::string &file_name) const {
    json data = load_json(file_name);
    try{
        return User(data,
                    load_attributes_for_character(data),
                    get_effects_for_char(data.at("effects").get<vector<int>>()),
                    get_items(data.at("items").get<vector<int>>()),
                    get_item(data.at("weapon")),
                    get_item(data.at("armour")),
                    data.at("capacity").get<int>());
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}


json Database::get_automaton(const std::string &file_name) {
    json automaton;
    try {
        ifstream ifs(file_name);
        automaton = json::parse(ifs);
        return automaton;
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}