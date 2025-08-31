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

#include <sstream>
#include <string>
#include <vector>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/api_config.hpp>

#include "manacommons/export.h"
#include "types.h"

#ifndef __MANACOMMONS_BASE64__
#define __MANACOMMONS_BASE64__ 1

namespace utils {

// ----------------------------------------------------------------------------

/**
 *	@brief	Converts the input data into a Base64 encoded string.
 *
 *	Taken from the boost examples and slightly adaped to handle padding.
 *
 *	@param	const std::vector<boost::uint8_t>& bytes A vector of bytes to encode.
 *
 *	@return	A string containing the Base64 representation of the input.
 */
DECLSPEC_MANACOMMONS pString b64encode(const std::vector<boost::uint8_t> &bytes);

} // namespace utils

#endif // __MANACOMMONS_BASE64__
