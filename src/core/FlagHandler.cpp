/*
 *  FlagHandler.cpp
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

#include <stdlib.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include "FlagHandler.hpp"
#include "Debug.hpp"
#include "Helper.hpp"

using namespace std;

namespace maptag
{

/*
 * Main constructor.
 */
FlagHandler::FlagHandler()
{
    // Init db to null
    database_ = NULL;

    // No error per default
    e_.err = NO_ERROR;
    e_.msg = "";

    // Home folder path + database
    stringstream ss;
    ss << getHomeFolder() << PATH_SEPARATOR << ".maptag";
    path_ = new string(ss.str());

#ifdef DEBUG
    debug::dbgPrint("Database: " + *path_);
#endif
}

/*
 * Destructor.
 */
FlagHandler::~FlagHandler()
{
    // Close if not closed
    if (database_)
        sqlite3_close(database_);

    // Delete path
    delete path_;
}

/*
 * Initialize the database. Creates the needed tables if they do not exist.
 */
bool FlagHandler::initDB()
{
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS "
            "tag(id INTEGER PRIMARY KEY, tagname TEXT COLLATE NOCASE, "
            "path TEXT COLLATE NOCASE, dt TEXT, UNIQUE(tagname, path));";

    // TODO: create key-value table

    const char* createIndexSQL =
            "CREATE INDEX IF NOT EXISTS tag_idx ON tag (tagname, path);";

    sqlite3_stmt* statement;

    // Create table if not exists
    if (sqlite3_prepare_v2(database_, createTableSQL, -1, &statement,
            0) != SQLITE_OK)
        return false;

    // Execute
    sqlite3_step(statement);

    // Finalize
    sqlite3_finalize(statement);

    // Create index if not exists
    if (sqlite3_prepare_v2(database_, createIndexSQL, -1, &statement,
            0) != SQLITE_OK)
        return false;

    // Execute
    sqlite3_step(statement);

    // Finalize
    sqlite3_finalize(statement);

    return true;
}

/*
 * Method for tagging files/folders. Receives arguments as an input vector.
 */
bool FlagHandler::tag(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // Tag name
    string tagName = fVec[0];

    // Verify tag name
    if (!verifyInput(tagName, REGEX_MAIN))
    {
        e_.err = VERIFICATION_ERROR;
        e_.msg = "Tag name can only contain numbers, letters and \"_\"";
        return false;
    }

    // Go through path(s)
    for (unsigned int i = 1; i < fVec.size(); i++)
    {
        // File/folder string
        string f = fVec[i];

        // Path which is to be tagged
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;

            stringstream ss;
            ss << "INSERT INTO tag (tagname, path, dt) VALUES('" << tagName
                    << "', '" << boost::filesystem::canonical(p).string()
                    << "', datetime('now'));";

            // Insert statement
            if (sqlite3_prepare_v2(database_, ss.str().c_str(), -1, &statement,
                    0) != SQLITE_OK)
            {
                e_.err = STATEMENT_PREPARATION_ERROR;
                e_.msg = string("Could not prepare statement [") + ss.str()
                        + string("]");
                return false;
            }

            // Execute
            sqlite3_step(statement);

            // Finalize
            sqlite3_finalize(statement);
        }
        else
        {
            e_.err = PATH_DOES_NOT_EXIST_ERROR;
            e_.msg = string("Path (") + p.string()
                    + string(") does not exist");
            return false;
        }
    } // FOR

    return true;
}

/*
 * Method to remove tag(s). Receives arguments as an input vector.
 */
bool FlagHandler::removeTag(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // "All" flag set
    bool all = find(extraFlags.begin(), extraFlags.end(), ALL)
            != extraFlags.end();

    // Tag name
    string tagName = (all) ? "" : fVec[0];

    // Verify tag name
    if (!tagName.empty() && !verifyInput(tagName, REGEX_MAIN_ALLOW_PERCENTAGE))
    {
        e_.err = VERIFICATION_ERROR;
        e_.msg =
                "Tag name can only contain numbers, letters, \"_\" and % (wildcard) for missing characters";
        return false;
    }

    // Removed rows
    bool removedRows = false;

    unsigned int i = (all) ? 0 : 1;

    // Go through path(s)
    for (; i < fVec.size(); i++)
    {
        // File/folder string
        string f = fVec[i];

        // Path which is to be tagged
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;

            stringstream ss;

            // Delete all tags
            if (all)
                ss << "DELETE FROM tag WHERE path = '"
                        << boost::filesystem::canonical(p).string() << "';";
            else
                // Delete specific tags from file(s)/folder(s)
                ss << "DELETE FROM tag WHERE tagname LIKE '" << tagName
                        << "' AND path = '"
                        << boost::filesystem::canonical(p).string() << "';";

            // Delete statement
            if (sqlite3_prepare_v2(database_, ss.str().c_str(), -1, &statement,
                    0) != SQLITE_OK)
            {
                e_.err = STATEMENT_PREPARATION_ERROR;
                e_.msg = string("Could not prepare statement [") + ss.str()
                        + string("]");
                return false;
            }
            else
            {
                // Execute
                sqlite3_step(statement);

                if (sqlite3_changes(database_) != 0)
                    removedRows = true;

                // Finalize
                sqlite3_finalize(statement);
            }
        }
        else
        {
            e_.err = PATH_DOES_NOT_EXIST_ERROR;
            e_.msg = string("Path (") + p.string()
                    + string(") does not exist");
            return false;
        }
    } // FOR

    return (removedRows) ? true : false;
}

/*
 * Method to show tag(s). Receives arguments as an input vector.
 */
bool FlagHandler::showTag(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // "All" flag set
    bool all = find(extraFlags.begin(), extraFlags.end(), ALL)
            != extraFlags.end();

    // Tag name
    string tagName = (all) ? "" : fVec[0];
    bool foundTag = false;

    // Verify tag name
    if (!tagName.empty() && !verifyInput(tagName, REGEX_MAIN_ALLOW_PERCENTAGE))
    {
        e_.err = VERIFICATION_ERROR;
        e_.msg =
                "Tag name can only contain numbers, letters, \"_\" and % (wildcard) for missing characters";
        return false;
    }

    unsigned int i = (all) ? 0 : 1;

    // Go through path(s)
    for (; i < fVec.size(); i++)
    {
        // File/folder string
        string f = fVec[i];

        // Path which is to be shown
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;
            stringstream ss;

            // Select all
            if (all)
                ss << "SELECT * FROM tag WHERE path = '"
                        << boost::filesystem::canonical(p).string()
                        << "' ORDER BY dt COLLATE NOCASE DESC";
            else
                // Select specific tag
                ss << "SELECT * FROM tag WHERE tagname LIKE '" << tagName
                        << "' AND path = '"
                        << boost::filesystem::canonical(p).string()
                        << "' ORDER BY dt COLLATE NOCASE DESC";

            // Select statement
            if (sqlite3_prepare_v2(database_, ss.str().c_str(), -1, &statement,
                    0) != SQLITE_OK)
            {
                e_.err = STATEMENT_PREPARATION_ERROR;
                e_.msg = string("Could not prepare statement [") + ss.str()
                        + string("]");
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
                        string tag = (char*) sqlite3_column_text(statement,
                                1);

                        // Path
                        string pathStr = (char*) sqlite3_column_text(
                                statement, 2);

                        // Datetime
                        string dt = (char*) sqlite3_column_text(statement,
                                3);

                        // Print found tag(s)
                        if (all
                                || tagName.find_first_of("%")
                                        != string::npos)
                            cout << "#" << tag << "\t" << pathStr << "\t["
                                    << dt << "]" << endl;
                        else
                            // Print path(s) found for specific tag
                            cout << pathStr << "\t[" << dt << "]"
                                    << endl;
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
            e_.err = PATH_DOES_NOT_EXIST_ERROR;
            e_.msg = string("Path (") + p.string()
                    + string(") does not exist");
            return false;
        }
    } // FOR

    return (!foundTag) ? false : true;
}

/*
 * Method to process input from the command line. Returns true on success.
 */
bool FlagHandler::processInput(const vector<string>& argVec,
        const Flag& flag, const vector<Flag>& extraFlags)
{
    // Check if database can be opened
    if (sqlite3_open(path_->c_str(), &database_) != SQLITE_OK)
    {
        e_.err = DB_CONNECTION_OR_CREATION_ERROR;
        e_.msg = "Could not connect or create DB in home folder";
        return false;
    }

    // Init db
    if (!initDB())
    {
        e_.err = DB_CONNECTION_OR_CREATION_ERROR;
        e_.msg = "Could not initialize DB";
        return false;
    }

    bool error = false;

    switch (flag)
    {
    case TAG:
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: tag");
#endif
        // Tag
        if (!tag(argVec, extraFlags))
            error = true;
        break;

    case REMOVE_TAG:
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: remove tag");
#endif
        // Remove tag
        if (!removeTag(argVec, extraFlags))
            error = true;

        break;

    case SHOW_TAG:
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: show tag");
#endif
        // Show tag
        if (!showTag(argVec, extraFlags))
            error = true;

        break;

    case WRITE_KEY_VALUE:
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: write key-value");
#endif
        // Write key-value
        if (!writeKV(argVec, extraFlags))
            error = true;
        break;

    case DELETE_KEY_VALUE:
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: delete key-value");
#endif
        // Delete key-value
        if (!deleteKV(argVec, extraFlags))
            error = true;
        break;

    case PRINT_KEY_VALUE:
#ifdef DEBUG
        debug::dbgPrint("PROCESSING: print key-value");
#endif
        // Print key-value
        if (!printKV(argVec, extraFlags))
            error = true;

        break;
        // Ignore

    case ALL:
        break;
    }

    // Close db
    sqlite3_close(database_);

    return !error;
}

/*
 * This method writes a key-value to the database associated with file(s)/folder(s).
 */
bool FlagHandler::writeKV(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // TODO: implement writeKV()
    return true;
}

/*
 * This method deletes a key-value in the database associated with file(s)/folder(s).
 */bool FlagHandler::deleteKV(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // TODO: implement deleteKV()
    return true;
}

/*
 * This method prints key-values associated with file(s)/folder(s).
 */
bool FlagHandler::printKV(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // TODO: implement printKV()
    return true;
}

/*
 * Get error, if an error occurred.
 */
Error FlagHandler::getError()
{
    return e_;
}

} /* namespace maptag */
