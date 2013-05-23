/*
 *  Main.cpp
 *
 *  This is the maptag main program entry point.
 *
 *  Created on: 30 apr 2013
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

#define VERSION "0.3"

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "Debug.hpp"
#include "core/FlagHandler.hpp"
#include "include/Flags.hpp"

namespace bpo = boost::program_options;
using namespace std;

#ifndef TEST
/*
 * Main function.
 */
int main(int argc, char** argv)
{
    // Processed ok per default
    bool processed = true;

    try
    {
        // TODO: room for improved command descriptions
        // Create options
        bpo::options_description opts("Usage");
        opts.add_options()("h", "help message")("w",
                bpo::value<vector<string> >()->multitoken(),
                "write key-value, [<key> <value (with quotes)> <file/folder> ...]")(
                "d", bpo::value<vector<string> >()->multitoken(),
                "delete key-value, [<key> opt=[<file/folder> ...]], "
                "% allowed in <key> for missing characters")("p",
                bpo::value<vector<string> >()->multitoken(),
                "print key-value, [<key> opt=[<file/folder> ...]], "
                "% allowed in <key> for missing characters")("a",
                "all flag, for specific file(s)/folder(s), "
                "together with -p to print all key-values, or "
                "together with -d to delete all key-values")("v",
                "version number");

        // Map for storing input
        bpo::variables_map vm;

        // Store input from cmdline in vm
        bpo::store(
                bpo::command_line_parser(argc, argv).options(opts).style(
                        bpo::command_line_style::default_style
                        | bpo::command_line_style::allow_long_disguise).run(),
                vm);

        // Manage "help" flag
        if (vm.size() == 0 || vm.count("h"))
        {
            stringstream stream;
            stream << opts;
            string helpMsg = stream.str();
            // Replace "--" with "-"
            boost::algorithm::replace_all(helpMsg, "--", "-");
            cout << helpMsg << endl;
            return 0;
        }

        // Validate inputs
        bpo::notify(vm);

        // Create handler to handle input
        maptag::FlagHandler fHandler;

        // Extra flags
        vector<maptag::Flag> extraFlags;

        // Manage all flag
        if (vm.count("a"))
        extraFlags.push_back(maptag::ALL);

        // Manage "write key-value" flag
        if (vm.count("w"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "write key-value | nr of args: "
                    + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["w"].as<vector<string> >());
#endif
            if ((argc - 2) >= 2)
            {
                processed = fHandler.processInput(vm["w"].as<vector<string> >(),
                        maptag::WRITE_KEY_VALUE, extraFlags);
            }
            else
            cerr
            << "Not the right number of arguments for write key-value flag."
            << endl;
        }

        // Manage "delete key-value" flag
        if (vm.count("d"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "delete key-value | nr of args: "
                    + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["d"].as<vector<string> >());
#endif
            if ((argc - 2) >= 1)
            {
                processed = fHandler.processInput(vm["d"].as<vector<string> >(),
                        maptag::DELETE_KEY_VALUE, extraFlags);
            }
            else
            cerr
            << "Not the right number of arguments for delete key-value flag."
            << endl;
        }

        // Manage "print key-value" flag
        if (vm.count("p"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "print key-value | nr of args: "
                    + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["p"].as<vector<string> >());
#endif
            if ((argc - 2) >= 1)
            {
                processed = fHandler.processInput(vm["p"].as<vector<string> >(),
                        maptag::PRINT_KEY_VALUE, extraFlags);
            }
            else
            cerr
            << "Not the right number of arguments for print key-value flag."
            << endl;
        }

        // Manage "version" flag
        if (vm.count("v"))
        {
            cout << "Version " + string(VERSION) << endl;
        }

        // Manage error
        if (!processed && fHandler.getError().err != maptag::NO_ERROR)
        cerr << fHandler.getError().msg << ".\t[" << fHandler.getError().err
        << "]" << endl;

    }
    catch (bpo::error& e)
    {
        // Write error to stderr
        cerr << "Invalid arguments." << endl;
        return 1;
    }

    return processed;
}

#endif
