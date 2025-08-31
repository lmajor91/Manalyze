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

class Hash {
  private:
    std::string algo;

  public:
    // get the name of the algorithm
    // md5, sha1, etc
    std::string getAlgorithm() {
        return this->algo;
    }

    // constructor
    virtual ~Hash() {};

    /// compute hash of a memory block
    virtual std::string operator()(const void *data, size_t numBytes);

    /// compute hash of a string, excluding final zero
    virtual std::string operator()(const std::string &text);

    /// add arbitrary number of bytes
    virtual void add(const void *data, size_t numBytes) = 0;

    /// return latest hash as hex characters
    virtual std::string getHash() = 0;

    /// restart
    virtual void reset() = 0;
};
