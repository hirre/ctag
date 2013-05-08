/*
 *  CTagHandler.cpp
 *
 *  Handler class that handles input from the command line.
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

#include <stdlib.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include "CTagHandler.h"
#include "Debug.hpp"
#include "Helper.h"

namespace ctag
{

CTagHandler::CTagHandler()
{
    database = NULL;

    // Home folder path + database
    std::stringstream ss;
    ss << getHomeFolder()
            << boost::filesystem::path("/").make_preferred().native()
            << ".ctag";
    path = new std::string(ss.str());

#ifdef DEBUG
    debug::dbgPrint("Home folder: " + *path);
#endif
}

CTagHandler::~CTagHandler()
{
    // Close if not closed
    if (database)
        sqlite3_close(database);

    // Delete path
    delete path;
}

/*
 * Initialize the database.
 */
bool CTagHandler::initDB()
{
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS "
            "tag(id INTEGER PRIMARY KEY, tagname TEXT, "
            "path TEXT, UNIQUE(tagname, path));";

    const char* createIndexSQL =
            "CREATE INDEX IF NOT EXISTS tag_idx ON tag (tagname, path);";

    sqlite3_stmt* statement;

    // Create table if not exists
    if (sqlite3_prepare_v2(database, createTableSQL, -1, &statement,
            0) != SQLITE_OK)
        return false;

    // Execute
    sqlite3_step(statement);
    sqlite3_finalize(statement);

    // Create index if not exists
    if (sqlite3_prepare_v2(database, createIndexSQL, -1, &statement,
            0) != SQLITE_OK)
        return false;

    // Execute
    sqlite3_step(statement);
    sqlite3_finalize(statement);

    return true;
}

/*
 * Tag method for tagging files/folders.
 */
bool CTagHandler::tag(const std::string& tagName, const std::string& f)
{
    // Path which is to be tagged
    boost::filesystem::path p(f);

    // Check if path exists
    if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
    {
        // TODO: check if path contains *

        sqlite3_stmt* statement;

        std::stringstream ss;
        ss << "INSERT INTO (tagname, path) VALUES('" << tagName << "', '" << f << "');";

        // Insert statement
        if (sqlite3_prepare_v2(database, ss.str().c_str(), -1, &statement,
                0) != SQLITE_OK)
        {
            std::cout << "Could not prepare statement!" << std::endl;
            return false;
        }

        // Execute
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }
    else
    {
        std::cout << "Path does not exist!" << std::endl;
        return false;
    }

    return true;
}

/*
 * Method to process input from the command line.
 */
void CTagHandler::processInput(const std::vector<std::string>& argVec,
        const std::string& flag)
{
    // Check if database can be opened
    if (sqlite3_open(path->c_str(), &database) != SQLITE_OK)
    {
        std::cout << "Could not connect or create DB in home folder!"
                << std::endl;
        return;
    }

    // Init db
    if (!initDB())
    {
        std::cout << "Could not initialize DB!" << std::endl;
        return;
    }

    if (flag.compare("tag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: tag");
#endif
        // Tag
        if (tag(argVec[0], argVec[1]))
            std::cout << "Created tag!" << std::endl;
    }
    else if (flag.compare("removetag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: removetag");
#endif

        // TODO: implement removetag
    }
    else if (flag.compare("showtag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: showtag");
#endif

        // TODO: implement showtag
    }

    // Close db
    sqlite3_close(database);
}

} /* namespace ctag */
