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

#include "gbodyreader.h"

#include <string>

#include "gutil.h"

void GBodyReader::parse()
{
	auto f = GUtil::check_and_open(file);
	data_ = GUtil::parse<std::string>(
	    &f,
	    [](const auto &s) {
		    if (s[0] == '\n')
			    return decltype(s){std::next(std::begin(s)),
					       std::end(s)};
		    else
			    return s;
	    },
	    '#');
	f.close();
}
