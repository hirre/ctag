/*
 *  CTagHandler.hpp
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

#ifndef CTAGHANDLER_HPP_
#define CTAGHANDLER_HPP_

// Allow numbers, letters and "_"
#define REGEX_MAIN "^[a-zA-Z0-9_]*$"
// Allow numbers, letters, "_", quotes, % and "#"
#define REGEX_SHOW "^[a-zA-Z0-9_\"#%]*$"

#include <iostream>
#include <vector>
#include "sqlite3.h"
#include "Flags.hpp"

namespace ctag
{

class CTagHandler
{
public:
    // Constructor
    CTagHandler();
    // Destructor
    virtual ~CTagHandler();
    // Method to process cmd line input
    bool processInput(const std::vector<std::string>& argVec,
            const Flag& flag);
private:
    // Database
    sqlite3* database;
    // Path to database
    std::string * path;
    // Initialize database
    bool initDB();
    // Tag method
    bool tag(const std::vector<std::string>& fVec);
    // Show tag method
    bool showTag(const std::vector<std::string>& fVec);
    // Remove tag method
    bool removeTag(const std::vector<std::string>& fVec);
};

} /* namespace ctag */
#endif /* CTAGHANDLER_HPP_ */
