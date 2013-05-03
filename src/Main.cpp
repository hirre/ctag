/*
 * Main.cpp
 *	This is the CTag main program entry point.
 *  Created on: 30 apr 2013
 *      Author: Hirad Asadi
 *      E-mail: hirad.asadi@gmail.com
 */

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
				"removetag,r",
				bpo::value<vector<string> >()->multitoken(),
				"arg= <tag-string> <file_1 ... file_n>, e.g. \"-r helo /test/file7.txt\" or \"-r helo /test/*.txt\" for all txt files in a specific folder")(
				"showtag,s", bpo::value<string>(),
				"arg= <folder name or \"all\">, e.g. \"-s all\" to show all tags or \"-s /test/\" to show all tags in a specific folder");

		// Map for storing input
		bpo::variables_map vm;

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
			dbgPrint("--tag | nr of args: " + boost::lexical_cast<string>(vm.count("tag")));
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

	} catch (bpo::error& e)
	{
		// Write error to stderr
		cerr << e.what();
		return 1;
	}

	return 0;
}
