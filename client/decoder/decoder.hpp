/***
    This file is part of snapcast
    Copyright (C) 2014-2019  Johannes Pohl

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef DECODER_H
#define DECODER_H
#ifndef ESP_PLATFORM
#include "common/sample_format.hpp"
#include "message/codec_header.hpp"
#include "message/pcm_chunk.hpp"
#else
#include <sample_format.hpp>
#include <message/codec_header.hpp>
#include <message/pcm_chunk.hpp>
#endif
#include <mutex>

namespace decoder
{

class Decoder
{
public:
    Decoder(){};
    virtual ~Decoder() = default;

    virtual bool decode(msg::PcmChunk* chunk) = 0;
    virtual SampleFormat setHeader(msg::CodecHeader* chunk) = 0;

protected:
    std::mutex mutex_;
};

} // namespace decoder

#endif
