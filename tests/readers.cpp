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

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "gbodyreader.h"
#include "gcsvreader.h"
#include "gheaderreader.h"

using namespace std::string_literals;

class Reader_F : public ::testing::Test
{
      protected:
	static constexpr auto tmpdir{"/tmp/"};
	void SetUp() override {}

	void TearDown() override
	{
		for (auto &file : files) {
			delete_file(file);
		}
	}

	void file(auto name, auto content)
	{
		create_file(name, std::forward<decltype(content)>(content));
		files.push_back(std::move(name));
	}

      private:
	static void create_file(std::string name, auto content)
	{
		std::ofstream f;
		f.open(tmpdir + name);
		f << content;
		f.close();
	}

	static void delete_file(std::string name)
	{
		std::remove(std::string{tmpdir + name}.c_str());
	}

	std::vector<std::string> files;
};

TEST_F(Reader_F, csv)
{
	file(GCSVReader::FILENAME_PERSONS, "123@mail.com,Name1\n"
					   "456@kek.de,Name2\n"
					   "7a8b9c@123.org,Name3\n"s);
	file(GCSVReader::FILENAME_PROXIES, "127.0.0.1:123,user1,pwd1\n"
					   "google.com:8888,user2,pwd2\n"
					   "server:3128,user3,pwd3\n"s);
	file(GCSVReader::FILENAME_CREDENTIALS,
	     "smtp.yandex.ru,goofmail.test,pwd,goofmail.test@yandex.ru\n"
	     "smtp.yandex.ru,u2,p2,u2@yandex.ru\n"s);

	try {
		GCSVReader r{tmpdir};
		{
			auto &p = r.persons();
			ASSERT_EQ(3, p.size());

			ASSERT_EQ("123@mail.com", p[0].email);
			ASSERT_EQ("Name1", p[0].name);

			ASSERT_EQ("456@kek.de", p[1].email);
			ASSERT_EQ("Name2", p[1].name);

			ASSERT_EQ("7a8b9c@123.org", p[2].email);
			ASSERT_EQ("Name3", p[2].name);
		}
		{
			auto p = r.proxies();
			ASSERT_EQ(3, p.size());

			ASSERT_EQ("127.0.0.1", p[0].addr);
			ASSERT_EQ(123, p[0].port);
			ASSERT_EQ("user1", p[0].username);
			ASSERT_EQ("pwd1", p[0].password);

			ASSERT_EQ("google.com", p[1].addr);
			ASSERT_EQ(8888, p[1].port);
			ASSERT_EQ("user2", p[1].username);
			ASSERT_EQ("pwd2", p[1].password);

			ASSERT_EQ("server", p[2].addr);
			ASSERT_EQ(3128, p[2].port);
			ASSERT_EQ("user3", p[2].username);
			ASSERT_EQ("pwd3", p[2].password);
		}
		{
			auto c = r.credentials();
			ASSERT_EQ(2, c.size());

			ASSERT_EQ("smtp.yandex.ru", c[0].server);
			ASSERT_EQ("goofmail.test", c[0].username);
			ASSERT_EQ("pwd", c[0].password);
			ASSERT_EQ("goofmail.test@yandex.ru", c[0].email);

			ASSERT_EQ("smtp.yandex.ru", c[1].server);
			ASSERT_EQ("u2", c[1].username);
			ASSERT_EQ("p2", c[1].password);
			ASSERT_EQ("u2@yandex.ru", c[1].email);
		}
	} catch (const std::exception &e) {
		std::cerr << e.what();
		throw;
	}
}

TEST_F(Reader_F, headers)
{
	file(GHeaderReader::FILENAME, "Hi, user!\n"
				      "Allo!\n"
				      "test\n");

	try {
		GHeaderReader r{tmpdir};
		{
			auto &h = r.data();
			ASSERT_EQ(3, h.size());

			ASSERT_EQ("Hi, user!", h[0]);
			ASSERT_EQ("Allo!", h[1]);
			ASSERT_EQ("test", h[2]);
		}
	} catch (const std::exception &e) {
		std::cerr << e.what();
		throw;
	}
}

TEST_F(Reader_F, bodies)
{
	auto body0 = "First line,\n"
		     "\n"
		     "Third one, many words here.\n"
		     "\n"
		     "Conclusion.\n";
	auto body1 = "Body 1\n"
		     "Content is here!\n"
		     "Bye!\n";

	auto body2 = ":3\n"
		     "<some> nice ?ym6o1$\n";

	auto delim = "#\n"s;

	file(GBodyReader::FILENAME,
	     ""s + body0 + delim + body1 + delim + body2);

	try {
		GBodyReader r{tmpdir};
		{
			auto &b = r.data();
			ASSERT_EQ(3, b.size());

			ASSERT_EQ(body0, b[0]);
			ASSERT_EQ(body1, b[1]);
			ASSERT_EQ(body2, b[2]);
		}
	} catch (const std::exception &e) {
		std::cerr << e.what();
		throw;
	}
}
