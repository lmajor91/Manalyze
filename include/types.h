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

#include <map>
#include <set>
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

#ifndef __MANALYZE_TYPES__
#define __MANALYZE_TYPES__ 1

typedef boost::shared_ptr<std::string> pString;
typedef boost::shared_ptr<std::vector<std::string>> shared_strings;
typedef boost::shared_ptr<const std::vector<std::string>> const_shared_strings;
typedef std::vector<std::string> strings;
typedef std::set<std::string> string_set;

typedef std::map<std::string, std::string> string_map;
typedef boost::shared_ptr<const std::map<std::string, std::string>> shared_string_map;

#endif // __MANALYZE_TYPES__
