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

#include "gutil.h"

#include <sys/stat.h>

#include <fstream>
#include <stdexcept>
#include <string>

std::string GUtil::intro()
{
	return std::string{"goofmail "} + version() + '\n' +
	       R"(
  Copyright (C) 2016  Viktor Filinkov

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.)";
}

std::string GUtil::version() { return "0.1-alpha"; }

bool GUtil::file_exist(const std::string &filename)
{
	struct stat buf;
	return (stat(filename.c_str(), &buf) == 0);
}

std::ifstream GUtil::check_and_open(const std::string &f)
{
	if (!file_exist(f))
		throw std::runtime_error{"can't find file " + f};
	std::ifstream s;
	s.open(f);
	if (!s.is_open())
		throw std::runtime_error{"can't open file " + f};
	return s;
}
