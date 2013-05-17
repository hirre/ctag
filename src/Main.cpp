/*
 *  Main.cpp
 *
 *  This is the ctag main program entry point.
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

#define VERSION "0.21"

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "Debug.hpp"
#include "core/CTagHandler.hpp"
#include "Flags.hpp"

namespace bpo = boost::program_options;
using namespace std;

#ifndef TEST
/*
 * Main function.
 */
int main(int argc, char** argv)
{
    try
    {
        // TODO: room for improved command descriptions
        // Create options
        bpo::options_description opts("Usage");
        opts.add_options()("h", "help message")("t",
                bpo::value<vector<string> >()->multitoken(),
                "tag arg= <tag-string> <file(s)/folder(s)>, e.g. \"-t helo /test/file7.txt\", or "
                "\"-t helo /test/*.txt\" for all txt files in a specific folder, or "
                "\"-t helo /test/\" to tag a specific folder")("r",
                bpo::value<vector<string> >()->multitoken(),
                "remove tag arg= <tag-string> <file(s)/folder(s)>, e.g. \"-r helo /test/file7.txt\", or "
                "\"-r helo /test/*.txt\" for all txt files in a specific folder, or "
                "\"-r helo /test/\" to remove tag for a specific folder, or "
                "combine tag name with % (wildcard) for missing characters")(
                "s", bpo::value<vector<string> >()->multitoken(),
                "show tag arg= <tag> <file(s)/folder(s)>, "
                "e.g. \"-s testtag /usr/lib/*\" shows all files in \"/usr/lib/\" tagged with \"testtag\", or "
                "combine tag name with % (wildcard) for missing characters")(
                "v", "version number")("a",
                "all flag together with -s to show all tags for specific file(s)/folder(s), or "
                        "-r to remove all tags for specific file(s)/folder(s)");

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
            std::stringstream stream;
            stream << opts;
            string helpMsg = stream.str();
            // Replace "--" with "-"
            boost::algorithm::replace_all(helpMsg, "--", "-");
            cout << helpMsg << endl;
            return 1;
        }

        // Validate inputs
        bpo::notify(vm);

        // Create handler to handle input
        ctag::CTagHandler cHandler;

        // Extra flags
        std::vector<ctag::Flag> extraFlags;

        // Manage all flag
        if (vm.count("a"))
        {
            extraFlags.push_back(ctag::ALL);
        }

        // Manage "tag" flag
        if (vm.count("t"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "tag | nr of args: "
                    + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["t"].as<vector<string> >());
#endif
            if ((argc - 2) >= 2)
            cHandler.processInput(vm["t"].as<vector<string> >(), ctag::TAG);
            else
            cerr << "Not the right number of arguments for tag flag."
            << std::endl;
        }

        // Manage "removetag" flag
        if (vm.count("r"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "remove tag | nr of args: "
                    + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["r"].as<vector<string> >());
#endif
            if ((argc - 2) >= 1)
            cHandler.processInput(vm["r"].as<vector<string> >(),
                    ctag::REMOVE_TAG, extraFlags);
            else
            cerr << "Not the right number of arguments for remove tag flag."
            << std::endl;
        }

        // Manage "showtag" flag
        if (vm.count("s"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "show tag | nr of args: "
                    + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["s"].as<vector<string> >());
#endif
            if ((argc - 2) >= 1)
            cHandler.processInput(vm["s"].as<vector<string> >(),
                    ctag::SHOW_TAG, extraFlags);
            else
            cerr << "Not the right number of arguments for show tag flag."
            << std::endl;
        }

        // Manage "version" flag
        if (vm.count("v"))
        {
            std::cout << "Version " + string(VERSION) << std::endl;
        }

    }
    catch (bpo::error& e)
    {
        // Write error to stderr
        cerr << "Invalid arguments." << std::endl;
        return 1;
    }

    return 0;
}

#endif
