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

/*
 * Main constructor.
 */
CTagHandler::CTagHandler()
{
    // Init db to null
    database = NULL;

    // Home folder path + database
    std::stringstream ss;
    ss << getHomeFolder()
            << boost::filesystem::path("/").make_preferred().native()
            << ".ctag";
    path = new std::string(ss.str());

#ifdef DEBUG
    debug::dbgPrint("Database: " + *path);
#endif
}

/*
 * Destructor.
 */
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
            "path TEXT, dt TEXT, UNIQUE(tagname, path));";

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
bool CTagHandler::tag(const std::vector<std::string>& fVec)
{
    // Tag name
    std::string tagName = fVec[0];

    // Go through path(s)
    for (unsigned int i = 1; i < fVec.size(); i++)
    {
        // File/folder string
        std::string f = fVec[i];

        // Path which is to be tagged
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;

            std::stringstream ss;
            ss << "INSERT INTO tag (tagname, path, dt) VALUES('" << tagName
                    << "', '" << boost::filesystem::canonical(p).string()
                    << "', datetime('now'));";

            // Insert statement
            if (sqlite3_prepare_v2(database, ss.str().c_str(), -1, &statement,
                    0) != SQLITE_OK)
            {
                std::cerr << "Could not prepare statement [" << ss.str() << "]!"
                        << std::endl;
                return false;
            }

            // Execute
            sqlite3_step(statement);
            sqlite3_finalize(statement);
        }
        else
        {
            std::cerr << "Path (" << p.string() << ") does not exist!"
                    << std::endl;
            return false;
        }
    }

    return true;
}

/*
 * Method to remove tag(s)
 */
bool CTagHandler::removeTag(const std::vector<std::string>& fVec)
{
    // TODO: implement removetag

    return true;
}

/*
 * Method to show tag(s).
 */
bool CTagHandler::showTag(const std::vector<std::string>& fVec)
{
    // Tag name
    std::string tagName = fVec[0];
    bool foundTag = false;

    // Go through path(s)
    for (unsigned int i = 1; i < fVec.size(); i++)
    {
        // File/folder string
        std::string f = fVec[i];

        // Path which is to be tagged
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;
            std::stringstream ss;

            // Select all
            if (tagName.compare("*") == 0)
                ss << "SELECT * FROM tag WHERE path = '"
                        << boost::filesystem::canonical(p).string() << "'";
            else
                // Select specific tag
                ss << "SELECT * FROM tag WHERE tagname = '" << tagName
                        << "' AND path = '"
                        << boost::filesystem::canonical(p).string() << "'";

            // Select statement
            if (sqlite3_prepare_v2(database, ss.str().c_str(), -1, &statement,
                    0) != SQLITE_OK)
            {
                std::cerr << "Could not prepare statement [" << ss.str() << "]!"
                        << std::endl;
                return false;
            }
            else // OK Select
            {
                int result = 0;

                while (true)
                {
                    // Execute
                    result = sqlite3_step(statement);

                    // Found row
                    if (result == SQLITE_ROW)
                    {
                        foundTag = true;

                        // Tag name
                        std::string tag = (char*) sqlite3_column_text(statement,
                                1);

                        // Path
                        std::string pathStr = (char*) sqlite3_column_text(
                                statement, 2);

                        // Datetime
                        std::string dt = (char*) sqlite3_column_text(statement,
                                3);

                        // Print found tag
                        std::cout << "#" << tag << "\t" << pathStr << "\t["
                                << dt << "]" << std::endl;
                    }
                    else
                    {
                        break;
                    }
                }

                // Finalize
                sqlite3_finalize(statement);
            }
        }
        else
        {
            std::cerr << "Path (" << p.string() << ") does not exist!"
                    << std::endl;
            return false;
        }
    }

    // Tag has not been found
    if (!foundTag)
        return false;

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
        std::cerr << "Could not connect or create DB in home folder!"
                << std::endl;
        return;
    }

    // Init db
    if (!initDB())
    {
        std::cerr << "Could not initialize DB!" << std::endl;
        return;
    }

    if (flag.compare("tag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: tag");
#endif
        // Tag
        if (tag(argVec))
            std::cout << "Tagged!" << std::endl;
    }
    else if (flag.compare("removetag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: removetag");
#endif
        if (removeTag(argVec))
            std::cout << "Removed tag(s)." << std::endl;
    }
    else if (flag.compare("showtag") == 0)
    {
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: showtag");
#endif

        // Show tag
        if (!showTag(argVec))
            std::cerr << "Tag(s) not found." << std::endl;
    }

    // Close db
    sqlite3_close(database);
}

} /* namespace ctag */
