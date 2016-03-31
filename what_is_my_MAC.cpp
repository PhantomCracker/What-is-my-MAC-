/*

	Developer: PhantomCracker
	OUIs list: IEEE official website
	Vendor list: IEEE official website
	Platform: Linux

	Usage: ./what_is_my_Mac.cpp <interface>

*/


#include <iostream>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

#ifndef ifr_newname
#define ifr_newname     ifr_ifru.ifru_slave
#endif
#define v_length 21861
/*
struct ifreq 
{
    char ifr_name[IFNAMSIZ]; // Interface name 
    union 
    {
        struct sockaddr ifr_addr;
        struct sockaddr ifr_dstaddr;
        struct sockaddr ifr_broadaddr;
        struct sockaddr ifr_netmask;
        struct sockaddr ifr_hwaddr;
        short           ifr_flags;
        int             ifr_ifindex;
        int             ifr_metric;
        int             ifr_mtu;
        struct ifmap    ifr_map;
        char            ifr_slave[IFNAMSIZ];
        char            ifr_newname[IFNAMSIZ];
        char           *ifr_data;
    };
};

struct ifconf 
{
    int ifc_len; // size of buffer 
    union 
    {
        char           *ifc_buf; // buffer address 
        struct ifreq   *ifc_req; // array of structures 
    };
};

Documentation: http://man7.org/linux/man-pages/man7/netdevice.7.html
*/

void DieWithError(char *error_message) // error handling
{
	perror(error_message);
	exit(1);
}

int main(int argc, char **argv)
{
	cout<<"Usage: ./what_is_my_Mac.cpp <interface>"<<endl<<endl;
	struct ifreq ifr;
	int s;
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) // create socket
		DieWithError("Socket error!");
	strcpy(ifr.ifr_name, argv[1]); // interface name
	if(ioctl(s, SIOCGIFHWADDR, &ifr) < 0) 
		DieWithError("ioctl error!");

	uint8_t* MAC;
	MAC = reinterpret_cast<uint8_t*>(ifr.ifr_hwaddr.sa_data);
	printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
	
	// convert uint8_t to string
	ostringstream convert;
	for(int i=0; i<6; i++)
		convert<<std::uppercase<<setfill('0')<<setw(2)<<std::hex<<(int)MAC[i];
	string hex_mac = convert.str();

	// get organizations name
	ifstream organization("organization_name.txt");
	string name[v_length];
	for(int i=0; i<v_length; i++)
		getline(organization, name[i]);

	// get OUI's
	ifstream assig("assignment.txt");
	string oui[v_length];
	for(int i=0; i<v_length; i++)
		getline(assig, oui[i]);

	// get Vendor Address
	string vendor_address;
	vendor_address = hex_mac.substr(0, 6);
	cout<<"Vendor address: "<<vendor_address<<endl;

	// print Provider
	cout<<"Provider: ";
	for(int i=0; i<v_length; i++)
		if(vendor_address.compare(oui[i]) == 0)
		{
			cout<<name[i]<<endl;
			break;
		}

	cout<<endl;
	return 0;
}