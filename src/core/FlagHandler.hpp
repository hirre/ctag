/*
 *  FlagHandler.hpp
 *
 *  Handler class that handles input from the command line.
 *  Handles:
 *      - write (K,V)
 *      - delete (K,V)
 *      - print (K,V)
 *
 *
 *  Created on: 5 maj 2013
 *  Author: Hirad Asadi
 *  E-mail: hirad.asadi@gmail.com
 */

#ifndef FLAGHANDLER_HPP_
#define FLAGHANDLER_HPP_

#include <iostream>
#include <vector>
#include "sqlite3.h"
#include "../include/Flags.hpp"
#include "../include/Error.hpp"

namespace maptag
{

// Allow numbers, letters and "_"
#define REGEX_MAIN "^[a-zA-Z0-9_]*$"
// Allow numbers, letters, "_", and %
#define REGEX_MAIN_ALLOW_PERCENTAGE "^[a-zA-Z0-9_%]*$"

class FlagHandler
{
public:
    // Constructor
    FlagHandler();

    // Destructor
    virtual ~FlagHandler();

    // Method to process cmd line input
    bool processInput(const std::vector<std::string>& argVec, const Flag& flag,
            const std::vector<Flag>& extraFlags = std::vector<Flag>());

    // Error method
    Error getError();

private:
    // Database
    sqlite3* database_;

    // Path to database
    std::string * path_;

    // Error struct
    Error e_;

    // Print question
    bool q();

    // Initialize database
    bool initDB();

    // Print key-value
    void print_kv(sqlite3_stmt* statement);

    // Write key-value
    bool writeKV(const std::vector<std::string>& fVec,
            const std::vector<Flag>& extraFlags = std::vector<Flag>());

    // Delete key-value
    bool deleteKV(const std::vector<std::string>& fVec,
            const std::vector<Flag>& extraFlags = std::vector<Flag>());

    // Print key-value
    bool printKV(const std::vector<std::string>& fVec,
            const std::vector<Flag>& extraFlags = std::vector<Flag>());
};

} /* namespace maptag */
#endif /* FLAGHANDLER_HPP_ */
