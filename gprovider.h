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

#include <functional>
#include <memory>
#include <stdexcept>

template <class T> struct GModifier {
	virtual ~GModifier() = default;
	virtual T modify(T, const T &) = 0;
};

template <class T> class GModifierL : public GModifier<T>
{
	std::function<T(T, const T &)> lambda;

      public:
	GModifierL(decltype(lambda) l) : lambda{l} {}

	T modify(T str, const T &replacement) override
	{
		return lambda(str, replacement);
	}
};

template <class T> class GProvider
{
      public:
	explicit GProvider(
	    const T &data,
	    std::unique_ptr<GModifier<typename T::value_type>> modifier =
		std::unique_ptr<GModifier<typename T::value_type>>())
	    : data_{data}, modifier_{std::move(modifier)}
	{
		if (data_.empty())
			throw std::runtime_error{"empty data provided"};
		i = std::begin(data_);
	}

	typename T::value_type next() const
	{
		auto ret = *i;
		++i;
		if (i == std::end(data_)) {
			i = std::begin(data_);
		}
		return ret;
	}

	typename T::value_type
	next(const typename T::value_type &replacement) const
	{
		auto ret = *i;
		++i;
		if (i == std::end(data_)) {
			i = std::begin(data_);
		}
		if (!modifier_) {
			return ret;
		} else {
			return modifier_->modify(ret, replacement);
		}
	}

      private:
	const T &data_;
	std::unique_ptr<GModifier<typename T::value_type>> modifier_;

	mutable typename T::const_iterator i;
};
