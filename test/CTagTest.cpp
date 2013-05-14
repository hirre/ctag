/*
 * CTagTest.cpp
 *
 *  Unit tests for ctag.
 *
 *  Created on: 11 maj 2013
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

#ifdef TEST
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CTagTestModule

#include "CTagTest.hpp"
#include <boost/test/auto_unit_test.hpp>
#include "../src/core/CTagHandler.hpp"
#include "../include/Helper.hpp"

BOOST_AUTO_TEST_SUITE(ctag_test_suite)

/*
 * Tag test for different inputs.
 */
BOOST_AUTO_TEST_CASE( tag_test )
{
    ctag::CTagHandler handler;
    char* c = genChar();

    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        std::vector<std::string> inputVec;
        inputVec.push_back(std::string("arg")+c[i]);
        // Root path always exists
        inputVec.push_back(std::string("/"));
        std::cout << ">> INPUT: " << inputVec[0] << " " << inputVec[1] << std::endl;

        // Correct input, try to tag it, expect true result, assert on false
        if(verifyInput(inputVec[0], REGEX_MAIN))
            BOOST_ASSERT(handler.processInput(inputVec, std::string("tag")) == true);
        else
            // Incorrect input, try to tag it, expect false result, assert on true
            BOOST_ASSERT(handler.processInput(inputVec, std::string("tag")) == false);
    }

    delete [] c;
}

/*
 * Show tag test for different inputs.
 */
BOOST_AUTO_TEST_CASE( show_test )
{

}

BOOST_AUTO_TEST_SUITE_END()

#endif

