// -*- mode: C++; coding: utf-8 -*-
#ifndef LOG_H_
#define LOG_H_ 1
/* *******************************************************************
* File: log.h                                  Part of Shadow Server *
*                                                                    *
* Copyright (C) 2012, 2013, Joachim Pileborg and individual          *
* contributors. All rights reserved.                                 *
*                                                                    *
* Redistribution and use in source and binary forms, with or without *
* modification, are permitted provided that the following conditions *
* are met:                                                           *
*                                                                    *
*   o Redistributions of source code must retain the above copyright *
*     notice, this list of conditions and the following disclaimer.  *
*   o Redistributions in binary form must reproduce the above        *
*     copyright notice, this list of conditions and the following    *
*     disclaimer in the documentation and/or other materials         *
*     provided with the distribution.                                *
*   o Neither the name of Shadow World nor the names of its          *
*     contributors may be used to endorse or promote products        *
*     derived from this software without specific prior written      *
*     permission.                                                    *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS  *
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,*
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,           *
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR *
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY       *
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR     *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF *
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF    *
* SUCH DAMAGE.                                                       *
*                                                                    *
******************************************************************* */

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/concepts.hpp>

namespace shadow {
namespace log {

/* **************************************************************** */

void init();
void clean();

/* **************************************************************** */

namespace logging
{
	struct basic_logger
	{
		virtual void write(const char*, const std::streamsize) = 0;
	};

	class ostream_logger : public basic_logger
	{
	public:
		ostream_logger(std::ostream& stream)
			: stream_(stream)
			{ }
		ostream_logger(const ostream_logger& other)
			: stream_(other.stream_)
			{ }

		void write(const char* s, const std::streamsize n)
			{
				stream_.write(s, n);
			}

		private:
			std::ostream& stream_;
	};

	// TODO: class window_system_logger;
	// TODO: class posix_system_logger;
	// TODO: Put the above system loggers in the correct `hosts` sub-folder
}

namespace streams
{
	class log_sink : public boost::iostreams::sink
	{
	public:
		std::streamsize write(const char* s, std::streamsize n)
			{
				// Write up to n characters to the underlying 
				// data sink into the buffer s, returning the 
				// number of characters written

				output_.write(s, n);

				return n;
			}

		log_sink(std::ostream& output) : output_(output)
			{ }
		log_sink(const log_sink& other) : output_(other.output_)
			{ }
		~log_sink()
			{ }

	private:
		std::ostream& output_;
	};

	class log_info : public boost::iostreams::stream<log_sink>
	{
	public:
		log_info(std::ostream& output)
			: boost::iostreams::stream<log_sink>(output),
			  output_(output)
			{ *this << "info :: "; }
		log_info(const log_info& other)
			: boost::iostreams::stream<log_sink>(other.output_),
			  output_(other.output_)
			{ }
		~log_info()
			{ *this << std::endl; }

	private:
		std::ostream& output_;
	};
} // namespace streams

inline streams::log_info info()
{
	return streams::log_info(std::clog);
}

/* **************************************************************** */

} // namespace log
} // namespace shadow

#endif // LOG_H_
