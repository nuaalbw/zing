/*************************************************************************
	> File Name: SocketFiles.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sat 20 Nov 2021 10:24:00 PM CST
 ************************************************************************/

#ifndef ZING_NET_SOCKET_FILES_H
#define ZING_NET_SOCKET_FILES_H

#include <sys/types.h>         
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h> 
#include <netinet/ether.h>   
#include <netinet/ip.h>  
#include <netpacket/packet.h>   
#include <arpa/inet.h>
#include <net/ethernet.h>   
#include <net/route.h>  
#include <netdb.h>
#include <net/if.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <cstdint>
#include <cstring>

#endif