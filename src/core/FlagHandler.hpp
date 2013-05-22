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
 *
 *  Copyright (C) 2013  Hirad Asadi
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
