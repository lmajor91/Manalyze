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

// this just contains export logic for DLLs and SOs

#ifndef __MANAPE_EXPORT__
#define __MANAPE_EXPORT__ 1

#if defined BOOST_WINDOWS_API && !defined DECLSPEC_MANAPE
#ifdef MANACOMMONS_EXPORT
#define DECLSPEC_MANAPE __declspec(dllexport)
#else
#define DECLSPEC_MANAPE __declspec(dllimport)
#endif
#elif !defined BOOST_WINDOWS_API && !defined DECLSPEC_MANAPE
#define DECLSPEC_MANAPE __attribute__((visibility("default")))
#endif

#endif // __MANAPE_EXPORT__
