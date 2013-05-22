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
 * Write key-value test for different inputs.
 */
BOOST_AUTO_TEST_CASE( write_key_val_test )
{
    // Print headline
    printHeadline("write key-value test");

    // Clean up vector
    std::vector<std::string> cleanupVec;
    // Vector of characters
    std::vector<char> vec = genChars();

    // Try writing the wrong path
    std::string tmp("write_wrong");
    BOOST_ASSERT(runFlagWithInput("key_wrong", maptag::WRITE_KEY_VALUE, "crazy/path") == false);
    cleanupVec.push_back(tmp);

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        std::string input("test_tag_");
        input += vec[i] + "la la la";
        bool b = runFlagWithInput(input, maptag::WRITE_KEY_VALUE);

        // Correct input, try to write it, expect true result, assert on false
        if (verifyInput(input, REGEX_MAIN))
        {
            BOOST_ASSERT(b == true);
        }
        else
        {
            // Incorrect input, try to write it, expect false result, assert on true
            BOOST_ASSERT(b == false);
        }

        // Data was written to db, save it for clean up
        if (b)
            cleanupVec.push_back(input);
    }

    // Clean up
    for (unsigned int i = 0; i < cleanupVec.size(); i++)
    {
        runFlagWithInput(cleanupVec[i], maptag::DELETE_KEY_VALUE);
    }
}

/*
 * Print key-value test for different inputs.
 */
BOOST_AUTO_TEST_CASE( print_key_value_test )
{
    // Print headline
    printHeadline("print key-value test");

    // Insert to db tags
    runFlagWithInput("T1_TEST", maptag::WRITE_KEY_VALUE);
    runFlagWithInput("T2_TEST", maptag::WRITE_KEY_VALUE);

    // Print key-value test, test different key name variations for print flag
    BOOST_ASSERT(
            runFlagWithInput("t1_tEst", maptag::PRINT_KEY_VALUE) == true);
    BOOST_ASSERT(runFlagWithInput("test%", maptag::PRINT_KEY_VALUE) == false);
    BOOST_ASSERT(runFlagWithInput("%tEst", maptag::PRINT_KEY_VALUE) == true);
    BOOST_ASSERT(runFlagWithInput("", maptag::PRINT_KEY_VALUE) == false);
    // Show all for
    BOOST_ASSERT(
            runFlagWithInput("", maptag::PRINT_KEY_VALUE, "wrongPath/",
                    std::vector<maptag::Flag>(1, maptag::ALL)) == false);
    BOOST_ASSERT(
                runFlagWithInput("", maptag::PRINT_KEY_VALUE, *testFilePath,
                        std::vector<maptag::Flag>(1, maptag::ALL)) == true);

    BOOST_ASSERT(runFlagWithInput("%test", maptag::PRINT_KEY_VALUE, "") == true);

    // Clean up
    runFlagWithInput("T1_TEST", maptag::DELETE_KEY_VALUE);
    runFlagWithInput("T2_TEST", maptag::DELETE_KEY_VALUE);
}

/*
 * Delete key-value test for different inputs.
 */
BOOST_AUTO_TEST_CASE( delete_key_value_test )
{
    // Print headline
    printHeadline("delete key-value test");

    // Insert to db tags
    runFlagWithInput("T1_TEST", maptag::WRITE_KEY_VALUE);
    runFlagWithInput("T2_TEST", maptag::WRITE_KEY_VALUE);
    runFlagWithInput("T3_TEST", maptag::WRITE_KEY_VALUE);

    // Test removal
    BOOST_ASSERT(
            runFlagWithInput("t1_tEsta", maptag::DELETE_KEY_VALUE) == false);
    BOOST_ASSERT(
            runFlagWithInput("t2_tEsta", maptag::DELETE_KEY_VALUE) == false);
    BOOST_ASSERT(runFlagWithInput("%1_tEst", maptag::DELETE_KEY_VALUE) == true);
    BOOST_ASSERT(
            runFlagWithInput("T2_TEST", maptag::DELETE_KEY_VALUE,
                    "wrongPath/") == false);
    BOOST_ASSERT(
            runFlagWithInput("T2_TEST", maptag::DELETE_KEY_VALUE) == true);

    BOOST_ASSERT(runFlagWithInput("%3_test", maptag::DELETE_KEY_VALUE, "") == true);

    // Clean up
    runFlagWithInput("T1_TEST", maptag::DELETE_KEY_VALUE);
    runFlagWithInput("T2_TEST", maptag::DELETE_KEY_VALUE);
    // Remove all tags for test file
    runFlagWithInput("", maptag::DELETE_KEY_VALUE, *testFilePath,
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
    runFlagWithInput("", maptag::DELETE_KEY_VALUE, *testFilePath,
                std::vector<maptag::Flag>(1, maptag::ALL));
    // Finish test
    finish();
    std::cout << "Done" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()

} /* namespace maptagtest */

#endif

