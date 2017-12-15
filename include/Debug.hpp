/*
 *  Debug.hpp
 *
 *  This is a header file for small debug functions.
 *
 *  Created on: 3 maj 2013
 *  Author: Hirad Asadi
 *  E-mail: hirad.asadi@gmail.com
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#ifdef DEBUG
namespace debug
{

// Simple print function for debug messages
inline static void dbgPrint(const std::string str)
{
    std::cout << "[DEBUG] " << str << std::endl;
}

// Function that prints the contents of a string vector
inline static void dbgPrintVector(const std::vector<std::string>& vec)
{
    for (const std::string& v : vec)
    {
        dbgPrint(std::string("in vector: " + v));
    }
}

}
#endif

#endif /* DEBUG_HPP_ */
