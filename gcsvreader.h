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

#include "gcredential.h"
#include "gcredentialsreader.h"
#include "gperson.h"
#include "gproxy.h"

class GCSVReader : public GCredentialsReader<GPerson, GProxy, GCredential>
{
	friend class Reader_F_csv_Test;

      public:
	explicit GCSVReader(std::string path);

	const std::vector<GPerson> &persons() const final;
	const std::vector<GProxy> &proxies() const final;
	const std::vector<GCredential> &credentials() const final;

      private:
	constexpr static auto FILENAME_PERSONS = "persons.csv";
	constexpr static auto FILENAME_PROXIES = "proxies.csv";
	constexpr static auto FILENAME_CREDENTIALS = "smtp.csv";
	constexpr static auto SEPARATOR = ',';
	constexpr static auto PROXY_REGEXP_STR =
	    R"(^([\S]+):([\d]+),([^,]*),([^,]*)$)";

	const std::string file_persons;
	const std::string file_proxies;
	const std::string file_credentials;

	std::vector<GPerson> persons_;
	std::vector<GProxy> proxies_;
	std::vector<GCredential> credentials_;
};
