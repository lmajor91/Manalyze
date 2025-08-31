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

#ifndef __MANACOMMONS_CRYPTO__
#define __MANACOMMONS_CRYPTO__ 1

#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_array.hpp>
#include <boost/system/api_config.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "hash.h"
#include "types.h"
#include "manape/pe.h"

namespace crypto {

/**
 * @brief Gets a reference to a hashing algorithm by an enum.
 * @returns An object for hashing.
 */
CRYPTOLIB_API boost::shared_ptr<Hash> get_algorithm(crypto::algorithm_t algo);

/**
 * @brief Converts a series of bytes into hexadecimal, all lowercase.
 * @returns The hexadecimal string of bytes.
 */
CRYPTOLIB_API const std::string bytes_to_hex(const void *data, size_t len);

/**
 * @brief Gets the proper name of a hashing algorithm.
 * @returns The name of the hashing algorithm this object is using.
 */
CRYPTOLIB_API const std::string algo_to_string(crypto::algorithm_t algo);


/**
 *	@brief	Computes the hash of a buffer.
 *
 *	@param	Digest& digest The digest to use.
 *	@param	const std::vector<boost::uint8_t>& bytes The buffer to hash.
 *
 *	@return	A shared string containing the hash value. May be empty if an error occurred.
 */
CRYPTOLIB_API pString hash_bytes(Hash &digest, const std::vector<boost::uint8_t> &bytes);

/**
 *	@brief	Computes the hash of a file.
 *
 *	@param	Digest& digest The digest to use.
 *	@param	const std::string& filename The path to the file to hash.
 *
 *	@return	A string containing the hash value. May be empty if an error occurred.
 */
CRYPTOLIB_API pString hash_file(Hash &digest, const std::string &filename);

/**
 *	@brief	Computes the hashes of a file.
 *
 *	This function is used to calculate multiple hashes of the same file in a single pass.
 *
 *	@param	const std::vector<pDigest>& digests A list of digests to use.
 *			hash::ALL_DIGESTS is a suitable pre-initialized vector given for convenience.
 *
 *	@param	const std::string& filename The path to the file to hash.
 *
 *	@return	A shared vector containing all the computed hashes, in the same order as the
 *input digests. If an error occurs for any digest, the return value's size is set to 0.
 */
CRYPTOLIB_API const_shared_strings hash_file(const std::vector<pHash> &digests,
                                             const std::string &filename);

/**
 *	@brief	Computes the hashes of a buffer.
 *
 *	@param	const std::vector<pDigest>& digests A list of digests to use.
 *			hash::ALL_DIGESTS is a suitable pre-initialized vector given for convenience.
 *
 *	@param	const std::vector<boost::uint8_t>& bytes The buffer to hash.
 *
 *	@return	A shared vector containing all the computed hashes, in the same order as the
 *input digests. If an error occurs for any digest, the return value's size is set to 0.
 */
CRYPTOLIB_API const_shared_strings hash_bytes(const std::vector<pHash> &digests,
                                              const std::vector<boost::uint8_t> &bytes);

/**
 *	@brief	Computes the hash of a PE's imports.
 *	Per http://www.mandiant.com/blog/tracking-malware-import-hashing/
 *
 *	@return	A MD5 hash of the ordered import list.
 *
 *	Implementation is located in imports.cpp.
 */
CRYPTOLIB_API const std::string hash_imports(const mana::PE& pe);

} // namespace crypto

#endif // __MANACOMMONS_CRYPTO__
