/*
 * Main.cpp
 *
 *	This is the CTag main program entry point.
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

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include "Debug.hpp"

namespace bpo = boost::program_options;
using namespace std;

/*
 * Main function.
 */
int main(int argc, char** argv)
{
	try
	{
		// Create options
		bpo::options_description opts("Usage");
		opts.add_options()("help,h", "help message")("tag,t",
				bpo::value<vector<string> >()->multitoken(),
				"arg= <tag-string> <file_1 ... file_n>, e.g. \"-t helo /test/file7.txt\" or \"-t helo /test/*.txt\" for all txt files in a specific folder")(
				"removetag,r", bpo::value<vector<string> >()->multitoken(),
				"arg= <tag-string> <file_1 ... file_n>, e.g. \"-r helo /test/file7.txt\" or \"-r helo /test/*.txt\" for all txt files in a specific folder")(
				"showtag,s", bpo::value<string>(),
				"arg= <folder name or \"all\">, e.g. \"-s all\" to show all tags or \"-s /test/\" to show all tags in a specific folder")
				("version,v", "version number");

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

		// Manage "tag" flag
		if (vm.count("tag"))
		{
			// TODO: implement tag, manage minimum number of flags

#ifdef DEBUG
			dbgPrint(
					"--tag | nr of args: "
							+ boost::lexical_cast<string>(vm.count("tag")));
			dbgPrintVector(vm["tag"].as<vector<string> >());
#endif
		}

		// Manage "removetag" flag
		if (vm.count("removetag"))
		{
			// TODO: implement remove tag, manage minimum number of flags

#ifdef DEBUG
			dbgPrint("--removetag | nr of args: " + vm.count("removetag"));
			dbgPrintVector(vm["removetag"].as<vector<string> >());
#endif
		}

		// Manage "showtag" flag
		if (vm.count("showtag"))
		{
			// TODO: implement show tag, handle "all" option and the rest

#ifdef DEBUG
			dbgPrint("--showtag | nr of args: " + vm.count("showtag"));
			dbgPrint(vm["showtag"].as<string>());
#endif
		}

		// Manage "version" flag
		if(vm.count("version"))
		{
			std::cout << "Version " + string(VERSION) << std::endl;
		}

	} catch (bpo::error& e)
	{
		// Write error to stderr
		cerr << e.what();
		return 1;
	}

	return 0;
}
