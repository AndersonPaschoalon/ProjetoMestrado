/*
 * DummyFlow.h
 *
 *  Created on: 14 de jun de 2016
 *      Author: apaschoalon
 */

#ifndef DUMMYFLOW_H_
#define DUMMYFLOW_H_

#include "NetworkFlow.h"
/*
 TODO Implementation of L2 data structures
 typedef struct mac_dst_n {
 string mac_dst;
 unsigned float frequency; //0-1
 struct mac_dst_n* next;
 }mac_dst_node;

 typedef struct l2_addr_list{
 string mac_src;
 unsigned long int mac_dst_count;
 mac_dst_node nodes;
 struct l2_addr_list* next;
 } l2_addr;
 */

class DummyFlow: public NetworkFlow
{
public:
	/**
	 *
	 */
	DummyFlow();

	/**
	 *
	 */
	virtual ~DummyFlow();

	/**
	 *
	 */
	void print();

	/**
	 *
	 * @return
	 */
	void flowGenerate();

	std::thread flowThread()
	{

#ifdef DEBUG_flowThread
		cout << "Debug: std::thread flowThread() ok!" << endl;
#endif

		return std::thread([=]
		{	flowGenerate();});
	}

	/**
	 * Return the next IP address form the destination-hosts file
	 * @return
	 */
	string getHostIP();

	/**
	 * returns a random transport port number
	 * @return
	 */
	int randTranportPort();
	protocol getApplicationProtocol() const;
	void setApplicationProtocol(protocol applicationProtocol);
	unsigned int getFlowDsByte() const;
	void setFlowDsByte(unsigned int flowDsByte);
	double getFlowDuration() const;
	void setFlowDuration(time_sec flowDuration);
	double getFlowStartDelay() const;
	void setFlowStartDelay(time_sec flowStartDelay);

	protocol getLinkProtocol() const;
	void setLinkProtocol(protocol linkProtocol);
	long int getLinkSrcAddrCount() const;
	void setLinkSrcAddrCount(long int linkSrcAddrCount);
	const string& getNetworkDstAddr() const;
	void setNetworkDstAddr(const string& networkDstAddr);
	int getNetworkHostListConter() const;
	void setNetworkHostListConter(int networkHostListConter);
	protocol getNetworkProtocol() const;
	void setNetworkProtocol(protocol networkProtocol);
	const string& getNetworkSrcAddr() const;
	void setNetworkSrcAddr(const string& networkSrcAddr);
	unsigned int getNetworkTtl() const;
	void setNetworkTtl(unsigned int networkTtl);
	unsigned long int getNumberOfKbytes() const;
	void setNumberOfKbytes(unsigned long int numberOfKbytes);
	unsigned long int getNumberOfPackets() const;
	void setNumberOfPackets(unsigned long int numberOfPackets);

//	const protocolSupport& getProtocols() const;
//	void setProtocols(const protocolSupport& protocols);

	unsigned int getTransportDstPort() const;
	void setTransportDstPort(unsigned int transportDstPort);
	unsigned int getTransportSctpAssociationId() const;
	void setTransportSctpAssociationId(unsigned int transportSctpAssociationId);
	unsigned int getTransportSctpMaxStreams() const;
	void setTransportSctpMaxStreams(unsigned int transportSctpMaxStreams);
	unsigned int getTransportSrcPort() const;
	void setTransportSrcPort(unsigned int transportSrcPort);
	protocol getTransportProtocol() const;
	void setTransportProtocol(protocol transportProtocol);


	void setInterDepertureTimeModels(list<StochasticModelFit>* modelList);
//	void setInterDepertureTimeModels(StochasticModelFit* modelVet);
	StochasticModelFit getInterDepertureTimeModel_next();
	unsigned int getNumberOfInterdepertureTimeModels();


	void setInterFileTimeModel(list<StochasticModelFit>* modelList);
//	void setInterFileTimeModel(StochasticModelFit* modelVet);
	StochasticModelFit getInterFileTimeModel_next();
	time_sec getInterFileTime();
	unsigned int getNumberOfInterfileTimeModels();

//	void setInterSessionTimeModel(StochasticModelFit* modelVet);
//	void setInterSessionTimeModel(list<StochasticModelFit>* modelList);
	void setInterSessionTimesOnOff(vector<time_sec>* onOffList);
//	StochasticModelFit getInterSessionOnOffTime_next();
	time_sec getInterSessionOnOffTime_next();
	time_sec getInterSessionTime();
	unsigned int getNumberOfSessionOnOffTimes();

	StochasticModelFit getPacketSizeModelMode1_next();
	StochasticModelFit getPacketSizeModelMode2_next();

//	void setPacketSizeModel(StochasticModelFit* modelVet1,
//			StochasticModelFit* modelVet, long int nkbytesMode1,
//			long int nkbytesMode2, long int nPacketsMode1,
//			long int nPacketsMode2);
	void setPacketSizeModel(list<StochasticModelFit>* modelList1,
			list<StochasticModelFit>* modelList2, long int nkbytesMode1,
			long int nkbytesMode2, long int nPacketsMode1,
			long int nPacketsMode2);
	long int getNkbytesMode1() const;
	long int getNkbytesMode2() const;
	long int getNpacketsMode1() const;
	long int getNpacketsMode2() const;

	unsigned int  getNumberOfPsMode2Models() const;
	unsigned int  getNumberOfPsMode1Models() const;

	//DEBUG
	void printModels();


protected:
	//protocolSupport protocols;

private:
	/**
	 * Flow-level options
	 */
	time_sec flow_duration;
	time_sec flow_start_delay;
	unsigned int flow_ds_byte;
	unsigned long int number_of_packets;
	unsigned long int number_of_kbytes;

	/**
	 * Protocol stack options
	 */
	//TODO L2: Link Layer Implementation of L2 structures
	protocol link_protocol;
	string link_src;
	string link_dst;
	//long int link_src_addr_count;
	//l2_addr linkedlist;

	//L3: Network Layer
	unsigned int network_ttl;
	protocol network_protocol;
	string network_dst_addr;
	string network_src_addr;
	int network_hostList_conter;

	//L4: Transport Layer
	unsigned int transport_dst_port;
	unsigned int transport_src_port;
	protocol transport_protocol;
	unsigned int transport_sctp_association_id;
	unsigned int transport_sctp_max_streams;

	//L5: Application Layer
	protocol application_protocol;
	// TODO Application Layer implementation

	/**
	 * Interperture time
	 */
	list<StochasticModelFit>* ptr_interArrivalModelList; //file interdeperture time
	counter interDepertureTimeModel_counter = 0; //counter of for model get method
	list<StochasticModelFit>* ptr_interFileModelList; //inter-file time
	counter interFileModel_counter = 0;
	vector<time_sec>* ptr_interSessionOnOffTimes; //inter-session time
	counter interSessionModel_counter = 0;
	/*
	 StochasticModelFit* interArrivalvet; //file interdeperture time
	 counter interDepertureTimeModel_counter = 0; //counter of for model get method
	 StochasticModelFit* interFileModel; //inter-file time
	 counter interFileModel_counter = 0;
	 StochasticModelFit* interSessionModel; //inter-session time
	 counter interSessionModel_counter = 0;
	 */

	/**
	 * Packet Size Model
	 */
	list<StochasticModelFit>* ptr_psMode1;
	list<StochasticModelFit>* ptr_psMode2;
	long int nkbytes_mode1;
	long int nkbytes_mode2;
	long int npacket_mode1;
	long int npackets_mode2;
	int packetSizeModel1_counter;
	int packetSizeModel2_counter;

};



#endif /* DUMMYFLOW_H_ */