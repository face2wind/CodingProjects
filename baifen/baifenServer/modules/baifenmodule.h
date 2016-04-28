/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  dream <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BAIFEN_MODULE_H
#define BAIFEN_MODULE_H

#include <socketMessage.h>
#include <map>
#include <networkManager.h>
#include <roomManager.h>

class BaifenModule : public face2wind::INetworkHandler
{
  std::map<unsigned int,unsigned int> _loginPlayerID2Socket;
  std::map<unsigned int,unsigned int> _socket2LoginPlayerID;

  int SendRoomMsgTo(const unsigned int &socketFD,
		    face2wind::RoomMsg *roomMsg );

 public:
  BaifenModule();
  ~BaifenModule();

  int OnReceiveMessage(unsigned int cmd,
		       face2wind::SocketMessage *msg,
		       const unsigned int &socketFD);
  int OnConnected(const std::string &ip,
		  const unsigned int &port,
		  const unsigned int &socketFD);
  int OnAccepted(const unsigned int &socketFD);
  int OnDisconnected(const unsigned int &socketFD);

};

#endif // BAIFEN_MODULE_H
