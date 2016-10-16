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

#include "gmailcurl.h"

TEST(Mail, DISABLED_basic)
{
	GMailCurl my;
	my.set_credentials({"smtp.yandex.ru", "goofmail.test", "xxx",
			    "goofmail.test@yandex.ru"});
	ASSERT_THROW(my.send("k@g.de", "hi!", "test \nbody\n:)"),
		     std::exception);

	GMailCurl m;
	m.set_credentials(
	    {"smtp.mailgun.org",
	     "postmaster@sandbox3d74f644107e4c96915a885cd1e664b8.mailgun.org",
	     "xxx",
	     "postmaster@sandbox3d74f644107e4c96915a885cd1e664b8.mailgun.org"});
	ASSERT_THROW(m.send("xxx@s.ru", "MAILGUN MSG", "test mailgun!"),
		     std::exception);
}
