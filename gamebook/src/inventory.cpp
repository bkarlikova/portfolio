#include "inventory.h"

Inventory::Inventory(int max_capacity) : m_capacity(Attribute(ID_INVENTORY, string(NAME_INV), 0, max_capacity)){}

Inventory::Inventory(shared_ptr<Item> weapon,
                       shared_ptr<Item> armour,
                       const Attribute& capacity,
                       vector<shared_ptr<Item>> items)
                       :
                       m_weapon(std::move(weapon)),
                       m_armour(std::move(armour)),
                       m_capacity(capacity),
                       m_items(std::move(items)){
    if (m_capacity.get_max() < (int)m_items.size())
        throw CustomExceptions("ERROR in 'Inventory ctor': capacity is smaller than vector of items");
}

bool Inventory::is_full() const {
    return m_items.size() >= (size_t)m_capacity.get_max();
}
void Inventory::add_to_items(const shared_ptr<Item> &item) {
    m_items.emplace_back(item);
    m_capacity.update_attribute('+', 1);
}

void Inventory::set_weapon(const shared_ptr<Item> &weapon) {
    if (!weapon->is_weapon()){
        cout << INVALID_TYPE;
    }
    m_weapon = weapon;
}

void Inventory::set_armour(const shared_ptr<Item> &armour) {
    if (!armour->is_armour()){
        cout << INVALID_TYPE;
    }
    m_armour = armour;
}

void Inventory::set_items(const vector<shared_ptr<Item>> &items) {
    for (const auto& item : items) {
        m_items.emplace_back(item);
    }
    m_capacity.update_attribute('+', items.size());
}

void Inventory::delete_item(size_t index) {
    if (index < m_items.size()){
        m_items.erase(next(m_items.begin(), index));
        m_capacity.update_attribute('-', 1);
    }
    else{
        cout << "Your index is out of range of your inventory. Try is again." << endl;
    }
}

void Inventory::delete_item(const shared_ptr<Item> &item) {
    for (size_t i = 0; i < m_items.size(); i ++) {
        if (m_items.at(i) == item){
            m_items.erase(m_items.begin() + i);
            m_capacity.update_attribute('-', 1);
        }
    }
}


string Inventory::build_string_of_items () const {
    string output;
    for (size_t i = 0; i < (size_t)m_capacity.get_max(); i++){
        if (i < m_items.size())
            output += to_string(i+1) +  " : " + m_items.at(i)->build_string_of_item();
        else{
            output += to_string(i+1) +  " : ---empty--- \n";
        }
    }
    return output;
}

string Inventory::build_string_of_items (const vector<shared_ptr<Item>> & items) const {
    string output;
    for (size_t i = 0; i < items.size(); i++){
        output += to_string(i+1) +  " : " + items.at(i)->build_string_of_item();
    }
    return output;
}

shared_ptr<Item> Inventory::get_weapon() const {
    return m_weapon;
}

shared_ptr<Item> Inventory::get_armour() const {
    return m_armour;
}

shared_ptr<Item> Inventory::get_item(size_t idx) const {
    try{
        return m_items.at(idx);
    }
    catch (const exception & ex){
        throw CUSTOM_EXCEPTION(ex);
    }
    catch (...){
        throw UNKNOWN_EXCEPTION;
    }
}

shared_ptr<Item> Inventory::get_item_by_id(size_t id) const {
    for (size_t i = 0; i < m_items.size(); i ++) {
        if (m_items.at(i)->get_id() == id){
            return m_items.at(i);
        }
    }
    return nullptr;
}

string Inventory::build_string_of_armour() const {
    if (m_armour == nullptr)
        return "ARMOUR: " + string(NONE) + "\n";
    return "ARMOUR: " + m_armour->build_string_of_item();
}

string Inventory::build_string_of_weapon() const {
    if (m_weapon == nullptr)
        return "WEAPON: " + string(NONE) + "\n";
    return "WEAPON: " + m_weapon->build_string_of_item();
}

string Inventory::build_string_of_inventory() const {
    string output = "\n__ INVENTORY __\n" + build_string_of_weapon() + build_string_of_armour();
    output += "capacity: " + to_string(m_capacity.get_max()) + "\n";
    output +=  build_string_of_items();
    return output;
}

Attribute Inventory::get_capacity() const {
    return m_capacity;
}

Attribute& Inventory::modify_capacity()  {
    return m_capacity;
}
vector<shared_ptr<Item>> Inventory::get_items() const {
    return m_items;
}

bool Inventory::have_armour_in_items() const {
    for (const auto & item : m_items){
        if (item->is_armour())
            return true;
    }
    return false;
}

bool Inventory::have_weapon_in_items() const {
    for (const auto & item : m_items){
        if (item->is_weapon())
            return true;
    }
    return false;
}

bool Inventory::have_object_in_items() const {
    for (const auto & item : m_items){
        if (item->is_object())
            return true;
    }
    return false;
}

void Inventory::change_armour(const shared_ptr<Item> &item) {
    m_items.emplace_back(m_armour);
    m_armour = item;
    delete_item(item);
}

void Inventory::change_weapon(const shared_ptr<Item> &item) {
    m_items.emplace_back(m_weapon);
    m_weapon = item;
    delete_item(item);
}

vector<shared_ptr<Item>> Inventory::get_armours() const {
    vector<shared_ptr<Item>> output;
    for (const auto & item : m_items){
        if (item->is_armour())
            output.emplace_back(item);
    }
    return output;
}

vector<shared_ptr<Item>> Inventory::get_weapons() const {
    vector<shared_ptr<Item>> output;
    for (const auto & item : m_items){
        if (item->is_weapon())
            output.emplace_back(item);
    }
    return output;
}

vector<shared_ptr<Item>> Inventory::get_objects() const {
    vector<shared_ptr<Item>> output;
    for (const auto & item : m_items){
        if (item->is_object())
            output.emplace_back(item);
    }
    return output;
}

