/*
 *  FlagHandler.hpp
 *
 *  Handler class that handles input from the command line.
 *  Handles:
 *      -tag
 *      -removetag
 *      -showtag
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

// Allow numbers, letters and "_"
#define REGEX_TAG "^[a-zA-Z0-9_]*$"
// Allow numbers, letters, "_", and %
#define REGEX_SHOW_REMOVE "^[a-zA-Z0-9_%]*$"

#include <iostream>
#include <vector>
#include "sqlite3.h"
#include "Flags.hpp"

namespace maptag
{

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
private:
    // Database
    sqlite3* database_;
    // Path to database
    std::string * path_;
    // Initialize database
    bool initDB();
    // Tag method
    bool tag(const std::vector<std::string>& fVec,
            const std::vector<Flag>& extraFlags = std::vector<Flag>());
    // Show tag method
    bool showTag(const std::vector<std::string>& fVec,
            const std::vector<Flag>& extraFlags = std::vector<Flag>());
    // Remove tag method
    bool removeTag(const std::vector<std::string>& fVec,
            const std::vector<Flag>& extraFlags = std::vector<Flag>());
};

} /* namespace maptag */
#endif /* FLAGHANDLER_HPP_ */
