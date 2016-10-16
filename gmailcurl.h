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

#include "gcredential.h"
#include "gproxy.h"

class GMailCurl
{
      public:
	explicit GMailCurl();
	~GMailCurl();

	void set_credentials(const GCredential &credentials, bool ssl = true);
	void set_proxy(const GProxy &proxy);

	void send(const std::string &addr, const std::string &header,
		  const std::string &body);

      private:
	constexpr static auto TIMEOUT = 30;

	void *curl;
	std::string from_;
};
