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

#ifndef CONTROL_SESSION_H
#define CONTROL_SESSION_H

#include "common/snap_queue.h"
#include "message/message.hpp"
#include "server_settings.hpp"
#include <atomic>
#include <boost/asio.hpp>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>

using boost::asio::ip::tcp;


class ControlSession;


/// Interface: callback for a received message.
class ControlMessageReceiver
{
public:
    // TODO: rename, error handling
    virtual std::string onMessageReceived(ControlSession* connection, const std::string& message) = 0;
};


/// Endpoint for a connected control client.
/**
 * Endpoint for a connected control client.
 * Messages are sent to the client with the "send" method.
 * Received messages from the client are passed to the ControlMessageReceiver callback
 */
class ControlSession
{
public:
    /// ctor. Received message from the client are passed to MessageReceiver
    ControlSession(ControlMessageReceiver* receiver) : message_receiver_(receiver)
    {
    }
    virtual ~ControlSession() = default;
    virtual void start() = 0;
    virtual void stop() = 0;

    /// Sends a message to the client (synchronous)
    virtual bool send(const std::string& message) = 0;

    /// Sends a message to the client (asynchronous)
    virtual void sendAsync(const std::string& message) = 0;

protected:
    ControlMessageReceiver* message_receiver_;
};



#endif
