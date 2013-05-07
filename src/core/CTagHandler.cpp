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
#include <boost/filesystem.hpp>
#include "CTagHandler.h"
#include "Debug.hpp"
#include "Helper.h"

namespace ctag
{

CTagHandler::CTagHandler()
{
}

CTagHandler::~CTagHandler()
{
}

/*
 * Initialize the database.
 */
bool CTagHandler::initDB(sqlite3* db)
{
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS "
            "tag(id INTEGER PRIMARY KEY, tagname TEXT, "
            "path TEXT, UNIQUE(tagname, path));";

    const char* createIndexSQL =
            "CREATE INDEX IF NOT EXISTS tag_idx ON tag (tagname, path);";

    sqlite3_stmt* statement;

    // Create table if not exists
    if (sqlite3_prepare_v2(db, createTableSQL, -1, &statement, 0) != SQLITE_OK)
        return false;

    // Execute
    sqlite3_step(statement);
    sqlite3_finalize(statement);

    // Create index if not exists
    if (sqlite3_prepare_v2(db, createIndexSQL, -1, &statement, 0) != SQLITE_OK)
        return false;

    // Execute
    sqlite3_step(statement);
    sqlite3_finalize(statement);

    return true;
}

/*
 * Method to process input from the command line.
 */
void CTagHandler::processInput(const std::vector<std::string>& argVec,
        const std::string& flag)
{
    // Open db connection
    sqlite3* database;

    // Home folder path + database
    const char* path = (std::string(getHomeFolder())
            + boost::filesystem::path("/").make_preferred().native() + ".ctag").c_str();

#ifdef DEBUG
printf("CTAG DB LOCATION: %s\n", path);
#endif

    // Check if database can be opened
    if (sqlite3_open(path, &database) != SQLITE_OK)
    {
        std::cout << "Could not connect to DB" << std::endl;
        return;
    }

    // Init db
    if (!initDB(database))
    {
        std::cout << "Could not initialize DB" << std::endl;
        return;
    }

    if (flag.compare("tag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: tag");
#endif

        // TODO: implement tag
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
