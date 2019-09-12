#pragma once
#include "Timestamp.h"
#include "getTimeOfDay.h"
#include <inttypes.h>//for PRId64
#include <time.h>



namespace Alime
{
	

	inline Timestamp::Timestamp()
		: ns_(0) {}

	inline Timestamp::Timestamp(int64_t nanoseconds)
		: ns_(nanoseconds) {}

	inline bool Timestamp::IsEpoch() const {
		return ns_ == 0;
	}

	inline Timestamp::Timestamp(const struct timeval& t)
		: ns_(static_cast<int64_t>(t.tv_sec) * Duration::kSecond + t.tv_usec * Duration::kMicrosecond) {}

	inline Timestamp Timestamp::Now()
	{

#if  ALIME_HAS_NOT_CPP11_OR_HIGHER
		return Timestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
#else
		return Timestamp(static_cast<int64_t>(utcmicrosecond() * Duration::kMicrosecond));
#endif
	}

	inline void Timestamp::Add(Duration d) {
		ns_ += d.Nanoseconds();
	}

	inline void Timestamp::To(struct timeval* t) const {
		t->tv_sec = static_cast<long>(ns_ / Duration::kSecond);
		t->tv_usec = static_cast<long>(ns_ % Duration::kSecond) / static_cast<long>(Duration::kMicrosecond);
	}

	inline struct timeval Timestamp::TimeVal() const {
		struct timeval t;
		To(&t);
		return t;
	}

	inline int64_t Timestamp::Unix() const {
		return ns_ / Duration::kSecond;
	}

	inline int64_t Timestamp::UnixNano() const {
		return ns_;
	}

	inline int64_t Timestamp::UnixMicro() const {
		return ns_ / Duration::kMicrosecond;
	}

	inline bool Timestamp::operator< (const Timestamp& rhs) const {
		return ns_ < rhs.ns_;
	}

	inline bool Timestamp::operator==(const Timestamp& rhs) const {
		return ns_ == rhs.ns_;
	}

	inline Timestamp Timestamp::operator+=(const Duration& rhs) {
		ns_ += rhs.Nanoseconds();
		return *this;
	}

	inline Timestamp Timestamp::operator+(const Duration& rhs) const {
		Timestamp temp(*this);
		temp += rhs;
		return temp;
	}

	inline Timestamp Timestamp::operator-=(const Duration& rhs) {
		ns_ -= rhs.Nanoseconds();
		return *this;
	}

	inline Timestamp Timestamp::operator-(const Duration& rhs) const {
		Timestamp temp(*this);
		temp -= rhs;
		return temp;
	}

	inline Duration Timestamp::operator-(const Timestamp& rhs) const {
		int64_t ns = ns_ - rhs.ns_;
		return Duration(ns);
	}

	//linux只能精确到毫秒
	std::string Timestamp::toString() const
	{
		char buf[32] = { 0 };
		int64_t seconds = ns_ / Duration::kSecond;
		int64_t microseconds = ns_ % Duration::kMicrosecond;
		snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
		return buf;
	}

	std::string Timestamp::toFormattedString(bool showMicroseconds) const
	{
		char buf[64] = { 0 };
		time_t seconds = static_cast<time_t>(ns_ / Duration::kSecond);
		struct tm tm_time;
		//gmtime_r(&seconds, &tm_time);
		gmtime_s(&tm_time, &seconds);

		if (showMicroseconds)
		{
			int microseconds = static_cast<int>((ns_% Duration::kSecond) / 1000);
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
				microseconds);
		}
		else
		{
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		}
		return buf;
	}

	bool Timestamp::valid() const
	{
		return  ns_ > 0;
	}

	Timestamp Timestamp::invalid()
	{
		return Timestamp();
	}
}