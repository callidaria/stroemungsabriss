#pragma once

#include <iostream>
#include <SDL2/SDL_net.h>

class ConnectionUDP
{
public:
	ConnectionUDP(int loc_port,std::string stringIP,int rem_port);
	void send_data(std::string dat);
	UDPpacket* get_data();
private:
	UDPsocket m_socket;
	UDPpacket* m_packet;
};
