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

#ifndef WIRE_CHUNK_H
#define WIRE_CHUNK_H

#ifndef ESP_PLATFORM
#include "common/time_defs.hpp"
#else
#include <time_defs.hpp>
#endif
#include "message.hpp"
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <streambuf>
#include <vector>


namespace msg
{

/**
 * Piece of raw data
 * Has information about "when" captured (timestamp)
 */
class WireChunk : public BaseMessage
{
public:
    WireChunk(size_t size = 0) : BaseMessage(message_type::kWireChunk), payloadSize(size), payload(nullptr)
    {
        if (size > 0)
            payload = (char*)malloc(size * sizeof(char));
    }

    WireChunk(const WireChunk& wireChunk) : BaseMessage(message_type::kWireChunk), timestamp(wireChunk.timestamp), payloadSize(wireChunk.payloadSize)
    {
        payload = (char*)malloc(payloadSize);
        memcpy(payload, wireChunk.payload, payloadSize);
    }

    ~WireChunk() override
    {
        free(payload);
    }

    void read(std::istream& stream) override
    {
        readVal(stream, timestamp.sec);
        readVal(stream, timestamp.usec);
        readVal(stream, &payload, payloadSize);
    }

    uint32_t getSize() const override
    {
        return sizeof(tv) + sizeof(int32_t) + payloadSize;
    }

    virtual chronos::time_point_clk start() const
    {
        return chronos::time_point_clk(chronos::sec(timestamp.sec) + chronos::usec(timestamp.usec));
    }

    tv timestamp;
    uint32_t payloadSize;
    char* payload;

protected:
    void doserialize(std::ostream& stream) const override
    {
        writeVal(stream, timestamp.sec);
        writeVal(stream, timestamp.usec);
        writeVal(stream, payload, payloadSize);
    }
};
} // namespace msg


#endif
