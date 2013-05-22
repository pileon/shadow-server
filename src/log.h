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
} // namespace streams

/* **************************************************************** */

} // namespace log
} // namespace shadow

#endif // LOG_H_
