#include "menuException.h"

MenuException::MenuException() : CustomExceptions("call for menu") {}

MenuException::MenuException(string mess) : CustomExceptions(std::move(mess)) {}