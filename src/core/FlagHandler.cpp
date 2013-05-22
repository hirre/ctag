/*
 *  FlagHandler.cpp
 *
 *  Handler class that handles input from the command line.
 *  Handles:
 *      - write (K,V)
 *      - delete (K,V)
 *      - print (K,V)
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
            "key_val(id INTEGER PRIMARY KEY, "
            "key TEXT COLLATE NOCASE, "
            "val TEXT COLLATE NOCASE, "
            "path TEXT COLLATE NOCASE, "
            "dt TEXT, UNIQUE(key, path));";

    const char* createIndexSQL =
            "CREATE INDEX IF NOT EXISTS key_val_idx ON key_val (key, path);";

    sqlite3_stmt* statement;

    // Prepare to create table tag if not exists
    if (sqlite3_prepare_v2(database_, createTableSQL, -1, &statement,
            0) != SQLITE_OK)
        return false;

    // Execute
    sqlite3_step(statement);

    // Prepare to create index tag if not exists
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
 * Method to process input from the command line. Returns true on success.
 */
bool FlagHandler::processInput(const vector<string>& argVec, const Flag& flag,
        const vector<Flag>& extraFlags)
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
    // Key
    string key = fVec[0];

    // Value
    string value = fVec[1];

    // Verify key string
    if (!verifyInput(key, REGEX_MAIN))
    {
        e_.err = VERIFICATION_ERROR;
        e_.msg = "Key can only contain numbers, letters and \"_\"";
        return false;
    }

    unsigned int i = 2;

    // Value empty
    if (fVec.size() == 2)
    {
        i = 1;
        value = "";
    }

    // Go through path(s)
    for (; i < fVec.size(); i++)
    {
        // File/folder string
        string f = fVec[i];

        // Path which is to be associated with key
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;

            stringstream ss;
            ss << "INSERT INTO key_val (key, val, path, dt) VALUES('" << key
                    << "', '" << value << "', '"
                    << boost::filesystem::canonical(p).string()
                    << "', datetime('now'));";

            // Prepare statement
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
        } // NO PATH
        else
        {
            e_.err = PATH_DOES_NOT_EXIST_ERROR;
            e_.msg = string("Path (") + p.string() + string(") does not exist");
            return false;
        }
    } // FOR

    return true;
}

/*
 * This method deletes a key-value in the database associated with file(s)/folder(s).
 */bool FlagHandler::deleteKV(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // "All" flag set
    bool all = find(extraFlags.begin(), extraFlags.end(), ALL)
            != extraFlags.end();

    // Removed rows
    bool removedRows = false;

    // Key
    string key = (all) ? "" : fVec[0];

    // Verify key string
    if (!verifyInput(key, REGEX_MAIN_ALLOW_PERCENTAGE))
    {
        e_.err = VERIFICATION_ERROR;
        e_.msg = "Key can only contain numbers, letters and \"_\"";
        return false;
    }

#ifndef TEST
    // Safety question
    if (!q())
    return false;
#endif

    // Remove key with no path
    if (fVec.size() == 1 && !all)
    {
        sqlite3_stmt* statement;
        stringstream ss;

        ss << "DELETE FROM key_val WHERE key LIKE '" << key << "'";

        // Prepare statement
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

        if (sqlite3_changes(database_) != 0)
            removedRows = true;

        // Finalize
        sqlite3_finalize(statement);

        return (removedRows) ? true : false;
    } // IF NO PATH

    unsigned int i = (all) ? 0 : 1;

    // Remove keys with paths
    // Go through path(s)
    for (; i < fVec.size(); i++)
    {
        // File/folder string
        string f = fVec[i];

        // Path which is to be removed
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;

            stringstream ss;

            // Delete all keys
            if (all)
                ss << "DELETE FROM key_val WHERE path = '"
                        << boost::filesystem::canonical(p).string() << "';";
            else
                // Delete specific tags from file(s)/folder(s)
                ss << "DELETE FROM key_val WHERE key LIKE '" << key
                        << "' AND path = '"
                        << boost::filesystem::canonical(p).string() << "';";

            // Prepare statement
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

            if (sqlite3_changes(database_) != 0)
                removedRows = true;

            // Finalize
            sqlite3_finalize(statement);
        } // NO PATH
        else
        {
            e_.err = PATH_DOES_NOT_EXIST_ERROR;
            e_.msg = string("Path (") + p.string() + string(") does not exist");
            return false;
        }
    } // FOR

    return (removedRows) ? true : false;
}

/*
 * This method prints key-values associated with file(s)/folder(s).
 */
bool FlagHandler::printKV(const vector<string>& fVec,
        const vector<Flag>& extraFlags)
{
    // "All" flag set
    bool all = find(extraFlags.begin(), extraFlags.end(), ALL)
            != extraFlags.end();

    bool foundKey = false;

    // Key
    string key = (all) ? "" : fVec[0];

    // Verify key string
    if (!verifyInput(key, REGEX_MAIN_ALLOW_PERCENTAGE))
    {
        e_.err = VERIFICATION_ERROR;
        e_.msg = "Key can only contain numbers, letters and \"_\"";
        return false;
    }

    // Print key-value with no path
    if (fVec.size() == 1 && !all)
    {
        sqlite3_stmt* statement;
        stringstream ss;

        ss << "SELECT * FROM key_val WHERE key LIKE '" << key
                << "' ORDER BY dt COLLATE NOCASE DESC";

        // Prepare statement
        if (sqlite3_prepare_v2(database_, ss.str().c_str(), -1, &statement,
                0) != SQLITE_OK)
        {
            e_.err = STATEMENT_PREPARATION_ERROR;
            e_.msg = string("Could not prepare statement [") + ss.str()
                    + string("]");
            return false;
        }

        int result = 0;

        while (true)
        {
            // Execute
            result = sqlite3_step(statement);

            // Found row
            if (result == SQLITE_ROW)
            {
                foundKey = true;
                print_kv(statement);
            }
            else
            {
                break;
            }
        } // WHILE

        // Finalize
        sqlite3_finalize(statement);

        return (!foundKey) ? false : true;
    } // IF NO PATH

    unsigned int i = (all) ? 0 : 1;

    // Print key-value with path(s)
    // Go through path(s)
    for (; i < fVec.size(); i++)
    {
        // File/folder string
        string f = fVec[i];

        // Path which is to be printed
        boost::filesystem::path p(f);

        // Check if path exists
        if (boost::filesystem::is_directory(p) || boost::filesystem::exists(p))
        {
            sqlite3_stmt* statement;
            stringstream ss;

            // Select all
            if (all)
                ss << "SELECT * FROM key_val WHERE path = '"
                        << boost::filesystem::canonical(p).string()
                        << "' ORDER BY dt COLLATE NOCASE DESC";
            else
                // Select specific key
                ss << "SELECT * FROM key_val WHERE key LIKE '" << key
                        << "' AND path = '"
                        << boost::filesystem::canonical(p).string()
                        << "' ORDER BY dt COLLATE NOCASE DESC";

            // Prepare statement
            if (sqlite3_prepare_v2(database_, ss.str().c_str(), -1, &statement,
                    0) != SQLITE_OK)
            {
                e_.err = STATEMENT_PREPARATION_ERROR;
                e_.msg = string("Could not prepare statement [") + ss.str()
                        + string("]");
                return false;
            }

            int result = 0;

            while (true)
            {
                // Execute
                result = sqlite3_step(statement);

                // Found row
                if (result == SQLITE_ROW)
                {
                    foundKey = true;
                    print_kv(statement);
                }
                else
                {
                    break;
                }
            } // WHILE

            // Finalize
            sqlite3_finalize(statement);
        } // NO PATH
        else
        {
            e_.err = PATH_DOES_NOT_EXIST_ERROR;
            e_.msg = string("Path (") + p.string() + string(") does not exist");
            return false;
        }
    } // FOR

    return (!foundKey) ? false : true;
}

/*
 * This method prints key-values.
 */
void FlagHandler::print_kv(sqlite3_stmt* statement)
{
    if (statement == NULL)
        return;

    // Key
    string keyStr = (char*) sqlite3_column_text(statement, 1);

    // Value
    string valueStr = (char*) sqlite3_column_text(statement, 2);

    // Path
    string pathStr = (char*) sqlite3_column_text(statement, 3);

    // Datetime
    string dt = (char*) sqlite3_column_text(statement, 4);

    // Print path(s) found for specific key
    cout << "@" << keyStr << "\t\"" << valueStr << "\"\t" << pathStr << "\t["
            << dt << "]" << endl;
}

/*
 * Prints question.
 */
bool FlagHandler::q()
{
    char yn;
    cout << "Are you sure (y/n)? ";
    cin >> yn;

    if (tolower(yn) != 'y')
        return false;

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
