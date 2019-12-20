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

#ifndef PCM_CHUNK_H
#define PCM_CHUNK_H

#ifndef ESP_PLATFORM
#include "common/sample_format.hpp"
#else
#include <sample_format.hpp>
#endif
#include "message.hpp"
#include "wire_chunk.hpp"
#include <chrono>


namespace msg
{

/**
 * Piece of PCM data with SampleFormat information
 * Has information about "when" recorded (start) and duration
 * frames can be read with "readFrames", which will also change the start time
 */
class PcmChunk : public WireChunk
{
public:
    PcmChunk(const SampleFormat& sampleFormat, size_t ms) : WireChunk(sampleFormat.rate * sampleFormat.frameSize * ms / 1000), format(sampleFormat), idx_(0)
    {
    }

    PcmChunk(const PcmChunk& pcmChunk) : WireChunk(pcmChunk), format(pcmChunk.format), idx_(0)
    {
    }

    PcmChunk() : WireChunk(), idx_(0)
    {
    }

    ~PcmChunk() override = default;

#if 0
    template <class Rep, class Period>
    int readFrames(void* outputBuffer, const std::chrono::duration<Rep, Period>& duration)
    {
        auto us = std::chrono::microseconds(duration).count();
        auto frames = (us * 48000) / std::micro::den;
        // return readFrames(outputBuffer, (us * 48000) / std::micro::den);
        return frames;
    }
#endif

    int readFrames(void* outputBuffer, size_t frameCount)
    {
        // logd << "read: " << frameCount << ", total: " << (wireChunk->length / format.frameSize) << ", idx: " << idx;// << std::endl;
        int result = frameCount;
        if (idx_ + frameCount > (payloadSize / format.frameSize))
            result = (payloadSize / format.frameSize) - idx_;

        // logd << ", from: " << format.frameSize*idx << ", to: " << format.frameSize*idx + format.frameSize*result;
        if (outputBuffer != nullptr)
            memcpy((char*)outputBuffer, (char*)(payload) + format.frameSize * idx_, format.frameSize * result);

        idx_ += result;
        // logd << ", new idx: " << idx << ", result: " << result << ", wireChunk->length: " << wireChunk->length << ", format.frameSize: " << format.frameSize
        // << "\n";//std::endl;
        return result;
    }

    int seek(int frames)
    {
        if ((frames < 0) && (-frames > (int)idx_))
            frames = -idx_;

        idx_ += frames;
        if (idx_ > getFrameCount())
            idx_ = getFrameCount();

        return idx_;
    }


    chronos::time_point_clk start() const override
    {
        return chronos::time_point_clk(chronos::sec(timestamp.sec) + chronos::usec(timestamp.usec) +
                                       chronos::usec((chronos::usec::rep)(1000000. * ((double)idx_ / (double)format.rate))));
    }

    inline chronos::time_point_clk end() const
    {
        return start() + durationLeft<chronos::usec>();
    }

    template <typename T>
    inline T duration() const
    {
        return std::chrono::duration_cast<T>(chronos::nsec((chronos::nsec::rep)(1000000 * getFrameCount() / format.msRate())));
    }

    template <typename T>
    inline T durationLeft() const
    {
        return std::chrono::duration_cast<T>(chronos::nsec((chronos::nsec::rep)(1000000 * (getFrameCount() - idx_) / format.msRate())));
    }

    inline bool isEndOfChunk() const
    {
        return idx_ >= getFrameCount();
    }

    inline size_t getFrameCount() const
    {
        return (payloadSize / format.frameSize);
    }

    inline size_t getSampleCount() const
    {
        return (payloadSize / format.sampleSize);
    }

    SampleFormat format;

private:
    uint32_t idx_;
};
} // namespace msg

#endif
