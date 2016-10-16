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

#include <iostream>

#include "gbodyreader.h"
#include "gcsvreader.h"
#include "gheaderreader.h"
#include "gmailcurl.h"
#include "greplacer.h"
#include "gutil.h"

using namespace std;

std::string pattern = "<name>"; // NOLINT

void status(auto s) { cout << "\n <###> " << s << endl; }

void err(auto s) { cerr << "\n <!!!> " << s << endl; }

int main(int argc, char **argv)
{
	status(GUtil::intro());

	if (argc != 2) {
		err("please, specify path like that:\n\t" +
		    std::string{argv[0]} + " <path>");
		return 666;
	}
	std::string path{argv[1]};

	status("init…");
	try {
		GCSVReader csv{path};
		GHeaderReader h{path};
		GBodyReader b{path};

		const GProvider<std::vector<GProxy>> proxies{csv.proxies()};
		const GProvider<std::vector<GCredential>> credentials{
		    csv.credentials()};
		using cont_t = std::vector<std::string>;
		GProvider<cont_t> headers{
		    h.data(), std::make_unique<GReplacer<cont_t>>(pattern)};
		GProvider<cont_t> bodies{
		    b.data(), std::make_unique<GReplacer<cont_t>>(pattern)};

		status("init OK\nstarting job…");
		for (const auto &p : csv.persons()) {
			status("trying send mail to <" + p.name + '>');
			try {
				GMailCurl m;
				m.set_credentials(credentials.next());
				m.set_proxy(proxies.next());
				m.send(p.email, headers.next(p.name),
				       bodies.next(p.name));
				status("OK");
			} catch (const std::exception &e) {
				err(e.what());
			}
		}
	} catch (const std::exception &e) {
		err(e.what());
	} catch (...) {
		err("unknown error");
	}
	status("bye!");
	return 0;
}
