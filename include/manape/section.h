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

#include <stdio.h>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>
#include <boost/system/api_config.hpp>

#include "manape/export.h"
#include "manape/pe_structs.h"
#include "manape/utils.h"
#include "types.h"


// #include "manape/utils.h"

#ifndef __MANEPE_SECTION__
#define __MANEPE_SECTION__ 1

namespace mana::pe {

class Section {
#pragma region private fields
  private:
    // Fields that match the PE structure
    std::string _name;
    boost::uint32_t _virtual_size;
    boost::uint32_t _virtual_address;
    boost::uint32_t _size_of_raw_data;
    boost::uint32_t _pointer_to_raw_data;
    boost::uint32_t _pointer_to_relocations;
    boost::uint32_t _pointer_to_line_numbers;
    boost::uint16_t _number_of_relocations;
    boost::uint16_t _number_of_line_numbers;
    boost::uint32_t _characteristics;

    // Handle to the file on the filesystem.
    pFile _file_handle;
    // Size of the file. This is used to reject sections with a wrong size.
    boost::uint64_t _file_size;
#pragma endregion

#pragma region public methods
  public:
    /**
     *	@brief	Create a Section object from a raw image_section_header structure.
     *
     *	@param	const image_section_header& header The structure on which the section will
     *be based.
     *	@param	pFile handle An open handle to the executable on the filesystem.
     *	@param	const std::vector<pString>& coff_string_table An optional COFF string
     *table, in case section names are located in it.
     */
    DECLSPEC_MANAPE
    Section(const image_section_header &header, pFile handle, boost::uint64_t file_size,
            const std::vector<pString> &coff_string_table = std::vector<pString>());

    DECLSPEC_MANAPE virtual ~Section() {}

    /**
     *	@brief	Returns the raw bytes of the section.
     *
     *	Note that calling this function for PEs which have giant sections may end up
     *	eating a lot of memory.
     *
     *	@return	A shared vector containing the raw bytes of the section. If an error
     *occurs, the vector will be empty.
     */
    DECLSPEC_MANAPE shared_bytes get_raw_data() const;

    DECLSPEC_MANAPE pString get_name() const {
        return boost::make_shared<std::string>(_name);
    }

    DECLSPEC_MANAPE boost::uint32_t get_virtual_size() const { return _virtual_size; }

    DECLSPEC_MANAPE boost::uint32_t get_virtual_address() const {
        return _virtual_address;
    }

    DECLSPEC_MANAPE boost::uint32_t get_size_of_raw_data() const {
        return _size_of_raw_data;
    }

    DECLSPEC_MANAPE boost::uint32_t get_pointer_to_raw_data() const {
        return _pointer_to_raw_data;
    }

    DECLSPEC_MANAPE boost::uint32_t get_pointer_to_relocations() const {
        return _pointer_to_relocations;
    }

    DECLSPEC_MANAPE boost::uint32_t get_pointer_to_line_numbers() const {
        return _pointer_to_line_numbers;
    }

    DECLSPEC_MANAPE boost::uint32_t get_number_of_relocations() const {
        return _number_of_relocations;
    }

    DECLSPEC_MANAPE boost::uint32_t get_number_of_line_numbers() const {
        return _number_of_line_numbers;
    }

    DECLSPEC_MANAPE boost::uint32_t get_characteristics() const {
        return _characteristics;
    }

    DECLSPEC_MANAPE double get_entropy() const {
        return mana::utils::shannon_entropy(*get_raw_data());
    }
#pragma endregion
};
typedef boost::shared_ptr<mana::pe::Section> pSection;

/**
 *	@brief	Checks whether the address belongs to a section.
 *
 *	@param	unsigned int rva The address (RVA) to check.
 *	@param	sg::pSection section The section in which the address may belong.
 *	@param	bool check_raw_size Use the SizeOfRawData instead of VirtualSize to determine
 *the bounds of the section.
 *
 *	@return	Whether the RVA is between the bounds of the section.
 */
DECLSPEC_MANAPE bool is_address_in_section(boost::uint64_t rva, pSection section,
                                           bool check_raw_size = false);

/**
 *	@brief	Finds the section containing a given RVA.
 *
 *	@param	unsigned int rva The address whose section we want to identify.
 *	@param	const std::vector<sg::pSection>& section_list A list of all the sections of
 *the PE.
 *
 *	@return	A pointer to the section containing the input address. NULL if no sections
 *match.
 */
DECLSPEC_MANAPE pSection find_section(unsigned int rva,
                                      const std::vector<pSection> &section_list);

} // namespace mana::pe

#endif // __MANEPE_SECTION__
