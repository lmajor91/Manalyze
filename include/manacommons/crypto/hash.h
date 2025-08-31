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

#include <cassert>
#include <stdexcept>
#include <string>

#include "portability.h"
#include "manacommons/export.h"
#include "manacommons/crypto/algorithms.h"

#if defined(WINDOWS)
#else
#include <openssl/evp.h>
#endif

#ifndef __MANACOMMONS_HASH__
#define __MANACOMMONS_HASH__ 1

namespace manacommons::crypto {

// templating the class to forcibly type the context
class Hash {

#pragma region fields
  private:
// the internal context block, this must be set by a
// child class.
#if defined(WINDOWS)
    void *ctx = nullptr;
    void *algo = nullptr;
#else
    // reference to the context block
    EVP_MD_CTX *_ctx = nullptr;

    // reference to the hashing algorithm
    EVP_MD *_algo = nullptr;
#endif

  public:
    // tracks what kind of hashing algorithm this is
    crypto::algorithm_t name = crypto::algorithm_t::NONE;

#pragma endregion
#pragma region methods

  public:
    // These functions are platform specific and will need to be defined
    // by all platforms.
    DECLSPEC_MANACOMMONS Hash();
    DECLSPEC_MANACOMMONS Hash(crypto::algorithm_t name);
    DECLSPEC_MANACOMMONS ~Hash();

// in case you still want to access the internal context block
#if defined(WINDOWS)
    DECLSPEC_MANACOMMONS void *get_ctx() { return this->ctx; }
#else
    DECLSPEC_MANACOMMONS EVP_MD_CTX *get_ctx() { return this->_ctx; }
#endif

// in case you still want access to the internal hashing algorithm
#if defined(WINDOWS)
    DECLSPEC_MANACOMMONS void *get_algorithm() { return this->ctx; }
#else
    DECLSPEC_MANACOMMONS EVP_MD *get_algorithm() { return this->_algo; }
#endif

    /// compute hash of a memory block
    DECLSPEC_MANACOMMONS std::string operator()(const void *data, size_t numBytes) {
        // adding the bytes
        this->add(data, numBytes);

        // translate to hex
        return this->get_hash();
    }

    /// compute hash of a string, excluding final zero
    DECLSPEC_MANACOMMONS std::string operator()(const std::string &text) {
        // adding the bytes
        this->add(text.c_str(), text.length());

        // translate to hex
        return this->get_hash();
    }

    /**
     *	@brief	Ingests bytes into the inner context variable. This function assumes that
     *          the inner context object and inner algorithm handler are not null and
     *          ready to be used.
     *	@param	data the bytes to ingest
     *	@param	numBytes the number of bytes to ingest
     */
    DECLSPEC_MANACOMMONS void add(const void *data, size_t numBytes);

    /**
     * @brief	Returns the hexadecimal representation of the hashed contents.
     * @returns
     */
    DECLSPEC_MANACOMMONS std::string get_hash();

    /**
     * @brief Resets the state of the context. This should make the internal context ready
     * to be reused.
     */
    DECLSPEC_MANACOMMONS void reset();
};

#pragma endregion
#pragma region types

typedef boost::shared_ptr<Hash> pHash;

#pragma endregion

} // namespace crypto

#endif // __MANACOMMONS_HASH__
