/*
 * IperfFlow.cpp
 *
 *  Created on: 13 de set de 2017
 *      Author: anderson
 */

#include "IperfFlow.h"

IperfFlow::IperfFlow()
{
	setTimeScale(seconds);
}

IperfFlow::~IperfFlow()
{

	// nothing to do
}

int IperfFlow::server(const std::string &netInterface)
{
	printf(
			"NOTE: Use Compact Trace Descriptors parameterized in seconds (sufix .sec) in the client side.\n");

	std::thread th_server_1(iperf_server_tcp);
	std::thread th_server_2(iperf_server_udp);

	th_server_1.join();
	th_server_2.join();

	return (0);
}

int IperfFlow::iperf_server_tcp()
{
	// popen args
	FILE *in;
	char buff[512];
	//iperf args

	char command[2048] = "iperf -s";

	if (!(in = popen(command, "r")))
	{
		PLOG_ERROR << ERRORMSG_CANNOT_CREATE_PROCESS
							<< "IperfFlow error: cannot execute command `"
							<< command << "`";
		return -1;
	}

	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		simitar_iostream_mutex.lock();
		std::cout << buff;
		simitar_iostream_mutex.unlock();
	}

	pclose(in);
	return (0);

}

int IperfFlow::iperf_server_udp()
{
	// popen args
	FILE *in;
	char buff[512];
	//iperf args

	char command[2048] = "iperf -s --udp";
	if (!(in = popen(command, "r")))
	{
		PLOG_ERROR << ERRORMSG_CANNOT_CREATE_PROCESS
							<< "IperfFlow error: cannot execute command `"
							<< command << "`";
		return -1;
	}

	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		simitar_iostream_mutex.lock();
		std::cout << buff;
		simitar_iostream_mutex.unlock();
	}

	pclose(in);
	return (0);
}

void IperfFlow::flowGenerate(const counter &flowId, const time_sec &onTime,
		const uint &npackets, const uint &nbytes, const string &netInterface)
{

	// popen args
	FILE *in;
	char buff[512];
	//iperf args
	char command[2048];
	strcpy(command,
			iperf_command(onTime, npackets, nbytes, netInterface).c_str());

	PLOG_INFO << "iperf flow[" << flowId << "]: " << command;

	if (!(in = popen(command, "r")))
	{
		PLOG_ERROR << ERRORMSG_CANNOT_CREATE_PROCESS
							<< "IperfFlow error: cannot execute command `"
							<< command << "`" << std::endl;
		return;
	}

	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		simitar_iostream_mutex.lock();
		std::cout << buff;
		simitar_iostream_mutex.unlock();
	}

	pclose(in);

}

/**
 Usage: iperf [-s|-c host] [options]
 iperf [-h|--help] [-v|--version]

 Client/Server:
 u		-b, --bandwidth #[KMG | pps]  bandwidth to send at in bits/sec or packets per second
 x		-e, --enhancedreports    use enhanced reporting giving more tcp/udp and traffic information
 u 		-f, --format    [kmKM]   format to report: Kbits, Mbits, KBytes, MBytes
 x		-i, --interval  #        seconds between periodic bandwidth reports
 ?		-l, --len       #[KM]    length of buffer to read or write (default 8 KB)
 x		-m, --print_mss          print TCP maximum segment size (MTU - TCP/IP header)
 x		-o, --output    <filename> output the report or error message to this specified file
 ?		-p, --port      #        server port to listen on/connect to
 u		-u, --udp                use UDP rather than TCP
 ?		-w, --window    #[KM]    TCP window size (socket buffer size)
 u		-z, --realtime           request realtime scheduler
 ?		-B, --bind      <host>   bind to <host>, an interface or multicast address
 x		-C, --compatibility      for use with older versions does not sent extra msgs
 ?		-M, --mss       #        set TCP maximum segment size (MTU - 40 bytes)
 u		-N, --nodelay            set TCP no delay, disabling Nagle's Algorithm
 x		-V, --ipv6_domain        Set the domain to IPv6

 Server specific:
 u		-s, --server             run in server mode
 x		-U, --single_udp         run in single threaded UDP mode
 x		-D, --daemon             run the server as a daemon

 Client specific:
 u		-c, --client    <host>   run in client mode, connecting to <host>
 x		-d, --dualtest           Do a bidirectional test simultaneously
 u		-n, --num       #[KM]    number of bytes to transmit (instead of -t)
 x		-r, --tradeoff           Do a bidirectional test individually
 u		-t, --time      #        time in seconds to transmit for (default 10 secs)
 x		-B, --bind [<ip> | <ip:port>] bind src addr(s) from which to originate traffic
 ?		-F, --fileinput <name>   input the data to be transmitted from a file
 x		-I, --stdin              input the data to be transmitted from stdin
 x		-L, --listenport #       port to receive bidirectional tests back on
 ?		-P, --parallel  #        number of parallel client threads to run
 u		-T, --ttl       #        time-to-live, for multicast (default 1)
 -Z, --linux-congestion <algo>  set TCP congestion control algorithm (Linux only)

 Miscellaneous:
 -x, --reportexclude [CDMSV]   exclude C(connection) D(data) M(multicast) S(settings) V(server) reports
 -y, --reportstyle C      report as a Comma-Separated Values
 -h, --help               print this message and quit
 -v, --version            print version information and quit

 [KM] Indicates options that support a K or M suffix for kilo- or mega-

 The TCP window size option can be set by the environment variable
 TCP_WINDOW_SIZE. Most other options can be set by an environment variable
 IPERF_<long option name>, such as IPERF_BANDWIDTH.

 Source at <http://sourceforge.net/projects/iperf2/>
 Report bugs to <iperf-users@lists.sourceforge.net>

 **/
std::string IperfFlow::iperf_command(const time_sec &onTime,
		const uint &npackets, const uint &nbytes, const string &netInterface)
{
	/// iperf running options. Traffic generation custom options
	enum_transmission_type transmission_type = packets;
	std::string congestion_algorithm_name = "reno";
	bool real_time = true;
	bool congestion_algorithm = true;
	bool no_delay = true;

	/// Simitar values. Values captured form the compact trace descriptor
	int bw = this->getNumberOfKbytes() * 8 / onTime;
	int pps = this->getNumberOfPackets() / onTime;

	int nkbytes = this->getNumberOfKbytes();
	int nkbits = this->getNumberOfKbytes() * 8;
	int ttlVal = this->getNetworkTtl();
	bool tcp = (this->getTransportProtocol() == PROTOCOL__TCP) ? true : false;
	bool ipv6 = (this->getNetworkProtocol() == PROTOCOL__IPV6) ? true : false;
	std::string server_ip = this->getNetworkDstAddr();
	std::string client_ip = this->getNetworkSrcAddr();
	uint tranport_port = this->getTransportDstPort();
	std::string format = "k"; // format to report: kbits, mbits, KBytes, MBytes

	/// Iperf config opts. Iperf command options
	std::string opt_bandwidth;
	std::string opt_tranmission;
	if (transmission_type == on_time)
	{
		opt_tranmission = " --time " + std::to_string(onTime);
		opt_bandwidth = " --bandwidth " + std::to_string(bw) + "k";
	}
	else if (transmission_type == bits)
	{
		opt_tranmission = " --time " + std::to_string(onTime);
		opt_bandwidth = " --bandwidth " + std::to_string(bw) + "k";
	}
	else // transmission_type == packets
	{
		opt_tranmission = " --num " + std::to_string(npackets);
		opt_bandwidth = " --bandwidth " + std::to_string(npackets / onTime)
				+ "pps";
	}
	std::string opt_format = " --format k "; // format to report: Kbits, Mbits, KBytes, MBytes
	std::string opt_transport_protocol =
			(tcp == true) ?
					" --linux-congestion " + congestion_algorithm_name + " " :
					" --udp ";
	std::string opt_transport_port = std::string(" --bind ") + client_ip
			+ std::string(":") + std::to_string(tranport_port);
	std::string opt_real_time = (real_time == true) ? " --realtime " : "";
	std::string opt_nodelay = " --nodelay ";

	std::string opt_ipv6 = (ipv6 == true) ? " --ipv6_domain " : "";
	std::string opt_ttl = " --ttl " + std::to_string(ttlVal);
	std::string opt_no_delay = (no_delay == true) ? " --nodelay " : "";

	/// iperf command
	std::string command_client = std::string("iperf -c ") + server_ip
			+ opt_bandwidth + opt_tranmission + opt_format
			+ opt_transport_protocol + opt_transport_port + opt_real_time
			+ opt_nodelay + opt_ipv6 + opt_ttl + opt_no_delay;

	return (command_client);
}

