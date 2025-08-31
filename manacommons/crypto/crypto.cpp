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

#include <boost/smart_ptr/make_shared_array.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "manacommons/crypto/crypto.h"
#include "manacommons/export.h"
#include "portability.h"

namespace mana::crypto {

DECLSPEC_MANACOMMONS boost::shared_ptr<Hash> get_algorithm(algorithm_t algo) {
    switch (algo) {
    case crypto::algorithm_t::MD5:
        return boost::make_shared<Hash>(Hash(algorithm_t::MD5));
    case crypto::algorithm_t::SHA1:
        return boost::make_shared<Hash>(Hash(algorithm_t::SHA1));

    // base case in case there's bad input
    case crypto::algorithm_t::NONE:
    default:
        return nullptr;
    }
}

DECLSPEC_MANACOMMONS const std::string get_string_name(crypto::algorithm_t algo) {
    switch (algo) {
    case crypto::algorithm_t::MD5:
        return std::string("md5");
    case crypto::algorithm_t::SHA1:
        return std::string("sha1");

    // base case in case there's bad input
    case crypto::algorithm_t::NONE:
    default:
        return std::string("none");
    }
}

DECLSPEC_MANACOMMONS const std::string bytes_to_hex(const void *data, size_t len) {
    return std::string("bytes_to_hex is not implemented yet");
}

}; // namespace mana::crypto
