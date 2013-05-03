/*
 * Debug.hpp
 *	This is a header file for small debug functions.
 *  Created on: 3 maj 2013
 *      Author: Hirad Asadi
 *      E-mail: hirad.asadi@gmail.com
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_
#ifdef DEBUG

// Simple print function for debug messages
void dbgPrint(const std::string str)
{
	std::cout << "[DEBUG] " << str << std::endl;
}

// Function that prints the content of a string vector array
void dbgPrintVector(const std::vector<std::string>& vec)
{
	for (const std::string& v : vec)
	{
		dbgPrint(std::string("in vector: " + v));
	}
}

#endif
#endif /* DEBUG_HPP_ */
