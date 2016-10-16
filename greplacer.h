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

#include "gprovider.h"

template <class P> class GReplacer : public GModifier<std::string>
{
      public:
	explicit GReplacer(std::string pattern) : pattern_{std::move(pattern)}
	{
	}
	std::string modify(std::string str,
			   const std::string &replacement) override
	{
		if (!replace(&str, pattern_, replacement))
			throw std::runtime_error{std::string{"no pattern "} +
						 pattern_ + " found in \'" +
						 str + '\''};
		return std::move(str);
	}

      private:
	const std::string pattern_;

	static bool replace(std::string *str, const std::string &pattern,
			    const std::string &to)
	{
		auto i = str->find(pattern);
		if (i != std::string::npos) {
			str->replace(i, pattern.length(), to);
			return true;
		} else {
			return false;
		}
	}
};
