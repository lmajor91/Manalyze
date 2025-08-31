/*
    This file is part of Manalyze.

    Manalyze is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Manalyze is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Manalyze.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/system/api_config.hpp>
#include <iostream>
#include <string>

#include "manacommons/export.h"
#include "portability.h"
#include "types.h"

#if defined(WINDOWS)
#include "windows.h"
#else
#include <stdio.h>
#include <unistd.h>
#endif

#ifndef __MANACOMMONS_COLOR__
#define __MANACOMMONS_COLOR__ 1

namespace mana::io {

enum Color { RED, GREEN, YELLOW, RESET };

/**
 *	@brief	Set the font in the terminal to the specified color.
 *
 *	@param	Color c	The new color of the text.
 */
void set_color(Color c);

/**
 *	@brief	Prints the input text to a stream, after having applied color to the terminal.
 *
 *	@param	const std::string& text The text to write.
 *	@param	Color c The color to write the text in.
 *	@param	The stream into which the text should be written.
 *	@param	const std::string& prefix An optional prefix string, which will be displayed
 *in the default color.
 *	@param	const std::string& suffix An optional suffix string, which will be displayed
 *in the default color.
 *
 *	@return	A reference to sink, so the operator "<<" can be chained.
 */
DECLSPEC_MANACOMMONS std::ostream &print_colored_text(const std::string &text, Color c,
                                                      std::ostream &sink = std::cout,
                                                      const std::string &prefix = "",
                                                      const std::string &suffix = "");

#define PRINT_ERROR                                                                      \
    mana::io::print_colored_text("!", io::RED, std::cerr, "[", "] Error: ")
#define PRINT_WARNING                                                                    \
    mana::io::print_colored_text("*", io::YELLOW, std::cerr, "[", "] Warning: ")

#define LOG_CAP 100

/**
 *	@brief	Checks whether warnings should still be printed.
 */
DECLSPEC_MANACOMMONS bool is_log_cap_reached();

// Macros to add around very verbose warnings to avoid flooding stderr.
#define CAPPED_LOGGING if (!mana::io::is_log_cap_reached()) {
#define CAPPED_LOGGING_END }

#ifdef DEBUG
#define DEBUG_INFO " (" << __FILE__ << ":" << std::dec << __LINE__ << ")"
#define DEBUG_INFO_PE                                                                    \
    " (" << __FILE__ << ":" << std::dec << __LINE__ << ", " << *pe.get_path() << ")"
#define DEBUG_INFO_INSIDEPE                                                              \
    " (" << __FILE__ << ":" << std::dec << __LINE__ << ", " << *get_path() << ")"
#else
#define DEBUG_INFO ""
#define DEBUG_INFO_PE ""
#define DEBUG_INFO_INSIDEPE ""
#endif

} // namespace mana::io

#endif // __MANACOMMONS_COLOR__
