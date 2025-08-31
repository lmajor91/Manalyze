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

#include "manacommons/crypto/crypto.h"
#include "manacommons/crypto/hash.h"
#include "manacommons/crypto/algorithms.h"
#include "portability.h"

#if defined(WINDOWS)
#else
#include <openssl/evp.h>
#endif

// only compile this if building for Windows
#if !defined(WINDOWS)

namespace crypto {

Hash::Hash(crypto::algorithm_t name) {
    // copying the name
    this->name = name;

    // getting the name of the algorithm
    std::string algo = algo_to_string(name);

    // create a new context
    this->_ctx = EVP_MD_CTX_new();
    if (this->_ctx == nullptr) {
        throw new std::runtime_error("EVP_MD_CTX_new failed");
    }

    // get a handle to the hashing function
    this->_algo = EVP_MD_fetch(nullptr, algo.c_str(), nullptr);
    if (this->_algo == nullptr) {
        // cleanup
        EVP_MD_CTX_free(this->_ctx);
        throw new std::runtime_error("EVP_MD_fetch(" + algo + ") failed");
    }

    // initialize it
    if (!EVP_DigestInit_ex(this->_ctx, this->_algo, nullptr)) {
        // cleanup
        EVP_MD_free(this->_algo);
        EVP_MD_CTX_free(this->_ctx);
        throw new std::runtime_error("EVP_DigestInit_ex(ctx, " + algo + ") failed");
    }
};

Hash::~Hash() {
    // it's really weird if only one of these is defined, but i'm not
    // gonna judge
    if (this->_algo != nullptr) {
        EVP_MD_free(this->_algo);
    }

    if (this->_ctx != nullptr) {
        EVP_MD_CTX_free(this->_ctx);
    }
}

inline void Hash::reset() { EVP_MD_CTX_reset(this->_ctx); }

void Hash::add(const void *data, size_t numBytes) {
    // checking
    assert(this->_ctx != nullptr);
    assert(this->_algo != nullptr);

    // ingest the data
    if (!EVP_DigestUpdate(this->_ctx, data, numBytes)) {
        // clean up
        EVP_MD_free(this->_algo);
        EVP_MD_CTX_free(this->_ctx);

        // build & throw an error message
        std::string algo_name = algo_to_string(this->name);
        throw new std::runtime_error("EVP_DigestUpdate(" + algo_name + ") failed");
    }
}

std::string Hash::get_hash() {
    // checking
    assert(this->_ctx != nullptr);
    assert(this->_algo != nullptr);

    unsigned int len;
    unsigned char *digest = (unsigned char *)malloc(EVP_MD_get_size(this->_algo));
    if (digest == nullptr) {
        // clean up
        EVP_MD_free(this->_algo);
        EVP_MD_CTX_free(this->_ctx);

        // build & throw an error message
        std::string algo_name = algo_to_string(this->name);
        throw new std::runtime_error("malloc(" + algo_name + ") failed");
    }

    // compute the final hash
    if (!EVP_DigestFinal_ex(this->_ctx, digest, &len)) {
        // clean up
        EVP_MD_free(this->_algo);
        EVP_MD_CTX_free(this->_ctx);
        free(digest);

        // build & throw an error message
        std::string algo_name = algo_to_string(this->name);
        throw new std::runtime_error("EVP_DigestFinal_ex(" + algo_name + ") failed");
    }

    // translate to hex
    auto hexdigest = bytes_to_hex(digest, len);

    // cleanup
    free(digest);

    // return
    return hexdigest;
};

} // namespace crypto

#endif // !defined (WINDOWS)
