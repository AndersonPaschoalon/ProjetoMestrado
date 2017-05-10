/*
 * DummyFlow.cpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: apaschoalon
 */

#include "Defines.h"
#include "DummyFlow.h"

DummyFlow::DummyFlow()
{

	///flow-level parameters initialization

	flow_ds_byte = 0;
	flow_duration = 0;
	flow_start_delay = 0;
	number_of_kbytes = 0;
	number_of_packets = 0;

	///protocols parameters  initialization

	link_protocol = PROTOCOL__ETHERNET; //default
	mac_src = "";
	mac_dst = "";
	network_dst_addr = "";
	network_src_addr = "";
	network_protocol = PROTOCOL__IPV4; //default
	network_ttl = 64; //default
	network_hostList_conter = 0;
	transport_protocol = PROTOCOL__UDP; //default
	transport_dst_port = 0;
	transport_sctp_association_id = 0;
	transport_sctp_max_streams = 0;
	transport_src_port = 0;
	application_protocol = PROTOCOL__NULL;

	//
	//Interarrival
	//
	ptr_interArrivalModelList = NULL;
	interDepertureTimeModel_counter = 0;
	ptr_interFileModelList = NULL;
	interFileModel_counter = 0;
	ptr_session_onTimes = NULL;
	ptr_session_offTimes = NULL;
	sessionOnTimes_counter = 0;
	sessionOffTimes_counter = 0;

	//
	//Packet size parameters
	//
	ptr_psMode1 = NULL;
	ptr_psMode2 = NULL;
	nkbytes_mode1 = 0;
	nkbytes_mode2 = 0;
	npacket_mode1 = 0;
	npackets_mode2 = 0;
	packetSizeModel1_counter = 0;
	packetSizeModel2_counter = 0;

}

DummyFlow::~DummyFlow()
{
	MESSER_LOG_INIT(DEBUG);
	//MESSER_DEBUG("@ <%s, %s>");

	ptr_interArrivalModelList->clear();
	//	ptr_interFileModelList->clear();
	//	ptr_interSessionOnOffTimes->clear();
	ptr_session_onTimes->clear();
	ptr_session_offTimes->clear();

	ptr_psMode1->clear();
	ptr_psMode2->clear();

	//MESSER_DEBUG("ptr_interArrivalModelList->size(): %d <%s, %s>",
	//		ptr_interArrivalModelList->size());
	//MESSER_DEBUG("ptr_psMode1->size():%d  <%s, %s>", ptr_psMode1->size());
	//MESSER_DEBUG("ptr_psMode2->size(): %d <%s, %s>", ptr_psMode2->size());

	//interarrival data structs
	delete ptr_interArrivalModelList;
	delete ptr_interFileModelList;
	delete ptr_session_onTimes;
	delete ptr_session_offTimes;

	//packet-size data structures
	delete ptr_psMode1;
	delete ptr_psMode2;
}

void DummyFlow::print()
{
	//TODO
	cout << "flow created" << endl;
}

void DummyFlow::flowGenerate()
{
	int rc = 0;

	unsigned int usecs = (unsigned int) (this->getFlowStartDelay() * MEGA_POWER);
	string flow_str_print = "";
	protocol prt;
	StochasticModelFit themodel;

	/***************************************************************************
	 * Dummy-parser
	 **************************************************************************/

	/**
	 * Flow-level
	 */
	flow_str_print += "Flow> Duration:" + std::to_string(flow_duration)
			+ ", Start-delay:" + std::to_string(flow_start_delay) + "s"
			+ ", N.packets: " + std::to_string(number_of_packets);

	////////////////////////////////////////////////////////////////////////////
	/// Link-layer protocol
	////////////////////////////////////////////////////////////////////////////
	flow_str_print += " Link[";
	prt = this->getLinkProtocol();
	if (prt == PROTOCOL__ETHERNET)
	{
		flow_str_print += "Ethernet";
	}
	else
	{
		flow_str_print += "Ethernet";
	}
	flow_str_print += "]";

	////////////////////////////////////////////////////////////////////////////
	/// Network-layer protocol
	////////////////////////////////////////////////////////////////////////////

	flow_str_print += " Network[";
	prt = this->getNetworkProtocol();
	if (prt == PROTOCOL__IPV4)
	{
		flow_str_print += "IPv4: ";
	}
	else if (prt == PROTOCOL__IPV6)
	{
		flow_str_print += "IPv6: ";
	}
	else
	{
		flow_str_print += std::to_string(prt) + ": ";
	}
	flow_str_print += this->getNetworkSrcAddr() + " > "
			+ this->getNetworkDstAddr() + "]";

	////////////////////////////////////////////////////////////////////////////
	/// Transport-layer protocol
	////////////////////////////////////////////////////////////////////////////

	flow_str_print += " Transport[";
	prt = this->getTransportProtocol();
	if (prt == PROTOCOL__TCP)
	{
		flow_str_print += "TCP";
	}
	else if (prt == PROTOCOL__UDP)
	{
		flow_str_print += "UDP";
	}
	else if (prt == PROTOCOL__ICMP)
	{
		flow_str_print += "ICMP";
	}
	else if (prt == PROTOCOL__ICMPV6)
	{
		flow_str_print += "ICMPv6";
	}

	else if (prt == PROTOCOL__SCTP)
	{
		flow_str_print += "SCTP";
	}

	else if (prt == PROTOCOL__DCCP)
	{
		flow_str_print += "";
	}

	else if (prt == PROTOCOL__GRE)
	{
		flow_str_print += "GRE";
	}
	else
	{
		flow_str_print += std::to_string(prt);
	}
	flow_str_print += ": " + std::to_string(this->getTransportSrcPort()) + " > "
			+ std::to_string(this->getTransportDstPort()) + "]";

	// Application protocol
	flow_str_print += " Application[no-protocol] ";

	////////////////////////////////////////////////////////////////////////////
	/// Inter-deperture model
	////////////////////////////////////////////////////////////////////////////

	flow_str_print += " Inter-deperture[";
	themodel = this->getInterDepertureTimeModel_next();
	flow_str_print += themodel.strModelName();

	if (themodel.modelName() == WEIBULL)
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", betha=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == NORMAL)
	{
		flow_str_print += ": mu=" + std::to_string(themodel.param1())
				+ ", sigma=" + std::to_string(themodel.param2());
	}
	else if ((themodel.modelName() == EXPONENTIAL_LINEAR_REGRESSION)
			|| (themodel.modelName() == EXPONENTIAL_MEAN))
	{
		flow_str_print += ": lambda=" + std::to_string(themodel.param1());
	}
	else if ((themodel.modelName() == PARETO_LINEAR_REGRESSION)
			|| (themodel.modelName() == PARETO_MAXIMUM_LIKEHOOD))
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", xm=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == CAUCHY)
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", x0=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == CONSTANT)
	{
		flow_str_print += ": mean=" + std::to_string(themodel.param1());
	}
	else if ((themodel.modelName() == SINGLE_PACKET)
			|| (themodel.modelName() == NO_MODEL))
	{
		//do nothing
	}
	else
	{

		cerr << "Error @" << __PRETTY_FUNCTION__
				<< "No model selected for Inter-deperture model\n" << "Hint: "
				<< " this->getInterDepertureTimeModel_next() = "
				<< themodel.strModelName() << endl;
	}

	flow_str_print += "] ";

	flow_str_print += " ("
			+ std::to_string(getNumberOfInterdepertureTimeModels()) + ")";

	////////////////////////////////////////////////////////////////////////////
	/// Packet-size model
	////////////////////////////////////////////////////////////////////////////

	flow_str_print += " Packet-size-Mode1[";

	themodel = this->getPacketSizeModelMode1_next();
	flow_str_print += themodel.strModelName();

	if (themodel.modelName() == WEIBULL)
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", betha=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == NORMAL)
	{
		flow_str_print += ": mu=" + std::to_string(themodel.param1())
				+ ", sigma=" + std::to_string(themodel.param2());
	}
	else if ((themodel.modelName() == EXPONENTIAL_LINEAR_REGRESSION)
			|| (themodel.modelName() == EXPONENTIAL_MEAN))
	{
		flow_str_print += ": lambda=" + std::to_string(themodel.param1());
	}
	else if ((themodel.modelName() == PARETO_LINEAR_REGRESSION)
			|| (themodel.modelName() == PARETO_MAXIMUM_LIKEHOOD))
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", xm=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == CAUCHY)
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", x0=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == CONSTANT)
	{
		flow_str_print += ": mean=" + std::to_string(themodel.param1());
	}
	else if (themodel.modelName() == NO_MODEL)
	{
		//flow_str_print += ": mean=" + std::to_string(themodel.param1);
	}
	else
	{
		cerr << "Error @ " << __PRETTY_FUNCTION__
				<< "No model selected for Packet-size Mode1 model\n" << endl;
		cerr << "Hint: " << " getPacketSizeModelMode1_next = "
				<< themodel.strModelName() << endl;
		errno = EINVAL;
	}
	flow_str_print += "] ";

	flow_str_print += " Packet-size-Mode2[";

	themodel = this->getPacketSizeModelMode2_next();
	flow_str_print += themodel.strModelName();

	if (themodel.modelName() == WEIBULL)
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", betha=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == NORMAL)
	{
		flow_str_print += ": mu=" + std::to_string(themodel.param1())
				+ ", sigma=" + std::to_string(themodel.param2());
	}
	else if ((themodel.modelName() == EXPONENTIAL_LINEAR_REGRESSION)
			|| (themodel.modelName() == EXPONENTIAL_MEAN))
	{
		flow_str_print += ": lambda=" + std::to_string(themodel.param1());
	}
	else if ((themodel.modelName() == PARETO_LINEAR_REGRESSION)
			|| (themodel.modelName() == PARETO_MAXIMUM_LIKEHOOD))
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", xm=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == CAUCHY)
	{
		flow_str_print += ": alpha=" + std::to_string(themodel.param1())
				+ ", x0=" + std::to_string(themodel.param2());
	}
	else if (themodel.modelName() == CONSTANT)
	{
		flow_str_print += ": mean=" + std::to_string(themodel.param1());
	}
	else if (themodel.modelName() == NO_MODEL)
	{
		//flow_str_print += ": mean=" + std::to_string(themodel.param1);
	}
	else
	{
		perror(
				"Error @ DummyFlow::flowGenerate(). No model selected for Packet-size Mode2 model\n");
		cerr << "Hint: " << " getPacketSizeModelMode2_next = "
				<< themodel.strModelName() << endl;
		errno = EINVAL;
	}
	flow_str_print += "] ";

	//convert the cpp flow-string to a c flow string, in order to atomize the printing
	const char* flow_print = flow_str_print.c_str();

	//rc = usleep(usecs);
	if (rc != 0)
	{
		perror(
				"Impossible to execute usleep() sleep at void DummyFlow::printFlow()\n");
		errno = EAGAIN;
		exit(EXIT_FAILURE);
	}

	pthread_mutex_t lock;
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_lock(&lock);
	printf("%s\n", flow_print);
	pthread_mutex_unlock(&lock);
	pthread_mutex_destroy(&lock);

}

int DummyFlow::randTranportPort()
{
	return (rand() % (MAX_TRANSPORT_PORT_NUMBER + 1));
}

//TODO implementar a lista de hosts -> catch these values from a list
string DummyFlow::getHostIP()
{
	string dstIP;
	pthread_mutex_t lock;

	pthread_mutex_init(&lock, NULL);
	pthread_mutex_lock(&lock);
	if (network_hostList_conter == 0)
	{
		dstIP = "172.16.0.1";
		network_hostList_conter++;
	}
	else
	{
		dstIP = "172.16.0.2";
		network_hostList_conter = 0;
	}
	pthread_mutex_unlock(&lock);
	pthread_mutex_destroy(&lock);

	return (dstIP);
}

unsigned int DummyFlow::getFlowDsByte() const
{
	return flow_ds_byte;
}

void DummyFlow::setFlowDsByte(unsigned int flowDsByte)
{
//allowed range [0, 255]
//Under Linux you need root privileges to set the DS byte to a value
//larger than 160
	if (flowDsByte < 255)
		flow_ds_byte = flowDsByte;
	else
		flow_ds_byte = 255;
}

protocol DummyFlow::getApplicationProtocol() const
{
	return application_protocol;
}

void DummyFlow::setApplicationProtocol(protocol applicationProtocol)
{
	application_protocol = applicationProtocol;
}

double DummyFlow::getFlowDuration() const
{
	return flow_duration;
}

void DummyFlow::setFlowDuration(double flowDuration)
{
	flow_duration = flowDuration;
}

double DummyFlow::getFlowStartDelay() const
{
	return flow_start_delay;
}

void DummyFlow::setFlowStartDelay(double flowStartDelay)
{
	flow_start_delay = flowStartDelay;
}

protocol DummyFlow::getLinkProtocol() const
{
	return link_protocol;
}

void DummyFlow::setLinkProtocol(protocol linkProtocol)
{
	link_protocol = linkProtocol;
}

//long int DummyFlow::getLinkSrcAddrCount() const
//{
//	return link_src_addr_count;
//}

//void DummyFlow::setLinkSrcAddrCount(long int linkSrcAddrCount)
//{
//	link_src_addr_count = linkSrcAddrCount;
//}

const string& DummyFlow::getNetworkDstAddr() const
{
	return network_dst_addr;
}

void DummyFlow::setNetworkDstAddr(const string& networkDstAddr)
{
	network_dst_addr = networkDstAddr;
}

int DummyFlow::getNetworkHostListConter() const
{
	return network_hostList_conter;
}

void DummyFlow::setNetworkHostListConter(int networkHostListConter)
{
	network_hostList_conter = networkHostListConter;
}

protocol DummyFlow::getNetworkProtocol() const
{
	return network_protocol;
}

void DummyFlow::setNetworkProtocol(protocol networkProtocol)
{
	network_protocol = networkProtocol;
}

const string& DummyFlow::getNetworkSrcAddr() const
{
	return network_src_addr;
}

void DummyFlow::setNetworkSrcAddr(const string& networkSrcAddr)
{
	network_src_addr = networkSrcAddr;
}

unsigned int DummyFlow::getNetworkTtl() const
{
	return network_ttl;
}

void DummyFlow::setNetworkTtl(unsigned int networkTtl)
{
	network_ttl = networkTtl;
}

unsigned long int DummyFlow::getNumberOfKbytes() const
{
	return number_of_kbytes;
}

void DummyFlow::setNumberOfKbytes(unsigned long int numberOfKbytes)
{
	number_of_kbytes = numberOfKbytes;
}

unsigned long int DummyFlow::getNumberOfPackets() const
{
	return number_of_packets;
}

void DummyFlow::setNumberOfPackets(unsigned long int numberOfPackets)
{
	number_of_packets = numberOfPackets;
}

unsigned int DummyFlow::getTransportDstPort() const
{
	return transport_dst_port;
}

void DummyFlow::setTransportDstPort(unsigned int transportDstPort)
{
	transport_dst_port = transportDstPort;
}

unsigned int DummyFlow::getTransportSctpAssociationId() const
{
	return transport_sctp_association_id;
}

void DummyFlow::setTransportSctpAssociationId(
		unsigned int transportSctpAssociationId)
{
	transport_sctp_association_id = transportSctpAssociationId;
}

unsigned int DummyFlow::getTransportSctpMaxStreams() const
{
	return transport_sctp_max_streams;
}

void DummyFlow::setTransportSctpMaxStreams(unsigned int transportSctpMaxStreams)
{
	transport_sctp_max_streams = transportSctpMaxStreams;
}

unsigned int DummyFlow::getTransportSrcPort() const
{
	return transport_src_port;
}

void DummyFlow::setTransportSrcPort(unsigned int transportSrcPort)
{
	transport_src_port = transportSrcPort;
}

protocol DummyFlow::getTransportProtocol() const
{
	return transport_protocol;
}

void DummyFlow::setTransportProtocol(protocol transportProtocol)
{
	transport_protocol = transportProtocol;
}

StochasticModelFit DummyFlow::getInterDepertureTimeModel_next()
{

	StochasticModelFit themodel;
	unsigned int i = 0;

	if (ptr_interArrivalModelList == NULL)
	{
		themodel.set(NO_MODEL, 0, 0, datum::inf, datum::inf);
		ptr_interArrivalModelList->push_back(themodel);

		cerr << "Error @ " << __PRETTY_FUNCTION__ << endl
				<< "Trying to get a StochasticModelFit, but no model was set.\n"
				<< "NO_MODEL set as default" << endl;
	}
	else
	{

		for (list<StochasticModelFit>::iterator it =
				ptr_interArrivalModelList->begin();
				it != ptr_interArrivalModelList->end(); it++)
		{
			if (i >= interDepertureTimeModel_counter)
			{

				themodel = *it;
				break;
			}
			else
			{
				i++;
			}

		}

	}

	if ((ptr_interArrivalModelList->size() - 1)
			> interDepertureTimeModel_counter)
	{
		interDepertureTimeModel_counter++;
	}

	return (themodel);

}

void DummyFlow::setInterDepertureTimeModels(list<StochasticModelFit>* modelList)
{

	ptr_interArrivalModelList = modelList;
	interDepertureTimeModel_counter = 0;
}

void DummyFlow::setInterFileTimeModel(list<StochasticModelFit>* modelList)
{
	ptr_interFileModelList = modelList;
	interFileModel_counter = 0;
}

//TODO implement on/off
StochasticModelFit DummyFlow::getInterFileTimeModel_next()
{
	StochasticModelFit themodel;
	counter i = 0;

	if (ptr_interFileModelList == NULL)
	{
		themodel.set(NO_MODEL, 0, 0, datum::inf, datum::inf);
		ptr_interFileModelList->push_back(themodel);

		cerr << "Error @ " << __PRETTY_FUNCTION__ << endl
				<< "Trying to get a StochasticModelFit, but no model was set.\n"
				<< "NO_MODEL set as default" << endl;
	}
	else
	{

		for (list<StochasticModelFit>::iterator it =
				ptr_interFileModelList->begin();
				it != ptr_interFileModelList->end(); it++)
		{
			if (i >= interFileModel_counter)
			{
				themodel = *it;
				break;
			}
			else
			{
				i++;
			}

		}

	}

	if ((ptr_interFileModelList->size() - 1) > interFileModel_counter)
	{
		interFileModel_counter++;
	}

	return (themodel);

}

time_sec DummyFlow::getInterFileTime()
{
	time_sec interFileTimeRngEstimation = 0;

//TODO: weibull-constant estimator rng generator

	return (interFileTimeRngEstimation);
}

//void DummyFlow::setInterSessionTimesOnOff(vector<time_sec>* onTimesVec,
//		vector<time_sec>* offTimesVec)
//{
//	ptr_session_onTimes = onTimesVec;
//	ptr_session_offTimes = offTimesVec;
//}
void DummyFlow::setSessionTimesOnOff(vector<time_sec>* onTimesVec,
		vector<time_sec>* offTimesVec)
{
	ptr_session_onTimes = onTimesVec;
	ptr_session_offTimes = offTimesVec;
}

time_sec DummyFlow::getSessionOnTime_next()
{
	MESSER_LOG_INIT(DEBUG);
	time_sec theTime = 0;

	//MESSER_DEBUG("ptr_session_onTimes->size()=%d", ptr_session_onTimes->size());

	if (sessionOnTimes_counter >= ptr_session_onTimes->size())
	{
		MESSER_NOTICE(
				"No more On times available on the stack. The last was the %dth value. It will be reseted  @ <%s, %s>",
				sessionOnTimes_counter);
		sessionOnTimes_counter = 0;
		theTime = ptr_session_onTimes->at(sessionOnTimes_counter);

	}
	else
	{
		theTime = ptr_session_onTimes->at(sessionOnTimes_counter);
		sessionOnTimes_counter++;
	}

	return (theTime);
}

time_sec DummyFlow::getSessionOffTime_next()
{
	MESSER_LOG_INIT(DEBUG);
	time_sec theTime = 0;

	if (sessionOffTimes_counter >= ptr_session_offTimes->size())
	{
		MESSER_NOTICE(
				"No more Off times available on the stack. The last was the \
%dth value. Now, it will return 0, and than will be reseted. This is because, \
the On/Off times should aways start with a On. After that, it will be reseted  @ <%s, %s>",
				sessionOffTimes_counter);

		//theTime = ptr_session_offTimes->at(sessionOffTimes_counter);
		theTime = 0;
		sessionOffTimes_counter = 0;

	}
	else
	{
		theTime = ptr_session_offTimes->at(sessionOffTimes_counter);
		sessionOffTimes_counter++;
	}

	return (theTime);
}

//StochasticModelFit DummyFlow::getInterSessionTimeModel_next()
/*
 time_sec DummyFlow::getInterSessionOnOffTime_next()
 {
 MESSER_LOG_INIT(DEBUG);

 if (ptr_session_onTimes->size() != (ptr_session_offTimes->size() + 1))
 {
 MESSER_CRIT(
 "The on/off vectors do not have the expected size. onTimes->size() = %d, offTimes->size() = %d @ <%s, %s>",
 ptr_session_onTimes->size(), ptr_session_offTimes->size());
 exit(-1);
 }

 time_sec theTime = 0;

 if(ptr_session_onTimes->size() == 0)
 {
 MESSER_ERROR("No inter-session times setted @ <%s, %s>");
 return(0);
 }
 else{
 if(ptr_session_onTimes->size() <= interSessionOnTimes_counter){
 MESSER_NOTICE("No more On times available on the stack.  @ <%s, %s>");
 }
 }

 //	if (interSessionModel_counter > ptr_interSessionOnOffTimes->size())
 //	{
 //		return (0);
 //	}
 //	else
 //	{
 //		theTime = ptr_interSessionOnOffTimes->at(interSessionModel_counter);
 //		interSessionModel_counter++;
 //	}

 return (theTime);
 }
 */

StochasticModelFit DummyFlow::getPacketSizeModelMode1_next()
{
	StochasticModelFit themodel;
	if (ptr_psMode1 == NULL)
	{
		themodel.set(NO_MODEL, 0, 0, datum::inf, datum::inf);
		ptr_psMode1->push_back(themodel);
	}
	else
	{
		counter i = 0;
		for (list<StochasticModelFit>::iterator it = ptr_psMode1->begin();
				it != ptr_psMode1->end(); it++)
		{
			if (i == packetSizeModel1_counter)
			{
				themodel = *it;
				break;
			}
			i++;

		}
	}

	if (ptr_psMode1->size() > packetSizeModel1_counter)
	{
		packetSizeModel1_counter++;
	}

	return (themodel);
}

StochasticModelFit DummyFlow::getPacketSizeModelMode2_next()
{
	StochasticModelFit themodel;
	if (ptr_psMode2 == NULL)
	{
		themodel.set(NO_MODEL, 0, 0, datum::inf, datum::inf);
		ptr_psMode2->push_back(themodel);
	}
	else
	{
		counter i = 0;
		for (list<StochasticModelFit>::iterator it = ptr_psMode2->begin();
				it != ptr_psMode2->end(); it++)
		{
			if (i == packetSizeModel2_counter)
			{
				themodel = *it;
				break;
			}
			i++;

		}
	}

	if (ptr_psMode2->size() > packetSizeModel2_counter)
	{
		packetSizeModel2_counter++;
	}

	return (themodel);

}

void DummyFlow::setPacketSizeModel(list<StochasticModelFit>* modelVet1,
		list<StochasticModelFit>* modelVet2, long int nkbytesMode1,
		long int nkbytesMode2, long int nPacketsMode1, long int nPacketsMode2)
{
	ptr_psMode1 = modelVet1;
	ptr_psMode2 = modelVet2;
	packetSizeModel1_counter = 0;
	packetSizeModel2_counter = 0;
	nkbytes_mode1 = nkbytesMode1;
	nkbytes_mode2 = nkbytesMode2;
	npacket_mode1 = nPacketsMode1;
	npackets_mode2 = nPacketsMode2;

}

long int DummyFlow::getNkbytesMode1() const
{
	return nkbytes_mode1;
}

long int DummyFlow::getNkbytesMode2() const
{
	return nkbytes_mode2;
}

long int DummyFlow::getNpacketsMode1() const
{
	return npacket_mode1;
}

unsigned int DummyFlow::getNumberOfInterdepertureTimeModels()
{
	if (ptr_interArrivalModelList != NULL)
	{
		return (ptr_interArrivalModelList->size());
	}
	else
	{
		return (0);
	}
}

unsigned int DummyFlow::getNumberOfInterfileTimeModels()
{
	if (ptr_interFileModelList != NULL)
	{
		return (ptr_interFileModelList->size());
	}
	else
	{
		return (0);
	}
}

//TODO test -> not tested
unsigned int DummyFlow::getNumberOfPsMode1Models() const
{
	if (ptr_psMode1 != NULL)
	{
		return (ptr_psMode1->size());
	}
	else
	{
		return (0);
	}
}

//TODO test -> not tested
unsigned int DummyFlow::getNumberOfPsMode2Models() const
{
	if (ptr_psMode2 != NULL)
	{
		return (ptr_psMode2->size());
	}
	else
	{
		return (0);
	}
}

//unsigned int DummyFlow::getNumberOfSessionOnOffTimes()
//{
//	if (ptr_interSessionOnOffTimes != NULL)
//	{
//		return (ptr_interSessionOnOffTimes->size());
//	}
//	else
//	{
//		return (0);
//	}
//	return (0);
//}

long int DummyFlow::getNpacketsMode2() const
{
	return npackets_mode2;
}
void DummyFlow::setMacAddr(const string& macSrc, const string& macDst)
{
	mac_src = macSrc;
	mac_dst = macDst;
}

const string& DummyFlow::getMacSrcAddr()
{
	return (mac_src);
}

const string& DummyFlow::getMacDstAddr()
{
	return (mac_dst);
}

//DEBUG
void DummyFlow::printModels()
{

	for (list<StochasticModelFit>::iterator it =
			ptr_interArrivalModelList->begin();
			it != ptr_interArrivalModelList->end(); it++)
	{
		it->print();
	}

}

vector<time_sec>* DummyFlow::getSessionOnVector()
{
	return (ptr_session_onTimes);
}

vector<time_sec>* DummyFlow::getSessionOffVector()
{
	return (ptr_session_offTimes);
}

//DEBUG ERASE IT
void DummyFlow::logOnOff()
{
	MESSER_LOG_INIT(DEBUG);

	char ontimes[CHAR_LARGE_BUFFER];
	char offtimes[CHAR_LARGE_BUFFER];
	vector2str(*ptr_session_onTimes, ontimes);
	vector2str(*ptr_session_offTimes, offtimes);
	MESSER_DEBUG("trace>> ontimes=[%s], offtimes=[%s]", ontimes, offtimes);
}
