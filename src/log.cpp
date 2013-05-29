// coding: utf-8 -*-
/* *******************************************************************
* File: shadow.h                               Part of Shadow Server *
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

#include "shadow.h"
#include "log.h"
#include <unordered_map>

namespace shadow {
namespace log {

/* **************************************************************** */

namespace
{
	std::unordered_map<std::string, backend::backend_col_type> channels;
}

/* **************************************************************** */

/* **************************************************************** */

void init()
{
	// By default use the `std::clog` output stream for the backend
	// TODO: The backends have to be configurable at compile-time,
	//       boot-time and run-time.
	channels["info" ].emplace_back(std::make_shared<backend::ostream_backend>("info" , std::clog));
	channels["debug"].emplace_back(std::make_shared<backend::ostream_backend>("debug", std::clog));

	log::info() << "hello world from logging";
	log::debug() << "Yay" << ' ' << "it works!";
}

void clean()
{

}

/* **************************************************************** */

backend::backend_col_type& backend::backends(const std::string& channel)
{
	if (channels.find(channel) != channels.end())
		return channels.at(channel);
	else
	{
		static backend_col_type empty;
		return empty;
	}
}

/* **************************************************************** */

} // namespace log
} // namespace shadow
