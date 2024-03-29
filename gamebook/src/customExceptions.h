/**
 * @file customException.h
 * @brief Defines the CCustomException class and its member functions.
 */
#pragma once
#include <iostream>
#include <string>
#include <exception>

/**
 * @def UNKNOWN_EXCEPTION
 * @brief Macro to throw an unknown error exception.
 *
 * This macro throws an unknown error exception with the error message containing the name of the function where the exception occurred.
 * It should be used when an unknown error condition is encountered.
 */
#define UNKNOWN_EXCEPTION CustomExceptions::throw_unknown_error(__FUNCTION__)

/**
 * @def CUSTOM_EXCEPTION(EX)
 * @brief Macro to throw a custom exception.
 * @param EX The nested exception.
 *
 * This macro throws a custom exception with the nested exception and the error message containing the name of the function
 * where the exception occurred. It should be used to throw custom exceptions with a nested exception.
 */
#define CUSTOM_EXCEPTION(EX) CustomExceptions::throw_exception(__FUNCTION__, EX)
/**
 * @def CUSTOM_EXCEPTION_M(EX, MESS)
 * @brief Macro to throw a custom exception with a custom error message.
 * @param EX The nested exception.
 * @param MESS The custom error message.
 *
 * This macro throws a custom exception with the nested exception and a custom error message containing the name of the function where the exception occurred.
 * It should be used to throw custom exceptions with a nested exception and a custom error message.
 */
#define CUSTOM_EXCEPTION_M(EX, MESS) CustomExceptions::throw_exception(__FUNCTION__, EX, MESS)
/**
 * @def EOF_EXCEPTION
 * @brief Macro to throw an end-of-file exception.
 *
 * This macro throws an end-of-file exception with the error message indicating an unexpected end of file.
 * It should be used when an unexpected end-of-file condition is encountered.
 */
#define EOF_EXCEPTION CustomExceptions("Unexpected EOF")

using namespace std;

/**
 * @class CustomExceptions
 * @brief Custom exception class for handling custom exception and error messages.
 *
 * This class provides a custom exception mechanism for handling custom error messages.
 * It inherits from the standard exception class.
 */
class CustomExceptions : public exception {
public:
    /**
     * @brief Constructor with an error message.
     * @param mess The error message associated with the exception.
     */
    CustomExceptions(string mess);
    /**
     * @brief Constructor with an error message and a nested exception.
     * @param mess The error message associated with the exception.
     * @param ex The nested exception.
     */
    CustomExceptions(string mess, const exception& ex);
    /**
     * @brief Constructor with a nested exception.
     * @param ex The nested exception.
     */
    CustomExceptions(const exception& ex);
    /**
     * @brief Throws an unknown error exception with the given message.
     * @param mess The error message.
     * @return A CustomExceptions object representing the unknown error.
     */
    static CustomExceptions throw_unknown_error(const string &mess);
    /**
     * @brief Throws an exception with the given nested exception.
     * @param func The name of the function where the exception occurred.
     * @param ex The nested exception.
     * @return A CustomExceptions object representing the thrown exception.
     */
    static CustomExceptions throw_exception(const string &func, const exception &ex);
    /**
     * @brief Throws an exception with the given nested exception and message.
     * @param func The name of the function where the exception occurred.
     * @param ex The nested exception.
     * @param mess The error message.
     * @return A CustomExceptions object representing the thrown exception.
     */
    static CustomExceptions throw_exception(const string &func, const exception &ex, string mess);
    /**
     * @brief Returns the error message associated with the exception.
     * @return The error message as a C-style string.
     */
    const char* what() const noexcept override;
private:
    /**
     * @brief The error message associated with the exception in specific function.
     */
    string m_error_message = "ERROR IN ";
};




