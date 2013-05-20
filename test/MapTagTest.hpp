/*
 * MapTagTest.hpp
 *
 *  Unit tests for maptag. Uses .maptag database in home folder.
 *
 *
 *  Created on: 13 maj 2013
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

#ifndef MAPTAGTEST_HPP_
#define MAPTAGTEST_HPP_

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "../src/core/FlagHandler.hpp"
#include "Flags.hpp"
#include "Helper.hpp"

namespace maptagtest
{

// Global test file path
static std::string* testFilePath;

/*
 * Init test method.
 */
void init()
{
    // Create a test file in home folder
    std::stringstream ss;
    ss << getHomeFolder() << PATH_SEPARATOR << "tag_test_file.test";
    testFilePath = new std::string(ss.str());
    std::ofstream outfile(*testFilePath);
    outfile << "Tag test file!" << std::endl;
    outfile.close();
}

/*
 * Finish test method.
 */
void finish()
{
    // Remove test file
    remove(testFilePath->c_str());
    delete testFilePath;
}

/*
 * Print headline.
 */
static inline void printHeadline(const std::string& h)
{
    std::cout << "\n****************************************" << std::endl;
    std::cout << boost::to_upper_copy(h) << std::endl;
    std::cout << "****************************************\n" << std::endl;
}

/*
 * Run flag with input against db.
 * Return true on success, else false.
 */
static inline bool runFlagWithInput(const std::string& input,
        const maptag::Flag& flag,
        const std::string& path = std::string(*testFilePath),
        const std::vector<maptag::Flag>& extraFlags = std::vector<maptag::Flag>())
{
    maptag::FlagHandler handler;
    std::vector<std::string> inputVec;

    if (!input.empty())
        inputVec.push_back(input);

    // Root path always exists
    inputVec.push_back(path);

    // String for extra flags
    std::stringstream ss;
    for (unsigned int i = 0; i < extraFlags.size(); i++)
    {
        ss << extraFlags[i] << " ";
    }

    // Print input
    if (!input.empty())
        std::cout << "\n>> Running: \"" << inputVec[0] << " " << path
                << "\" (flag = " << flag << ", extra flags = {" << ss.str()
                << "})\n" << std::endl;
    else
        std::cout << "\n>> Running: \" " << path << "\" (flag = " << flag
                << ", extra flags = {" << ss.str() << "})\n" << std::endl;

    // Process flag with input
    if (extraFlags.empty())
        return handler.processInput(inputVec, flag);
    else // Extra flags added
        return handler.processInput(inputVec, flag, extraFlags);
}

/*
 * Generates vector of chars
 */
std::vector<char> genChars()
{
    std::vector<char> vec;

    for (int i = 32; i < 127; i++)
    {
        vec.push_back(i);
    }

    return vec;
}

} /* namespace maptagtest */

#endif /* MAPTAGTEST_HPP_ */
