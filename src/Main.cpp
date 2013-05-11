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

#define VERSION "0.1"

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "Debug.hpp"
#include "core/CTagHandler.h"

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
        opts.add_options()("help,h", "help message")("tag,t",
                bpo::value<vector<string> >()->multitoken(),
                "arg= <tag-string> <file(s)/folder(s)>, e.g. \"-t helo /test/file7.txt\", or "
                        "\"-t helo /test/*.txt\" for all txt files in a specific folder, or "
                        "\"-t helo /test/\" to tag a specific folder")(
                "removetag,r", bpo::value<vector<string> >()->multitoken(),
                "arg= <tag-string> <file(s)/folder(s)>, e.g. \"-r helo /test/file7.txt\", or "
                        "\"-r helo /test/*.txt\" for all txt files in a specific folder, or "
                        "\"-r helo /test/\" to remove tag for a specific folder, "
                        "use \"#\" (with quotes) instead of tag name to remove all tags for file(s)/folder(s)")(
                "showtag,s", bpo::value<vector<string> >()->multitoken(),
                "arg= <tag> <file(s)/folder(s)> to show file(s)/folder(s) with a specific tag, "
                        "e.g. \"-s testtag /usr/lib/*\" shows all files in \"/usr/lib/\" tagged with \"testtag\", "
                        "use \"#\" (with quotes) instead of tag name to show all tags")(
                "version,v", "version number");

        // Map for storing input
        bpo::variables_map vm;

        // Store input from cmdline in vm
        bpo::store(bpo::command_line_parser(argc, argv).options(opts).run(),
                vm);

        // Manage "help" flag
        if (vm.size() == 0 || vm.count("help"))
        {
            cout << opts << endl;
            return 1;
        }

        // Validate inputs
        bpo::notify(vm);

        // Create handler to handle input
        ctag::CTagHandler cHandler;

        // Manage "tag" flag
        if (vm.count("tag"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "--tag | nr of args: "
                            + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["tag"].as<vector<string> >());
#endif
            if ((argc - 2) >= 2)
                cHandler.processInput(vm["tag"].as<vector<string> >(), "tag");
            else
                cerr << "Not the right number of arguments for tag flag."
                        << std::endl;
        }

        // Manage "removetag" flag
        if (vm.count("removetag"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "--removetag | nr of args: "
                            + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["removetag"].as<vector<string> >());
#endif
            if ((argc - 2) >= 2)
                cHandler.processInput(vm["removetag"].as<vector<string> >(),
                        "removetag");
            else
                cerr << "Not the right number of arguments for removetag flag."
                        << std::endl;
        }

        // Manage "showtag" flag
        if (vm.count("showtag"))
        {
#ifdef DEBUG
            debug::dbgPrint(
                    "--showtag | nr of args: "
                            + boost::lexical_cast<string>(argc - 2));
            debug::dbgPrintVector(vm["showtag"].as<vector<string> >());
#endif
            if ((argc - 2) >= 2)
                cHandler.processInput(vm["showtag"].as<vector<string> >(),
                        "showtag");
            else
                cerr << "Not the right number of arguments for showtag flag."
                        << std::endl;
        }

        // Manage "version" flag
        if (vm.count("version"))
        {
            std::cout << "Version " + string(VERSION) << std::endl;
        }

    } catch (bpo::error& e)
    {
        // Write error to stderr
        cerr << "Invalid arguments." << std::endl;
        return 1;
    }

    return 0;
}

#endif
