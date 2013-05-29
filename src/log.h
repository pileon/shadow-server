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

namespace backend
{
	class basic_backend
	{
	public:
		basic_backend(const std::string& channel)
			: channel_(channel)
			{ }
		virtual ~basic_backend()
			{ }

		//! Called when starting a logging sequence
		virtual void start() = 0;

		//! Called when ending a logging sequence
		virtual void end() = 0;

		virtual void write(const char*, const std::streamsize) = 0;

		virtual void flush() = 0;

	protected:
		const std::string& channel() const
			{
				return channel_;
			}

	private:
		const std::string channel_;
	};

	// class buffered_backend : public basic_backend
	// {
	// public:
	// 	void write(const char* s, const std::streamsize)
	// 		{
	// 			buffer_ += s;
	// 		}

	// protected:
	// 	const std::string& buffer() const
	// 		{
	// 			return buffer_;
	// 		}

	// private:
	// 	std::string buffer_;
	// };

	class ostream_backend : public basic_backend
	{
	public:
		ostream_backend(const std::string& channel, std::ostream& stream)
			: basic_backend(channel),
			  stream_(stream)
			{ }

		~ostream_backend()
			{ }

		void start()
			{
				stream_ << timestamp() << " :: " << channel() << " :: ";
			}

		void end()
			{ }

		void write(const char* s, const std::streamsize n)
			{
				stream_.write(s, n);
			}

		void flush()
			{
				stream_.flush();
			}

		private:
			std::ostream& stream_;

			const std::string timestamp() const
				{
					return "timestamp";
				}
	};

	// TODO: class window_system_logger;
	// TODO: class posix_system_logger;
	// TODO: Put the above system loggers in the correct `hosts` sub-folder

	using backend_ptr_type = std::shared_ptr<basic_backend>;
	using backend_col_type = std::vector<backend_ptr_type>;

	//! Return the backends for a specific channel
	backend_col_type& backends(const std::string& channel);
} // namespace backends

namespace channel
{
	namespace sink
	{
		class sink : public boost::iostreams::sink
		{
		public:
			sink(backend::backend_col_type& backends)
				: backends_(backends)
				{ }
			sink(const sink& other)
				: backends_(other.backends_)
				{ }
			~sink()
				{
					for (auto& backend : backends_)
						backend->flush();
				}

		std::streamsize write(const char* s, std::streamsize n)
			{
				for (auto& backend : backends_)
					backend->write(s, n);

				return n;
			}

		private:
			backend::backend_col_type& backends_;
		};
	} // namespace sink

	class basic_channel : public boost::iostreams::stream<sink::sink>
	{
	public:
		basic_channel(const std::string& channel)
			: boost::iostreams::stream<sink::sink>(log::backend::backends(channel)),
			  channel_(channel)
			{
				for (auto& backend : backend::backends(channel_))
					backend->start();
			}
		basic_channel(const basic_channel& other)
			: boost::iostreams::stream<sink::sink>(log::backend::backends(other.channel_)),
			  channel_(other.channel_)
			{ }
		virtual ~basic_channel()
			{
				*this << '\n';
				for (auto& backend : backend::backends(channel_))
					backend->end();
			}

	private:
		const std::string channel_;
	};

	class info : public basic_channel
	{
	public:
		info()
			: basic_channel("info")
			{ }
		info(const info& other)
			: basic_channel(other)
			{ }
	};

} // namespace channel

inline channel::info info()
{
	return channel::info{};
}

/* **************************************************************** */

} // namespace log
} // namespace shadow

#endif // LOG_H_
