/*
 * Debug.hpp
 *	This is a header file for small debug functions.
 *  Created on: 3 maj 2013
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
