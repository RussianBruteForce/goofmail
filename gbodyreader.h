/*
 *     This file is part of goofmail.
 *
 *    goofmail is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    goofmail is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with goofmail.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>
#include <vector>

#include "gdatareader.h"

class GBodyReader : public GDataReader<std::vector<std::string>>
{
	friend class Reader_F_bodies_Test;

      public:
	explicit GBodyReader(std::string path)
	    : GDataReader(FILENAME, std::move(path))
	{
		parse();
	}

      private:
	constexpr static auto FILENAME = "bodies";
	void parse();
};
