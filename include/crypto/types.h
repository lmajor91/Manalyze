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

#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_array.hpp>
#include <boost/system/api_config.hpp>

#ifndef __MANA_COMMONS_TYPES__
#define __MANA_COMMONS_TYPES__ 1

#if defined BOOST_WINDOWS_API && !defined CRYPTOLIB_API
#ifdef CRYPTOLIB_EXPORT
#define CRYPTOLIB_API __declspec(dllexport)
#else
#define CRYPTOLIB_API __declspec(dllimport)
#endif
#elif !defined BOOST_WINDOWS_API && !defined CRYPTOLIB_API
#define CRYPTOLIB_API
#endif

namespace crypto {

/**
 * This exists to marry the Windows and OpenSSL implementation of various hashing functions
 * together. Solely because Windows acquires hashing functions via. integers and OpenSSL
 * acquires hashing functions via. function calls or by strings.
 *
 * This interface is here to provide a platform independent way of obtaining a refernce to
 * either one.
 */
enum __algorithm_t { MD5, SHA1, NONE };

typedef crypto::__algorithm_t algorithm_t;

typedef boost::shared_ptr<std::string> pString;
typedef boost::shared_ptr<std::vector<std::string> > shared_strings;
typedef boost::shared_ptr<const std::vector<std::string> > const_shared_strings;

} // namespace crypto

#endif // __MANA_COMMONS_TYPES__