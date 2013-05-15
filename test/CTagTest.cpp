/*
 * CTagTest.cpp
 *
 *  Unit tests for ctag. Assumes database is empty!
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

#include <boost/test/auto_unit_test.hpp>
#include "CTagTest.hpp"
#include "../src/core/CTagHandler.hpp"
#include "../include/Helper.hpp"

namespace ctagtest
{

BOOST_AUTO_TEST_SUITE(ctag_test_suite)

/*
 * Tag test for different inputs.
 */
BOOST_AUTO_TEST_CASE( tag_test )
{
    // Print headline
    printHeadline("tag test");

    std::vector<char> vec = genChars();

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        std::string input(std::string("arg") + vec[i]);

        // Correct input, try to tag it, expect true result, assert on false
        if (verifyInput(input, REGEX_MAIN))
            BOOST_ASSERT(runFlagWithInput(input, ctag::TAG) == true);
        else
            // Incorrect input, try to tag it, expect false result, assert on true
            BOOST_ASSERT(runFlagWithInput(input, ctag::TAG) == false);
    }
}

/*
 * Show tag test for different inputs.
 */

BOOST_AUTO_TEST_CASE( show_tag_test )
{
    // Print headline
    printHeadline("show tag test");

    // Insert to db tags
    runFlagWithInput("TAG1_TEST", ctag::TAG);
    runFlagWithInput("TAG2_TEST", ctag::TAG);

    // Show tag test
    // TODO: test fails even though real version works
    BOOST_ASSERT(runFlagWithInput("tAg1_tEst", ctag::SHOW_TAG) == true);
    BOOST_ASSERT(runFlagWithInput("tEst%", ctag::SHOW_TAG) == false);
    BOOST_ASSERT(runFlagWithInput("%tEst", ctag::SHOW_TAG) == true);
}

/*
 * Remove tag test for different inputs.
 */

BOOST_AUTO_TEST_CASE( remove_tag_test )
{
    // Print headline
    printHeadline("remove tag test");
}
BOOST_AUTO_TEST_SUITE_END()

}

#endif

