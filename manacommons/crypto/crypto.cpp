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

#include "crypto/crypto.h"
#include "portability.h"

namespace crypto {

CRYPTOLIB_API Hash *get_algorithm(algorithm_t algo) {
    switch (algo) {
    case crypto::algorithm_t::MD5:
        return new Hash(algorithm_t::MD5);
    case crypto::algorithm_t::SHA1:
        return new Hash(algorithm_t::SHA1);

    // base case in case there's bad input
    case crypto::algorithm_t::NONE:
    default:
        return nullptr;
    }
}

CRYPTOLIB_API const std::string get_string_name(crypto::algorithm_t algo) {
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

CRYPTOLIB_API const std::string bytes_to_hex(const void *data, size_t len) {
    return std::string("bytes_to_hex is not implemented yet");
}

}; // namespace crypto