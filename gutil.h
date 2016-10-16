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

#include <fstream>
#include <functional>
#include <string>
#include <vector>

namespace GUtil
{

std::string intro();

std::string version();

bool file_exist(const std::string &filename);

std::ifstream check_and_open(const std::string &f);

template <class T>
std::vector<T> parse(std::ifstream *s,
		     std::function<T(const std::string &)> lineparser,
		     char delim = '\n')
{
	std::vector<T> data;
	std::string buf;
	while (!s->eof()) {
		std::getline(*s, buf, delim);
		if (!buf.empty())
			data.push_back(lineparser(buf));
	}
	return data;
}

} // namespace GUtil
