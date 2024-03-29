#include "customExceptions.h"

CustomExceptions::CustomExceptions(string mess) {
    m_error_message += std::move(mess);
}

CustomExceptions::CustomExceptions(const std::exception &ex) : m_error_message(ex.what()){}

CustomExceptions::CustomExceptions(string mess, const exception &ex) : m_error_message(mess + " " + ex.what()){}

CustomExceptions CustomExceptions::throw_unknown_error(const std::string &mess) {
    return {"UNKNOWN ERROR in '" + mess + "' "};
}

CustomExceptions CustomExceptions::throw_exception(const std::string &func, const exception &ex) {
    return CustomExceptions("'" + func + "' -> " , ex);
}

CustomExceptions CustomExceptions::throw_exception(const std::string &func, const exception &ex, string mess) {
    return CustomExceptions("'" + func + "' : " + mess + " -> ", ex);
}

const char *CustomExceptions::what() const noexcept {
    return m_error_message.c_str();
}


