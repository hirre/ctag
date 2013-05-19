/*
 * MapTagTest.cpp
 *
 *  Unit tests for maptag. Uses .maptag database in home folder.
 *  DON'T CHANGE THE TESTS' ORDER OF APPEARANCE!
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
#define BOOST_TEST_MODULE MapTagTestModule

#include <boost/test/auto_unit_test.hpp>
#include <vector>
#include "MapTagTest.hpp"

namespace maptagtest
{

BOOST_AUTO_TEST_SUITE(maptag_test_suite)

/*
* Init test.
*/
BOOST_AUTO_TEST_CASE( init_test )
{
    // Print headline
    printHeadline("init");
    // Init test
    init();
    std::cout << "Done" << std::endl;
}

/*
 * Tag test for different inputs.
 */
BOOST_AUTO_TEST_CASE( tag_test )
{
    // Print headline
    printHeadline("tag test");

    // Clean up vector
    std::vector<std::string> cleanupVec;
    // Vector of characters
    std::vector<char> vec = genChars();

    // Try tagging the wrong path
    std::string tmp("TAG_wrong");
    BOOST_ASSERT(runFlagWithInput("TAG_wrong", maptag::TAG, "crazy/path") == false);
    cleanupVec.push_back(tmp);

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        std::string input("test_tag_");
        input += vec[i];
        bool b = runFlagWithInput(input, maptag::TAG);

        // Correct input, try to tag it, expect true result, assert on false
        if (verifyInput(input, REGEX_TAG))
        {
            BOOST_ASSERT(b == true);
        }
        else
        {
            // Incorrect input, try to tag it, expect false result, assert on true
            BOOST_ASSERT(b == false);
        }

        // Data was written to db, save it for clean up
        if (b)
            cleanupVec.push_back(input);
    }

    // Clean up
    for (unsigned int i = 0; i < cleanupVec.size(); i++)
    {
        runFlagWithInput(cleanupVec[i], maptag::REMOVE_TAG);
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
    runFlagWithInput("TAG1_TEST", maptag::TAG);
    runFlagWithInput("TAG2_TEST", maptag::TAG);

    // Show tag test, test different tag name variations for show flag
    BOOST_ASSERT(
            runFlagWithInput("tAg1_tEst", maptag::SHOW_TAG) == true);
    BOOST_ASSERT(runFlagWithInput("test%", maptag::SHOW_TAG) == false);
    BOOST_ASSERT(runFlagWithInput("%tEst", maptag::SHOW_TAG) == true);
    BOOST_ASSERT(runFlagWithInput("", maptag::SHOW_TAG) == false);
    // Show all
    BOOST_ASSERT(
            runFlagWithInput("", maptag::SHOW_TAG, "wrongPath/",
                    std::vector<maptag::Flag>(1, maptag::ALL)) == false);
    BOOST_ASSERT(
                runFlagWithInput("", maptag::SHOW_TAG, *testFilePath,
                        std::vector<maptag::Flag>(1, maptag::ALL)) == true);

    // Clean up
    runFlagWithInput("TAG1_TEST", maptag::REMOVE_TAG);
    runFlagWithInput("TAG2_TEST", maptag::REMOVE_TAG);
}

/*
 * Remove tag test for different inputs.
 */
BOOST_AUTO_TEST_CASE( remove_tag_test )
{
    // Print headline
    printHeadline("remove tag test");

    // Insert to db tags
    runFlagWithInput("TAG1_TEST", maptag::TAG);
    runFlagWithInput("TAG2_TEST", maptag::TAG);
    runFlagWithInput("TAG3_TEST", maptag::TAG);

    // Test removal
    BOOST_ASSERT(
            runFlagWithInput("tAg1_tEsta", maptag::REMOVE_TAG) == false);
    BOOST_ASSERT(
            runFlagWithInput("tAg2_tEsta", maptag::REMOVE_TAG) == false);
    BOOST_ASSERT(runFlagWithInput("%1_tEst", maptag::REMOVE_TAG) == true);
    BOOST_ASSERT(
            runFlagWithInput("TAG2_TEST", maptag::REMOVE_TAG,
                    "wrongPath/") == false);
    BOOST_ASSERT(
            runFlagWithInput("TAG2_TEST", maptag::REMOVE_TAG) == true);

    // Clean up
    runFlagWithInput("TAG1_TEST", maptag::REMOVE_TAG);
    runFlagWithInput("TAG2_TEST", maptag::REMOVE_TAG);
    // Remove all tags for test file
    runFlagWithInput("", maptag::REMOVE_TAG, *testFilePath,
            std::vector<maptag::Flag>(1, maptag::ALL));

}

/*
 * Finish test.
 */
BOOST_AUTO_TEST_CASE( finish_test )
{
    // Print headline
    printHeadline("finish test");
    // Remove all tags for test file
    runFlagWithInput("", maptag::REMOVE_TAG, *testFilePath,
                std::vector<maptag::Flag>(1, maptag::ALL));
    // Finish test
    finish();
    std::cout << "Done" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()

}

#endif

