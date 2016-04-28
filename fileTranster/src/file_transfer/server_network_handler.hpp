#ifndef _SERVER_NETWORK_HANDLER_HPP_
#define _SERVER_NETWORK_HANDLER_HPP_

#include "network/i_network.hpp"
#include <map>
#include "messages.hpp"

class CommandAnalyser;

using namespace face2wind;

namespace face2wind {
	class Network;
}

class ServerNetworkHandler : public INetworkHandler
{
public:
	ServerNetworkHandler(CommandAnalyser *analyser);
	~ServerNetworkHandler() {}

	virtual void OnConnect(bool is_success, NetworkID network_id, Port local_port, IPAddr remote_ip_addr, Port remote_port);
	virtual void OnAccept(NetworkID network_id, Port listen_port, IPAddr remote_ip_addr, Port remote_port);
	virtual void OnRecv(NetworkID network_id, const char *data, int length);
	virtual void OnDisconnect(NetworkID network_id);

	void Running();

protected:
	void OnRequestFileListReq(NetworkID network_id, const char *data, int length);
	void OnRequestGetFileInfoReq(NetworkID network_id, const char *data, int length);
	void OnRequestGetFileReq(NetworkID network_id, const char *data, int length);

	typedef void (ServerNetworkHandler::*HandleFunc)(NetworkID network_id, const char *data, int length);

private:
	CommandAnalyser *m_analyser;
	face2wind::Network *m_networkMgr;
	std::map<HeadType, HandleFunc> m_handler_map;
};

#endif
