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
#include <utility>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_array.hpp>
#include <boost/system/api_config.hpp>

#if !defined(WINDOWS)
#include <sys/stat.h>
#endif

#include "manape/pe_structs.h"
#include "manape/utils.h"

#ifndef __MANAPE_RESOURCES__
#define __MANAPE_RESOURCES__ 1

namespace mana::pe {

class Resource {
#pragma region private fields
  private:
    std::string _type;

    // Resources can either have an identifier or a name.
    std::string _name;
    boost::uint32_t _id;

    std::string _language;
    boost::uint32_t _codepage;
    boost::uint32_t _size;

    // Keep the timestamp provided in the resource directory
    boost::uint32_t _timestamp;

    // These fields do not describe the PE structure.
    unsigned int _offset_in_file;
    std::string _path_to_pe;

    /**
     *	@brief	Opens the PE file and sets the cursor to the resource bytes.
     *
     *	@return	A file object with its cursor correctly set, or NULL if there was an
     *error.
     */
    FILE *_reach_data() const;
#pragma endregion

#pragma region public methods
  public:
    DECLSPEC_MANAPE Resource(std::string type, std::string name, std::string language,
             boost::uint32_t codepage, boost::uint32_t size, boost::uint32_t timestamp,
             boost::uint32_t offset_in_file, std::string path_to_pe)
        : _type(std::move(type)), _name(std::move(name)), _language(std::move(language)),
          _codepage(codepage), _size(size), _timestamp(timestamp),
          _offset_in_file(offset_in_file), _path_to_pe(std::move(path_to_pe)), _id(0) {}

    DECLSPEC_MANAPE Resource(std::string type, boost::uint32_t id, std::string language,
             boost::uint32_t codepage, boost::uint32_t size, boost::uint32_t timestamp,
             boost::uint32_t offset_in_file, std::string path_to_pe)
        : _type(std::move(type)), _name(), _language(std::move(language)),
          _codepage(codepage), _offset_in_file(offset_in_file), _size(size),
          _timestamp(timestamp), _path_to_pe(std::move(path_to_pe)), _id(id) {}

    DECLSPEC_MANAPE virtual ~Resource() = default;

    DECLSPEC_MANAPE pString get_type() const {
        return boost::make_shared<std::string>(_type);
    }
    DECLSPEC_MANAPE pString get_language() const {
        return boost::make_shared<std::string>(_language);
    }

    DECLSPEC_MANAPE boost::uint32_t get_codepage() const { return _codepage; }
    DECLSPEC_MANAPE boost::uint32_t get_size() const { return _size; }
    DECLSPEC_MANAPE boost::uint32_t get_id() const { return _id; }
    DECLSPEC_MANAPE boost::uint32_t get_offset() const { return _offset_in_file; }
    DECLSPEC_MANAPE boost::uint32_t get_timestamp() const { return _timestamp; }

    DECLSPEC_MANAPE double get_entropy() const {
        return utils::shannon_entropy(*get_raw_data());
    }

    DECLSPEC_MANAPE pString get_name() const {
        if (!_name.empty()) {
            return boost::make_shared<std::string>(_name);
        } else {
            std::stringstream ss;
            ss << _id;
            return boost::make_shared<std::string>(ss.str());
        }
    }

    /**
     *	@brief	Retrieves the raw bytes of the resource.
     *
     *	@return	A vector containing the read bytes. Its size may be 0 if
     *			the resource could not be read.
     */
    DECLSPEC_MANAPE shared_bytes get_raw_data() const;

    /**
     *	@brief	Interprets the resource as a given type.
     *
     *	All the work is performed in one of the template specializations.
     *	Currently, the following interpretations are implemented:
     *	* pString for RT_MANIFEST
     *	* const_shared_strings for RT_STRING
     *	* pgroup_icon_directory_t for RT_GROUP_ICON and RT_GROUP_CURSOR
     *	* pbitmap for RT_BITMAP
     *  * pversion_info for RT_VERSION
     *	* shared_bytes for all resource types (equivalent to  get_raw_data()).
     *
     *	@tparam	T The type into which the resource should be interpreted.
     *
     *	@return	An instance of T representing the resource.
     */
    template <class T> T DECLSPEC_MANAPE interpret_as();

    /**
     * @brief   Extracts the resource to the specified path.
     *
     * @param   const boost::filesystem::path& destination The place where the resource
     * should be written (i.e. "/tmp/image.bmp").
     */
    DECLSPEC_MANAPE bool extract(const boost::filesystem::path &destination);

    /**
     * @brief   Extraction function dedicated to icons.
     *
     * Icon data may be spread over multiple resources. For this reason, it is necessary
     * to have access to all the resources of the PE files to reconstruct it properly.
     *
     * @param   const boost::filesystem::path& destination The place where the resource
     * should be written (i.e. "/tmp/icon.ico").
     */
    DECLSPEC_MANAPE bool
    icon_extract(const boost::filesystem::path &destination,
                 const std::vector<boost::shared_ptr<Resource>> &resources);
#pragma endregion
};
typedef boost::shared_ptr<Resource> pResource;

/**
 *	@brief	Recreates a .ico from resources.
 *
 *	@param	pgroup_icon_directory directory The RT_ICON_GROUP of the icon we want to
 *recreate.
 *	@param	const std::vector<pResource>& resources The resources of the PE.
 *
 *	We need to have access to the other resources, because the image data is spread across
 *	several of them. This function will find the relevant RT_ICON in the vector.
 *
 *	@return	The reconstructed file bytes.
 */
DECLSPEC_MANAPE shared_bytes reconstruct_icon(pgroup_icon_directory directory,
                                              const std::vector<pResource> &resources);

/**
 *	@brief	Parses a VERSION_INFO_HEADER, which is not a standard structure but does come
 *up a lot.
 *
 *	@param	vs_version_info_header& header The structure to fill.
 *	@param	FILE* f An opened file to read from. The cursor has to be set to the right
 *offset and will be updated.
 *
 *	@return	Whether the structure was read successfully.
 */
DECLSPEC_MANAPE bool parse_version_info_header(vs_version_info_header &header, FILE *f);

} // namespace mana::pe

#endif // __MANAPE_RESOURCES__
