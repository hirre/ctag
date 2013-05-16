/*
 * CTagTest.hpp
 *
 *  Unit tests for ctag. Uses .ctag database in home folder.
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

#ifndef CTAGTEST_HPP_
#define CTAGTEST_HPP_

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include "../src/core/CTagHandler.hpp"
#include "Flags.hpp"

namespace ctagtest
{

// Global id
static long double id = 0;

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
        const ctag::Flag& flag, const std::string& path = "/")
{
    ctag::CTagHandler handler;
    std::vector<std::string> inputVec;
    inputVec.push_back(input);
    // Root path always exists
    inputVec.push_back(path);
    // Print input
    std::cout << "\n>> Running: \"" << inputVec[0] << " " << path
            << "\" (flag = " << flag << ")\n" << std::endl;
    // Add test tag
    return handler.processInput(inputVec, flag);
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

/**
 * Returns a unique current time string.
 */
static inline const std::string uniqueStr()
{
    static const char* fmt = "_%Y%m%d%H%M%S_";
    std::ostringstream ss;
    ss.imbue(
            std::locale(std::cout.getloc(),
                    new boost::posix_time::time_facet(fmt)));
    ss << id++ << boost::posix_time::second_clock::local_time();
    return ss.str();
}

}
#endif /* CTAGTEST_HPP_ */
