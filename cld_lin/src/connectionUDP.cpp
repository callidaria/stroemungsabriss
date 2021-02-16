#include "../net/connectionUDP.h"

ConnectionUDP::ConnectionUDP(int loc_port,std::string stringIP,int rem_port)
{
	if (SDLNet_Init()==-1) printf("ERROR: network initialization failed\n");
	
	m_socket = SDLNet_UDP_Open(loc_port);
	if (m_socket==nullptr) printf("ERROR: port opening failed\n");
	IPaddress IP;
	if (SDLNet_ResolveHost(&IP,stringIP.c_str(),rem_port)==-1) printf("ERROR: ip:port set failure\n");
	
	m_packet = SDLNet_AllocPacket(512);
	if (m_packet==nullptr) printf("ERROR: packet creation failure\n");
	m_packet->address.host = IP.host;
	m_packet->address.port = IP.port;
}
void ConnectionUDP::send_data(std::string dat)
{
	memcpy(m_packet->data,dat.c_str(),dat.length());
	m_packet->len = dat.length();
	SDLNet_UDP_Send(m_socket,-1,m_packet);
}
UDPpacket* ConnectionUDP::get_data()
{
	SDLNet_UDP_Recv(m_socket,m_packet);
	return m_packet;
}
