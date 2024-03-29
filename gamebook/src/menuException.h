/**
 * @file menuException.h
 * @brief Defines the MenuException class for handling menu exceptions.
 */

#pragma once
#include "customExceptions.h"

/**
 * @class MenuException
 * @brief Represents an exception related to menu input.
 *
 * The MenuException class is a derived class of CustomExceptions and is used for handling exceptions
 * related to menu input. It is specifically designed to be thrown when the user inputs 'm' or 'menu' as a command.
 */
class MenuException : public CustomExceptions {
public:
    /**
     * @brief Default constructor.
     */
    MenuException();

    /**
     * @brief Constructor with error message.
     * @param mess The error message associated with the exception.
     */
    explicit MenuException(string mess);
};
