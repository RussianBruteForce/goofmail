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

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "gprovider.h"
#include "greplacer.h"

using cont_t = std::vector<std::string>;

TEST(Provider, basic)
{
	const cont_t data = {"str1", "str2", "str3"};

	try {
		GProvider<cont_t> p{data};
		ASSERT_EQ(data[0], p.next());
		ASSERT_EQ(data[1], p.next());
		ASSERT_EQ(data[2], p.next());
		ASSERT_EQ(data[0], p.next());
		ASSERT_EQ(data[1], p.next());
		ASSERT_EQ(data[2], p.next());
		ASSERT_EQ(data[0], p.next());
	} catch (const std::exception &e) {
		std::cerr << e.what();
		throw;
	}
}

TEST(Provider, replace)
{
	const cont_t data = {"str1 <name> end1", "str2 <name> end2",
			     "str3 <name> end3"};

	const cont_t names = {"name1", "name2", "name3"};

	try {
		const GProvider<cont_t> name_provider{names};

		GProvider<cont_t> data_provider{
		    data, std::make_unique<GReplacer<cont_t>>("<name>")};

		ASSERT_EQ("str1 name1 end1",
			  data_provider.next(name_provider.next()));
		ASSERT_EQ("str2 name2 end2",
			  data_provider.next(name_provider.next()));
		ASSERT_EQ("str3 name3 end3",
			  data_provider.next(name_provider.next()));
	} catch (const std::exception &e) {
		std::cerr << e.what();
		throw;
	}
}
