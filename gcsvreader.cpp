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

#include "gcsvreader.h"

#include <regex> // NOLINT
#include <stdexcept>

#include "gutil.h"

GCSVReader::GCSVReader(std::string path)
    : file_persons{path + '/' + FILENAME_PERSONS},
      file_proxies{path + '/' + FILENAME_PROXIES},
      file_credentials{std::move(path) + '/' + FILENAME_CREDENTIALS}
{
	auto pers = GUtil::check_and_open(file_persons);
	auto prox = GUtil::check_and_open(file_proxies);
	auto cred = GUtil::check_and_open(file_credentials);

	persons_ = GUtil::parse<GPerson>(&pers, [](const auto &l) {
		std::stringstream ss{l};
		GPerson p;
		std::getline(ss, p.email, SEPARATOR);
		std::getline(ss, p.name, SEPARATOR);
		if (p.email.empty() || p.name.empty())
			throw std::runtime_error{"broken file format"};
		return p;
	});
	pers.close();

	std::regex regexp{PROXY_REGEXP_STR};
	proxies_ = GUtil::parse<GProxy>(&prox, [&regexp](const auto &l) {
		GProxy p;
		std::smatch sm;
		std::regex_match(l, sm, regexp);
		if (sm.size() < 5)
			throw std::runtime_error{"broken file format"};
		auto i{0};
		p.addr = sm[++i]; // 1
		p.port = std::stoul(sm[++i]);
		p.username = sm[++i];
		p.password = sm[++i];
		return p;
	});
	prox.close();

	credentials_ = GUtil::parse<GCredential>(&cred, [](const auto &l) {
		std::stringstream ss{l};
		GCredential c;
		std::getline(ss, c.server, SEPARATOR);
		std::getline(ss, c.username, SEPARATOR);
		std::getline(ss, c.password, SEPARATOR);
		std::getline(ss, c.email, SEPARATOR);
		if (c.server.empty() || c.username.empty() ||
		    c.password.empty())
			throw std::runtime_error{"broken file format"};
		return c;
	});
	cred.close();
}

const std::vector<GPerson> &GCSVReader::persons() const { return persons_; }

const std::vector<GProxy> &GCSVReader::proxies() const { return proxies_; }

const std::vector<GCredential> &GCSVReader::credentials() const
{
	return credentials_;
}
