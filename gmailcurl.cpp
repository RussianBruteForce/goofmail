#include "gmailcurl.h"

#include <curl/curl.h>
#include <stdio.h>

#include <stdexcept>

GMailCurl::GMailCurl() : curl{curl_easy_init()}
{
	if (!curl)
		throw std::runtime_error{"currl init error"};

	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);
	// debug info
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
}

GMailCurl::~GMailCurl() { curl_easy_cleanup(curl); }

void GMailCurl::set_credentials(const GCredential &credentials, bool ssl)
{
	curl_easy_setopt(curl, CURLOPT_USERNAME, credentials.username.c_str());
	curl_easy_setopt(curl, CURLOPT_PASSWORD, credentials.password.c_str());

	from_ = credentials.email;
	if (!from_.empty()) {
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM,
				 credentials.email.c_str());
	} else {
		from_ = "Mr. John Doe";
	}

	std::string addr;
	if (ssl) {
		addr = "smtps://" + credentials.server;
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	} else {
		addr = "smtp://" + credentials.server;
	}
	curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
}

void GMailCurl::set_proxy(const GProxy &proxy)
{
	curl_easy_setopt(curl, CURLOPT_PROXY, proxy.addr.c_str());
	curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxy.port);
	curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
	if (!proxy.username.empty()) {
		// auth
	}
}

void GMailCurl::send(const std::string &addr, const std::string &header,
		     const std::string &body)
{
	struct curl_slist *recipients = nullptr;
	recipients = curl_slist_append(recipients, addr.c_str());
	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

	// NOLINTNEXTLINE
	auto payload_gen = [&]() {
		return "To: " + addr + "\r\n"
				       "From: " +
		       from_ + "(Example User)\r\n"
			       "Subject: " +
		       header + "\r\n\r\n" + body;
	};
	auto data = payload_gen();
	FILE *f = fmemopen(const_cast<char *>(data.data()), data.size(), "r");
	curl_easy_setopt(curl, CURLOPT_READDATA, f);

	auto status = curl_easy_perform(curl);
	if (status != CURLE_OK) {
		curl_slist_free_all(recipients);
		throw std::runtime_error{std::string{"curl error: "} +
					 curl_easy_strerror(status)};
	}
	curl_slist_free_all(recipients);
}
