/*
 * DataProcessor.cpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: apaschoalon
 */

#include "DataProcessor.h"

DataProcessor::DataProcessor()
{
	//m_time_scale = seconds;
	m_time_scale = 1.0;
	m_min_on_time = 0.1;
	m_session_cut_time = 30.0;
	informationCriterionParam = "aic";
	minimumAmountOfPackets = 30;
}

DataProcessor::DataProcessor(time_scale timeScale, time_sec minOnTime,
		time_sec sessionCutTime, uint minNumberOfPackets,
		const std::string& criterion)
{
	if (timeScale == milliseconds)
		m_time_scale = 1000.0;
	else
		m_time_scale = 1.0;

	if (criterion == "aic")
		informationCriterionParam = "aic";
	else if (criterion == "bic")
		informationCriterionParam = "bic";
	else
	{
		cerr << "\nInvalid criterion or no criterion selected: " << criterion
				<< endl << "Selecting default criterion: AIC\n";
		informationCriterionParam = "aic";
	}
	minimumAmountOfPackets = minNumberOfPackets;
	m_min_on_time = m_time_scale * minOnTime;
	m_session_cut_time = m_time_scale * sessionCutTime;
	min_time = min_time * m_time_scale;
}

DataProcessor::~DataProcessor()
{
//nothing to do at all
}

void DataProcessor::getConfiguration(time_scale timeScale, string& timeScaleStr,
		time_sec& minOnTime, time_sec& sessionCutTime, string& criterion)
{

	if (int(m_time_scale) == 1000)
	{
		timeScaleStr = "miliseconds";
		timeScale = milliseconds;
	}
	else if (int(m_time_scale) == 1)
	{
		timeScaleStr = "seconds";
		timeScale = seconds;
	}
	else
	{
		cerr << "Some problem ocurred getting the time scalle\n"
				<< "m_timeScale=" << m_time_scale
				<< ", but it should be 1 or 1000" << endl;
		timeScaleStr = "seconds";
		timeScale = seconds;
	}
	minOnTime = m_min_on_time;
	sessionCutTime = m_session_cut_time;
	criterion = informationCriterionParam;
}

int DataProcessor::calculate_v2(const string& experimentName,
		TraceDbManager* dbManager, NetworkTrace* netTrace)
{
	///iterator variables
	uint fcounter = 0;
	uint nflows = dbManager->getNumberOfFlows(experimentName);
	///protocol variables
	string flowStrData = "";
	int flowIntData = 0;
	time_sec startDalay = 0;		//time of the fist packet of the flow
	time_sec flowDuration = 0;	//duration of the flow
	list<int> ttlList;
	int ttl = 0;
	///flow-level variables
	list<time_sec> relativeTime;	//time relative to the 1st packet list
	long int nbytesMode1 = 0;
	long int nbytesMode2 = 0;
	long int nKbytesMode1 = 0;		//Number of kbytes (bytes/1024) of the
	long int nKbytesMode2 = 0;
	///packetSize variables
	//list<unsigned int> pslist;
	list<uint> pslist;
	list<packet_size> psFirstMode;
	list<packet_size> psSecondMode;
	///inter-deperture time variables
	list<time_sec> arrival_list;
	list<time_sec> interArrival_list; // list of inter arrival times of a flow
	list<time_sec> interArrival_fileStack;
	time_sec idt_pivot = 0;
	time_sec idt_next = 0;
	time_sec idt_start = 0;

	for (fcounter = 0; fcounter < nflows; fcounter++)
	{
		//NetworkFlow* netFlow = NetworkFlow::make_flow("dummy");
		NetworkFlow* netFlow = NetworkFlowFactory::make_flow("dummy");

		//reset temp vars
		flowStrData = "";
		flowIntData = 0;
		nbytesMode1 = 0;
		nbytesMode2 = 0;
		nKbytesMode1 = 0;
		nKbytesMode2 = 0;
		relativeTime.clear();
		pslist.clear();
		psFirstMode.clear();
		psSecondMode.clear();
		arrival_list.clear();
		interArrival_list.clear();
		interArrival_fileStack.clear();

		dbManager->getFlowPktData(experimentName, fcounter, "pktSize", pslist);
		//databaseInterface->getFlowData(experimentName, fcounter, "frame__len",
		//		pslist);

		// Evaluate packet-size and number ob kbytes for each mode
		for (list<uint>::iterator it = pslist.begin(); it != pslist.end(); it++)
		{
			if (*it <= PACKET_SIZE_MODE_CUT_VALUE)
			{
				psFirstMode.push_back(packet_size(*it));
				nbytesMode1 += *it;
			}
			else
			{
				psSecondMode.push_back(packet_size(*it));
				nbytesMode2 += *it;
			}
		}
		nKbytesMode1 = nbytesMode1 / 1024;
		nKbytesMode2 = nbytesMode2 / 1024;

		// load time-relative data. The time values are relative to the begin of
		// the experiment
		dbManager->getFlowPktData(experimentName, fcounter, "arrivalTime",
				relativeTime);
		//databaseInterface->getFlowData(experimentName, fcounter,
		//		"frame__time_relative", relativeTime);

		scalar_product(relativeTime, m_time_scale);

		// Evaluate interarriaval  times
		for (list<time_sec>::iterator it = relativeTime.begin();
				it != relativeTime.end(); it++)
		{
			if (it == relativeTime.begin())
			{
				idt_pivot = *it;
				idt_start = idt_pivot;
			}
			else
			{
				idt_next = *it;
				interArrival_list.push_back(idt_next - idt_pivot);
				arrival_list.push_back(idt_next - idt_start);
				idt_pivot = idt_next;
			}
		}

		// Separate file inter packet times
		for (list<time_sec>::iterator it = interArrival_list.begin();
				it != interArrival_list.end(); it++)
		{
			if (*it < m_session_cut_time)
			{
				interArrival_fileStack.push_back(*it);
			}
		}

		////////////////////////////////////////////////////////////////////////
		/// Flow-level Options
		////////////////////////////////////////////////////////////////////////
		startDalay = relativeTime.front();
		netFlow->setFlowStartDelay(startDalay);

		flowDuration = relativeTime.back() - relativeTime.front();
		netFlow->setFlowDuration(flowDuration);

		/// DS byte configuration -- now it is just set to zero
		netFlow->setFlowDsByte(0);

		/// set npackes and nkbytes
		netFlow->setNumberOfKbytes(nKbytesMode1 + nKbytesMode2);
		netFlow->setNumberOfPackets(pslist.size());

		////////////////////////////////////////////////////////////////////////
		/// Protocols Options
		////////////////////////////////////////////////////////////////////////

		/// L2 protocols
		netFlow->setLinkProtocol(PROTOCOL__ETHERNET);
		//TODO mac src/dst
		netFlow->setMacAddr("no-mac-src", "no-mac-dst");

		/// L3 protocols
		/// reference :
		/// http://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml

		//databaseInterface->getFlowData(experimentName, fcounter, "eth__type",
		//		&flowIntData);
		dbManager->getFlowData(experimentName, fcounter, "protocolNetwork",
				flowIntData);

		if (flowIntData == IPV4_CODE)
		{ /// IPv4
			netFlow->setNetworkProtocol(PROTOCOL__IPV4);
		}
		else if (flowIntData == (ARP_CODE || ARP_CODE_REV))
		{ /// ARP, Frame Relay ARP
			netFlow->setNetworkProtocol(PROTOCOL__ARP);
		}
		else if (flowIntData == IPV6_CODE)
		{ /// IPv6
			netFlow->setNetworkProtocol(PROTOCOL__ICMPV6);
		}
		else
		{ /// default IPv4
			netFlow->setNetworkProtocol(PROTOCOL__IPV4);
		}

		dbManager->getFlowData(experimentName, fcounter, "netSrc", flowStrData);
		netFlow->setNetworkSrcAddr(flowStrData);

		dbManager->getFlowData(experimentName, fcounter, "netDst", flowStrData);
		netFlow->setNetworkDstAddr(flowStrData);

		/// set ttl as the most frequent
		dbManager->getFlowPktData(experimentName, fcounter, "ttl", ttlList);
		ttl = mode(&ttlList);

		netFlow->setNetworkTtl(ttl);

		/// L4 Protocols
		/// reference: https://en.wikipedia.org/wiki/List_of_IP_protocol_numbers

		dbManager->getFlowData(experimentName, fcounter, "protocolTransport",
				flowIntData);
		if (flowIntData == ICMP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__ICMP);
		}
		else if (flowIntData == TCP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__TCP);
		}
		else if (flowIntData == UDP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__UDP);
		}
		else if (flowIntData == DCCP_CODE)
		{
			netFlow->setTransportDstPort(PROTOCOL__DCCP);
		}
		else if (flowIntData == GRE_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__GRE);
		}
		else if (flowIntData == ICMPV6_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__ICMPV6);
		}
		else if (flowIntData == SCTP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__SCTP);
		}
		else
		{
			netFlow->setTransportProtocol(PROTOCOL__NULL);
		}

		dbManager->getFlowData(experimentName, fcounter, "portSrc",
				flowIntData);
		netFlow->setTransportSrcPort(flowIntData);
		dbManager->getFlowData(experimentName, fcounter, "portDst",
				flowIntData);
		netFlow->setTransportDstPort(flowIntData);

		/// Application protocol
		/// reference :
		/// https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
		netFlow->setApplicationProtocol(
				aplicationProtocol(netFlow->getTransportProtocol(),
						netFlow->getTransportSrcPort(),
						netFlow->getTransportDstPort()));

		////////////////////////////////////////////////////////////////////////
		/// Packet-level Options
		////////////////////////////////////////////////////////////////////////

		netFlow->setInterDepertureTimeModels(
				fitModelsInterArrival(interArrival_fileStack,
						informationCriterionParam));

		//Session Times
		vector<time_sec>* onTimes = new vector<time_sec>;
		vector<time_sec>* offTimes = new vector<time_sec>;
		vector<unsigned int>* pktCounter = new vector<unsigned int>;
		vector<unsigned int>* fileSize = new vector<unsigned int>;
		//calcOnOff(interArrival_list, 7, 0.1, onTimes, offTimes);

		calcOnOff(interArrival_list, pslist, m_session_cut_time, m_min_on_time,
				onTimes, offTimes, pktCounter, fileSize);

		//netFlow->setInterSessionTimesOnOff(onTimes, offTimes);
		//netFlow->setSessionTimesOnOff(onTimes, offTimes);
		netFlow->setSessionTimesOnOff(onTimes, offTimes, pktCounter, fileSize);

		/// Packet size data
		netFlow->setPacketSizeModel(fitModelsPsSize(psFirstMode),
				fitModelsPsSize(psSecondMode), nKbytesMode1, nKbytesMode2,
				psFirstMode.size(), psSecondMode.size());

		netFlow->setFlowId(fcounter);

		////////////////////////////////////////////////////////////////////////
		/// Push-back the flow to Trace Flow-list
		////////////////////////////////////////////////////////////////////////
		netTrace->pushback_Netflow(netFlow);

	}

	return (0);
}

int DataProcessor::calculate(const string& experimentName,
		DatabaseInterface* databaseInterface, NetworkTrace* netTrace)
{



	///iterator variables
	long int fcounter = 0;
	long int nflows = 0;
	databaseInterface->getNumberOfFlows(experimentName, &nflows);


	///protocol variables
	string flowStrData = "";
	long int flowIntData = 0;
	time_sec startDalay = 0;		//time of the fist packet of the flow
	time_sec flowDuration = 0;	//duration of the flow
	list<long int> ttlList;
	int ttl = 0;

	///flow-level variables
	list<time_sec> relativeTime;	//time relative to the 1st packet list
	long int nbytesMode1 = 0;
	long int nbytesMode2 = 0;
	long int nKbytesMode1 = 0;		//Number of kbytes (bytes/1024) of the
	long int nKbytesMode2 = 0;

	///packetSize variables
	list<unsigned int> pslist;
	list<packet_size> psFirstMode;
	list<packet_size> psSecondMode;

	///inter-deperture time variables
	list<time_sec> arrival_list;
	list<time_sec> interArrival_list; // list of inter arrival times of a flow
	list<time_sec> interArrival_fileStack;

	time_sec idt_pivot = 0;
	time_sec idt_next = 0;
	time_sec idt_start = 0;

	for (fcounter = 0; fcounter < nflows; fcounter++)
	{
		cout << ".";
		///new network flow
		//NetworkFlow* netFlow = NetworkFlow::make_flow("dummy");
		NetworkFlow* netFlow = NetworkFlowFactory::make_flow("dummy");

		//TODO checar valor retornado de databaseInterface

		//reset temp vars
		flowStrData = "";
		flowIntData = 0;
		nbytesMode1 = 0;
		nbytesMode2 = 0;
		nKbytesMode1 = 0;
		nKbytesMode2 = 0;
		relativeTime.clear();
		pslist.clear();
		psFirstMode.clear();
		psSecondMode.clear();
		arrival_list.clear();
		interArrival_list.clear();
		interArrival_fileStack.clear();

		//load packet-size data
		databaseInterface->getFlowData(experimentName, fcounter, "frame__len",
				pslist);

		// Evaluate packet-size and number ob kbytes for each mode
		for (list<unsigned int>::iterator it = pslist.begin();
				it != pslist.end(); it++)
		{
			if (*it <= PACKET_SIZE_MODE_CUT_VALUE)
			{
				psFirstMode.push_back(packet_size(*it));
				nbytesMode1 += *it;
			}
			else
			{
				psSecondMode.push_back(packet_size(*it));
				nbytesMode2 += *it;
			}
		}
		nKbytesMode1 = nbytesMode1 / 1024;
		nKbytesMode2 = nbytesMode2 / 1024;

		// load time-relative data. The time values are relative to the begin of
		// the experiment
		databaseInterface->getFlowData(experimentName, fcounter,
				"frame__time_relative", relativeTime);
		scalar_product(relativeTime, m_time_scale);

		// Evaluate interarriaval  times
		for (list<time_sec>::iterator it = relativeTime.begin();
				it != relativeTime.end(); it++)
		{
			if (it == relativeTime.begin())
			{
				idt_pivot = *it;
				idt_start = idt_pivot;
			}
			else
			{
				idt_next = *it;
				interArrival_list.push_back(idt_next - idt_pivot);
				arrival_list.push_back(idt_next - idt_start);
				idt_pivot = idt_next;
			}
		}

		// Separate file inter packet times
		for (list<time_sec>::iterator it = interArrival_list.begin();
				it != interArrival_list.end(); it++)
		{
			if (*it < m_session_cut_time)
			{
				interArrival_fileStack.push_back(*it);
			}
		}

		////////////////////////////////////////////////////////////////////////
		/// Flow-level Options
		////////////////////////////////////////////////////////////////////////
		startDalay = relativeTime.front();
		netFlow->setFlowStartDelay(startDalay);

		flowDuration = relativeTime.back() - relativeTime.front();
		netFlow->setFlowDuration(flowDuration);

		/// DS byte configuration -- now it is just set to zero
		netFlow->setFlowDsByte(0);

		/// set npackes and nkbytes
		netFlow->setNumberOfKbytes(nKbytesMode1 + nKbytesMode2);
		netFlow->setNumberOfPackets(pslist.size());

		////////////////////////////////////////////////////////////////////////
		/// Protocols Options
		////////////////////////////////////////////////////////////////////////

		/// L2 protocols
		netFlow->setLinkProtocol(PROTOCOL__ETHERNET);
		//TODO mac src/dst
		netFlow->setMacAddr("no-mac-src", "no-mac-dst");

		/// L3 protocols
		/// reference :
		/// http://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml

		databaseInterface->getFlowData(experimentName, fcounter, "eth__type",
				&flowIntData);
		if (flowIntData == IPV4_CODE)
		{ /// IPv4
			netFlow->setNetworkProtocol(PROTOCOL__IPV4);
			databaseInterface->getFlowData(experimentName, fcounter, "ip__src",
					&flowStrData);
			netFlow->setNetworkSrcAddr(flowStrData);

			databaseInterface->getFlowData(experimentName, fcounter, "ip__dst",
					&flowStrData);

			netFlow->setNetworkDstAddr(flowStrData);
		}
		else if (flowIntData == (ARP_CODE || ARP_CODE_REV))
		{ /// ARP, Frame Relay ARP
			netFlow->setNetworkProtocol(PROTOCOL__ARP);
			databaseInterface->getFlowData(experimentName, fcounter, "ip__src",
					&flowStrData);
			netFlow->setNetworkSrcAddr(flowStrData);
			databaseInterface->getFlowData(experimentName, fcounter, "ip__dst",
					&flowStrData);
			netFlow->setNetworkDstAddr(flowStrData);
		}
		else if (flowIntData == IPV6_CODE)
		{ /// IPv6
			netFlow->setNetworkProtocol(PROTOCOL__ICMPV6);
			databaseInterface->getFlowData(experimentName, fcounter,
					"ipv6__addr", &flowStrData);
			netFlow->setNetworkSrcAddr(flowStrData);
			databaseInterface->getFlowData(experimentName, fcounter,
					"ipv6__dst", &flowStrData);
			netFlow->setNetworkDstAddr(flowStrData);
		}
		else
		{ /// default IPv4
			netFlow->setNetworkProtocol(PROTOCOL__IPV4);
			databaseInterface->getFlowData(experimentName, fcounter, "ip__src",
					&flowStrData);
			netFlow->setNetworkSrcAddr(flowStrData);

			databaseInterface->getFlowData(experimentName, fcounter, "ip__dst",
					&flowStrData);

			netFlow->setNetworkDstAddr(flowStrData);
		}

		/// set ttl as the most frequent
		databaseInterface->getFlowData(experimentName, fcounter, "ip__ttl",
				ttlList);
		ttl = mode(&ttlList);
		netFlow->setNetworkTtl(ttl);

		/// L4 Protocols
		/// reference: https://en.wikipedia.org/wiki/List_of_IP_protocol_numbers

		databaseInterface->getFlowData(experimentName, fcounter, "ip__proto",
				&flowIntData);
		if (flowIntData == ICMP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__ICMP);
		}
		else if (flowIntData == TCP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__TCP);
			databaseInterface->getFlowData(experimentName, fcounter,
					"tcp__dstport", &flowIntData);
			netFlow->setTransportSrcPort(flowIntData);
			databaseInterface->getFlowData(experimentName, fcounter,
					"tcp__srcport", &flowIntData);
			netFlow->setTransportDstPort(flowIntData);
		}
		else if (flowIntData == UDP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__UDP);
			databaseInterface->getFlowData(experimentName, fcounter,
					"udp__dstport", &flowIntData);
			netFlow->setTransportSrcPort(flowIntData);
			databaseInterface->getFlowData(experimentName, fcounter,
					"udp__srcport", &flowIntData);
			netFlow->setTransportDstPort(flowIntData);
		}
		else if (flowIntData == DCCP_CODE)
		{
			netFlow->setTransportDstPort(PROTOCOL__DCCP);
			databaseInterface->getFlowData(experimentName, fcounter,
					"dccp__dstport", &flowIntData);
			netFlow->setTransportSrcPort(flowIntData);
			databaseInterface->getFlowData(experimentName, fcounter,
					"dccp__srcport", &flowIntData);
			netFlow->setTransportDstPort(flowIntData);
		}
		else if (flowIntData == GRE_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__GRE);
		}
		else if (flowIntData == ICMPV6_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__ICMPV6);
		}
		else if (flowIntData == SCTP_CODE)
		{
			netFlow->setTransportProtocol(PROTOCOL__SCTP);
			databaseInterface->getFlowData(experimentName, fcounter,
					"sctp__dstport", &flowIntData);
			netFlow->setTransportSrcPort(flowIntData);
			databaseInterface->getFlowData(experimentName, fcounter,
					"sctp__srcport", &flowIntData);
			netFlow->setTransportDstPort(flowIntData);
		}
		else
		{
			netFlow->setTransportProtocol(PROTOCOL__NULL);
		}

		/// Application protocol
		/// reference :
		/// https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
		netFlow->setApplicationProtocol(
				aplicationProtocol(netFlow->getTransportProtocol(),
						netFlow->getTransportSrcPort(),
						netFlow->getTransportDstPort()));

		////////////////////////////////////////////////////////////////////////
		/// Packet-level Options
		////////////////////////////////////////////////////////////////////////

		/// Inter-packet times
		netFlow->setInterDepertureTimeModels(
				fitModelsInterArrival(interArrival_fileStack,
						informationCriterionParam));

		//Session Times
		vector<time_sec>* onTimes = new vector<time_sec>;
		vector<time_sec>* offTimes = new vector<time_sec>;
		vector<unsigned int>* pktCounter = new vector<unsigned int>;
		vector<unsigned int>* fileSize = new vector<unsigned int>;
		//calcOnOff(interArrival_list, 7, 0.1, onTimes, offTimes);

		calcOnOff(interArrival_list, pslist, m_session_cut_time, m_min_on_time,
				onTimes, offTimes, pktCounter, fileSize);

		netFlow->setSessionTimesOnOff(onTimes, offTimes, pktCounter, fileSize);

		/// Packet size data
		netFlow->setPacketSizeModel(fitModelsPsSize(psFirstMode),
				fitModelsPsSize(psSecondMode), nKbytesMode1, nKbytesMode2,
				psFirstMode.size(), psSecondMode.size());

		netFlow->setFlowId(fcounter);

		////////////////////////////////////////////////////////////////////////
		/// Push-back the flow to Trace Flow-list
		////////////////////////////////////////////////////////////////////////
		netTrace->pushback_Netflow(netFlow);


	}

	cout << " done" << std::endl;
	return (0);
}

protocol DataProcessor::aplicationProtocol(protocol transportProtocol,
		port_number srcPort, port_number dstPort)
{

	if ((transportProtocol == PROTOCOL__TCP)
			&& ((srcPort == 443) || (dstPort == 443)))
	{
		//cout << "https" << endl;
		return (PROTOCOL__HTTPS);
	}

	if ((transportProtocol == PROTOCOL__TCP)
			&& ((srcPort == 20) || (dstPort == 20) || (srcPort == 21)
					|| (dstPort == 21)))
	{
		//cout << "ftp" << endl;
		return (PROTOCOL__FTP);
	}

	if ((transportProtocol == PROTOCOL__TCP)
			&& ((srcPort == 80) || (dstPort == 80)))
	{
		//cout << "http" << endl;
		return (PROTOCOL__HTTP);
	}

	if ((transportProtocol == PROTOCOL__TCP)
			&& ((srcPort == 179) || (dstPort == 179)))
	{
		//cout << "bgp" << endl;
		return (PROTOCOL__BGP);
	}

	if ((transportProtocol == PROTOCOL__UDP)
			&& ((srcPort == 67) || (dstPort == 67) || (srcPort == 68)
					|| (dstPort == 68)))
	{
		//cout << "dhcp" << endl;
		return (PROTOCOL__DHCP);
	}

	if (((transportProtocol == PROTOCOL__TCP)
			|| (transportProtocol == PROTOCOL__UDP))
			&& ((srcPort == 161) || (dstPort == 161)))
	{
		//cout << "snmp" << endl;
		return (PROTOCOL__SNMP);
	}

	if (((transportProtocol == PROTOCOL__TCP)
			|| (transportProtocol == PROTOCOL__UDP))
			&& ((srcPort == 53) || (dstPort == 53)))
	{
		//cout << "dns" << endl;
		return (PROTOCOL__DNS);
	}

	if (((transportProtocol == PROTOCOL__TCP)
			|| (transportProtocol == PROTOCOL__UDP))
			&& ((srcPort == 22) || (dstPort == 22)))
	{
		//cout << "ssh" << endl;
		return (PROTOCOL__SSH);
	}

	if (((transportProtocol == PROTOCOL__TCP)
			|| (transportProtocol == PROTOCOL__UDP))
			&& ((srcPort == 23) || (dstPort == 23)))
	{
		//cout << "telnet" << endl;
		return (PROTOCOL__Telnet);
	}

	if (((transportProtocol == PROTOCOL__TCP)
			|| (transportProtocol == PROTOCOL__UDP))
			&& ((srcPort == 49) || (dstPort == 49)))
	{
		//cout << "cacacs" << endl;
		return (PROTOCOL__TACACS);
	}

	return (PROTOCOL__NULL);

}

list<StochasticModelFit>* DataProcessor::fitModelsInterArrival(
		list<double>& empiricalData, const string& criterion)
{

	const int m = empiricalData.size(); //empirical data-size
//vars
	int counter = 0;
	list<StochasticModelFit>* modelList = new list<StochasticModelFit>;
	StochasticModelFit smf = StochasticModelFit();
	vec paramVec = zeros<vec>(2);
	vec infoCriterion = zeros<vec>(2);

	if (m == 0)
	{
		smf.set(SINGLE_PACKET, 0, 0, datum::inf, datum::inf);
		modelList->push_back(smf);

	}
	else if (m == 1)
	{
		smf.set(CONSTANT, *empiricalData.begin(), 0, datum::inf, datum::inf);
		modelList->push_back(smf);

	}
	else if (m < minimumAmountOfPackets)
	{
		//Inter-arrival vec
		vec interArrival = zeros<vec>(m);
		counter = 0;
		for (list<double>::iterator it = empiricalData.begin();
				it != empiricalData.end(); it++)
		{
			interArrival(counter) = *it + min_time;
			counter++;
		}

		//Constant
		constantFitting(interArrival, paramVec, infoCriterion);
		smf.set(CONSTANT, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));

		modelList->push_back(smf);
	}
	else
	{
		//Inter-arrival vec
		vec interArrival = zeros<vec>(m);
		counter = 0;
		for (list<double>::iterator it = empiricalData.begin();
				it != empiricalData.end(); it++)
		{
			interArrival(counter) = *it + min_time;
			counter++;
		}

		//Empirical CDF of interArrival
		vec* interArrivalCdf = empiricalCdf(empiricalData);

		//Weibull
		weibullFitting(interArrival, *interArrivalCdf, paramVec, infoCriterion);
		smf.set(WEIBULL, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

		//normal
		normalFitting(interArrival, paramVec, infoCriterion);
		smf.set(NORMAL, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

		//exponential mean
		exponentialMeFitting(interArrival, paramVec, infoCriterion);
		smf.set(EXPONENTIAL_MEAN, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

		//exponential Linear Regression (LR)
		exponentialLrFitting(interArrival, *interArrivalCdf, paramVec,
				infoCriterion);
		smf.set(EXPONENTIAL_LINEAR_REGRESSION, paramVec(0), paramVec(1),
				infoCriterion(0), infoCriterion(1));
		modelList->push_back(smf);

		//pareto linear regression
		paretoLrFitting(interArrival, *interArrivalCdf, paramVec,
				infoCriterion);
		smf.set(PARETO_LINEAR_REGRESSION, paramVec(0), paramVec(1),
				infoCriterion(0), infoCriterion(1));
		modelList->push_back(smf);

		//pareto maximum likehood
		paretoMlhFitting(interArrival, *interArrivalCdf, paramVec,
				infoCriterion);
		smf.set(PARETO_MAXIMUM_LIKEHOOD, paramVec(0), paramVec(1),
				infoCriterion(0), infoCriterion(1));
		modelList->push_back(smf);

		//Cauchy
		cauchyFitting(interArrival, *interArrivalCdf, paramVec, infoCriterion);
		smf.set(CAUCHY, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

		//Constant
		constantFitting(interArrival, paramVec, infoCriterion);
		smf.set(CONSTANT, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

		if (criterion == "bic")
		{
			for (list<StochasticModelFit>::iterator it = modelList->begin();
					it != modelList->end(); it++)
			{
				it->setInforCriterion(BIC);
			}
		}
		else if (criterion != "aic")
		{
			cerr
					<< "Error @ DataProcessor::fitModelsInterArrival -> Invalid criterion argument: "
					<< criterion << endl;
			cerr << "AIC set as default" << endl;
		}

		delete interArrivalCdf;

		modelList->sort();
	}

//modelList->sort();

	return (modelList);
}

list<StochasticModelFit>* DataProcessor::fitModelsPsSize(
		list<double>& empiricalData)
{
//constants
//const int numberOfModels = 3;
	const int m = empiricalData.size(); //empirical data-size
//vars
	int counter = 0;
//StochasticModelFit* modelVet = NULL;
	vec paramVec = zeros<vec>(2);
	vec infoCriterion = zeros<vec>(2);

	StochasticModelFit smf = StochasticModelFit();
	list<StochasticModelFit>* modelList = new list<StochasticModelFit>;

	if (m == 0)
	{
		smf.set(NO_MODEL, 0, 0, datum::inf, datum::inf);
		modelList->push_back(smf);
	}
	else if (m == 1)
	{
		smf.set(CONSTANT, *empiricalData.begin(), 0, datum::inf, datum::inf);
		modelList->push_back(smf);
	}
	else
	{
		//Inter-arrival vec
		vec psList = zeros<vec>(m);
		counter = 0;
		for (list<double>::iterator it = empiricalData.begin();
				it != empiricalData.end(); it++)
		{
			psList(counter) = *it;
			counter++;
		}

		//Constant
		constantFitting(psList, paramVec, infoCriterion);
		smf.set(CONSTANT, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

		//normal
		normalFitting(psList, paramVec, infoCriterion);
		smf.set(NORMAL, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

		//exponential mean
		exponentialMeFitting(psList, paramVec, infoCriterion);
		smf.set(EXPONENTIAL_MEAN, paramVec(0), paramVec(1), infoCriterion(0),
				infoCriterion(1));
		modelList->push_back(smf);

	}

	return (modelList);
}

inline void DataProcessor::weibullFitting(const vec& interArrival,
		const vec& interArrivalCdf, vec& paramVec, vec& infoCriterion)
{
//init
	int iterations = 1500;
	double learning_rate = 0.01;
	vec theta = zeros(2);
	vec J_history = zeros(iterations);

//linearization
	vec y = log(-log(1.0 + diferential - interArrivalCdf));
	vec x = log(interArrival);
	mat* X = featureMatrix(x);

//gradient descendent
	gradientDescendent(*X, y, learning_rate, iterations, theta, J_history);

//parameter evaluation
	double weibull_alpha = theta(1);
	double weibull_betha = exp(-theta(0) / theta(1));

	if (isRealPositive(weibull_alpha, weibull_betha))
	{
		paramVec(0) = weibull_alpha;
		paramVec(1) = weibull_betha;
	}
	else
	{
		paramVec(0) = default_weibullAlpha;
		paramVec(1) = default_weibullBeta;
	}

	delete X;

//information criterion
	double aic = informationCriterion(interArrival, "weibull", paramVec, "aic");
	double bic = informationCriterion(interArrival, "weibull", paramVec, "bic");
	infoCriterion(0) = aic;
	infoCriterion(1) = bic;

}

inline void DataProcessor::normalFitting(const vec& interArrival, vec& paramVec,
		vec& infoCriterion)
{
	double sigma = stddev(interArrival);
	double mu = mean(interArrival);
	paramVec(0) = mu;
	paramVec(1) = sigma;

//information criterion
	double aic = informationCriterion(interArrival, "normal", paramVec, "aic");
	double bic = informationCriterion(interArrival, "normal", paramVec, "bic");
	infoCriterion(0) = aic;
	infoCriterion(1) = bic;
}

inline void DataProcessor::exponentialLrFitting(const vec& interArrival,
		const vec& interArrivalCdf, vec& paramVec, vec& infoCriterion)
{
//init
	int iterations = 1500;
	double learning_rate = 0.01;
	vec theta = zeros<vec>(2);
	vec J_history = zeros<vec>(iterations);

//linearization
	vec y = log(1.00 + diferential - interArrivalCdf);
//vec y = log(1.00 - interArrivalCdf);
	vec x = interArrival;
	mat* X = featureMatrix(x);

//gradient descendent
	gradientDescendent(*X, y, learning_rate, iterations, theta, J_history);

//parameter evaluation
	double exp_lambda = -theta(1);

	if (isRealPositive(exp_lambda))
	{
		paramVec(0) = exp_lambda;
	}
	else
	{
		paramVec(0) = default_exponentialLambda;
	}
	paramVec(1) = 0;

	delete X;

//information criterion
	double aic = informationCriterion(interArrival, "exponential", paramVec,
			"aic");
	double bic = informationCriterion(interArrival, "exponential", paramVec,
			"bic");
	infoCriterion(0) = aic;
	infoCriterion(1) = bic;
}

inline void DataProcessor::exponentialMeFitting(const vec& interArrival,
		vec& paramVec, vec& infoCriterion)
{
	paramVec(0) = 1.0 / mean(interArrival);
	paramVec(1) = 0;

//information criterion
	double aic = informationCriterion(interArrival, "exponential", paramVec,
			"aic");
	double bic = informationCriterion(interArrival, "exponential", paramVec,
			"bic");
	infoCriterion(0) = aic;
	infoCriterion(1) = bic;
}

inline void DataProcessor::paretoLrFitting(const vec& interArrival,
		const vec& interArrivalCdf, vec& paramVec, vec& infoCriterion)
{
//init
	int iterations = 1500;
	double learning_rate = 0.01;
	vec theta = zeros(2);
	vec J_history = zeros(iterations);

//linearization
	vec y = log(1.00 + diferential - interArrivalCdf);
//vec y = log(1.00 - interArrivalCdf);
	vec x = log(interArrival);
	mat* X = featureMatrix(x);

//gradient descendent
	gradientDescendent(*X, y, learning_rate, iterations, theta, J_history);

//parameter evaluation
	double pareto_alpha = -theta(1);
	double pareto_xm = interArrival.min();
	if (isRealPositive(pareto_alpha))
	{
		paramVec(0) = pareto_alpha;
	}
	else
	{
		paramVec(0) = default_paretoAlpha;
	}
	paramVec(1) = pareto_xm;

	delete X;

//information criterion
	double aic = informationCriterion(interArrival, "pareto", paramVec, "aic");
	double bic = informationCriterion(interArrival, "pareto", paramVec, "bic");
	infoCriterion(0) = aic;
	infoCriterion(1) = bic;
}

inline void DataProcessor::paretoMlhFitting(const vec& interArrival,
		const vec& interArrivalCdf, vec& paramVec, vec& infoCriterion)
{
	double pareto_xm = interArrival.min();
	int m = interArrival.size();
//vec logDiff = log(interArrival) - log(ones<vec>(m) * pareto_xm);
	vec logDiff = log(interArrival) - log(pareto_xm);
	double pareto_alpha = double(m) / (sum(logDiff));
	if (isRealPositive(pareto_alpha))
	{
		paramVec(0) = pareto_alpha;
	}
	else
	{
		paramVec(0) = default_paretoAlpha;
	}
	paramVec(1) = pareto_xm;

//information criterion
	double aic = informationCriterion(interArrival, "pareto", paramVec, "aic");
	double bic = informationCriterion(interArrival, "pareto", paramVec, "bic");
	infoCriterion(0) = aic;
	infoCriterion(1) = bic;
}

inline void DataProcessor::cauchyFitting(const vec& interArrival,
		const vec& interArrivalCdf, vec& paramVec, vec& infoCriterion)
{
//initialization
	int iterations = 1500;
	double learning_rate = 0.01;
	int m = interArrival.size();
//double minInterArrival = interArrival.min();
//double y_max = 1.0 / minInterArrival;

//avoid too high numbers when tan() tends to infinity
	vec interArrivalCdf_temp = zeros<vec>(m);

	for (int i = 0; i < m; i++)
	{
		if (interArrivalCdf(i) < almost_zero)
		{
			interArrivalCdf_temp(i) = almost_zero;
		}
		else if (interArrivalCdf(i) > almost_one)
		{
			interArrivalCdf_temp(i) = almost_one;
		}
		else
		{
			interArrivalCdf_temp(i) = interArrivalCdf(i);
		}
	}

	vec y = tan(datum::pi * (interArrivalCdf_temp - 0.5));
//for (int i = 0; i < m; i++)
//{
//	//avoid too high numbers when tan() tends to infinity
//	if (y(i) > y_max)
//		y(i) = y_max;
//}

//linearization
	vec J_history = zeros<vec>(iterations);
	vec theta = zeros(2);
	mat* X = featureMatrix(interArrival);

//gradient descendent
	gradientDescendent(*X, y, learning_rate, iterations, theta, J_history);

//parameter evaluation
	double cauchy_gamma = double(1.0) / theta(1);
	double cauchy_x0 = -theta(0) / theta(1);
	if (isRealPositive(cauchy_gamma))
	{
		paramVec(0) = cauchy_gamma;
		paramVec(1) = cauchy_x0;
	}
	else
	{
		paramVec(0) = default_cauchyGamma;
		paramVec(1) = default_cauchyX0;
	}

	delete X;

//information criterion
	double aic = informationCriterion(interArrival, "cauchy", paramVec, "aic");
	double bic = informationCriterion(interArrival, "cauchy", paramVec, "bic");
	infoCriterion(0) = aic;
	infoCriterion(1) = bic;

}

void DataProcessor::constantFitting(const vec& interArrival, vec& paramVec,
		vec& informationCriterion)
{
	paramVec(0) = mean(interArrival);
	paramVec(1) = 0;
	informationCriterion(0) = datum::inf;
	informationCriterion(1) = datum::inf;
}

template<typename T> bool DataProcessor::isEqual(const T* vet1, const T* vet2,
		const int size)
{
	for (int i = 0; i < size; i++)
	{
		if (vet1[i] != vet2[i])
		{
			return (false);
		}
	}

	return (true);
}

template<typename T> void DataProcessor::vectorC_to_list(list<T>* theList,
		T* vet, int size)
{
	for (int i = 0; i < size; i++)
	{
		theList->push_back(vet[i]);
	}
}

template<typename T>
void DataProcessor::quickSort(T* vet, int left, int right)
{
	int p = left;
	int i = 0;
	T ch = 0;
	int j = 0;

	for (i = left + 1; i <= right; i++)
	{
		j = i;
		if (vet[j] < vet[p])
		{
			ch = vet[j];
			while (j > p)
			{
				vet[j] = vet[j - 1];
				j--;
			}
			vet[j] = ch;
			p++;
		}
	}
	if (p - 1 >= left)
		quickSort(vet, left, p - 1);
	if (p + 1 <= right)
		quickSort(vet, p + 1, right);
}

template<typename T>
T DataProcessor::mode(list<T>* theList)
{
	T* vet;
	int listSize = theList->size();
	int i = 0;
	T candidate = 0;
	T prev = 0;
	int counter = 0;
	int largerCounter = 0;
	T mode = 0;
	vet = new T[listSize];

	if (listSize == 0)
	{
		return (0);
	}

	i = 0;

	for (typename list<T>::iterator it = theList->begin(); it != theList->end();
			it++)
	{
		vet[i] = *it;
		i++;
	}

	quickSort(vet, 0, listSize - 1);

	prev = vet[0];
	largerCounter = 0;
	for (i = 0; i < listSize; i++)
	{
		candidate = vet[i];
		if (candidate == prev)
		{
			counter++;
			if (counter > largerCounter)
			{
				largerCounter = counter;
				mode = vet[i];
			}
		}
		else
			counter = 1;

		prev = vet[i];
	}

	delete[] vet;

	return (mode);
}

template<typename T>
inline T* DataProcessor::list_to_cvector(list<T>* theList) const
{
	int listSize = theList->size();
	T* vet;
	int i = 0;

	vet = new T[listSize];
	if (listSize == 0)
	{
		return (0);
	}

	i = 0;
	for (typename list<T>::iterator it = theList->begin(); it != theList->end();
			it++)
	{
		vet[i] = *it;
		i++;
	}

	return (vet);
}
template<typename T>
inline void DataProcessor::delete_cvector(T* c_vet) const
{
	delete[] c_vet;
}

inline vec* DataProcessor::empiricalCdf(list<double>& empiricalData)
{
	double* cvet_empiricalData = list_to_cvector(&empiricalData);
	int data_size = empiricalData.size();
	quickSort(cvet_empiricalData, 0, data_size - 1);
	double x = cvet_empiricalData[0]; // anchor - fist element;
	int i1 = 0; // fist index
	int i2 = 0;
	double prob = 0;
	vec* interArrivalCdf = new vec(zeros<vec>(data_size));

	for (int i = 0; i < data_size; i++)
	{
		if (cvet_empiricalData[i] > x)
		{
			i2 = i - 1;
			prob = double(i) / data_size;
			for (int j = i1; j <= i2; j++)
			{
				(*interArrivalCdf)(j) = prob;
			}
			i1 = i;
			x = cvet_empiricalData[i];
		}
	}
//last position == 1
	(*interArrivalCdf)(data_size - 1) = 1;

	delete_cvector(cvet_empiricalData);
	return (interArrivalCdf);
}

inline mat* DataProcessor::featureMatrix(list<double>& empiricalData)
{
	int data_size = empiricalData.size();
	mat* featureMatrix = new mat(ones<mat>(data_size, 2));

	int i = 0;
	for (list<double>::iterator it = empiricalData.begin();
			it != empiricalData.end(); it++)
	{
		(*featureMatrix)(i, 1) = *it;
		i++;
	}

	return (featureMatrix);

}

inline mat* DataProcessor::featureMatrix(const vec& empiricalData)
{
	int data_size = empiricalData.size();
	mat* featureMatrix = new mat(ones<mat>(data_size, 2));
	for (int i = 0; i < data_size; i++)
	{
		(*featureMatrix)(i, 1) = empiricalData(i);
	}

	return (featureMatrix);
}

inline double DataProcessor::computeCost(const mat& X, const vec& y,
		const vec& theta)
{
	double J = .0;
	double jtemp = .0;
	int m = y.size();

	for (int i = 0; i < m; i++)
	{
		jtemp = jtemp + pow(double(theta(0) + theta(1) * X(i, 1) - y(i)), 2);

	}
	J = (1.0 / double(2 * m)) * jtemp;

	return (J);
}

//TODO O GRADIENT DESCENDENT PRECISA POSSUIR UMA CONDIÇÃO DE PARADA PARA CONVERGENCIA
//TODO 1500 STEPS PARA TODOS OS CASOS É MUITO, MUITO!!!
inline void DataProcessor::gradientDescendent(const mat& X, const vec& y,
		const double learning_rate, const int num_iters, vec& theta,
		vec& J_history)
{

	int m = y.n_rows; //number of training examples
	double temp1 = .0;
	double temp2 = .0;
	double delta1 = .0;
	double delta2 = .0;
	vec x = X.col(1);

	for (int i = 0; i < num_iters; i++)
	{
		temp1 = .0;
		temp2 = .0;
		delta1 = .0;
		delta2 = .0;

		//slope
		delta1 = sum(theta(0) + theta(1) * x - y) / double(m);
		delta2 = sum((theta(0) + theta(1) * x - y) % x) / double(m);

		//linear regression
		temp1 = theta(0) - learning_rate * delta1;
		temp2 = theta(1) - learning_rate * delta2;

		theta(0) = temp1;
		theta(1) = temp2;

		//Save the cost J in every iteration
		J_history(i) = computeCost(X, y, theta);

	}

}

inline double DataProcessor::informationCriterion(const vec& data,
		const string& functionName, const vec& paramVet,
		const string& criterion)
{
	double criterionVal = .0;
	double nEstimatedParameters = double(paramVet.size());
	double likehoodLogVal = logLikehood(data, functionName, paramVet);
	double logDataLen = log(double(data.size()));

	if (criterion == "aic")
	{
		criterionVal = 2 * nEstimatedParameters - 2 * likehoodLogVal;
	}
	else if (criterion == "bic")
	{
		criterionVal = nEstimatedParameters * logDataLen - 2 * likehoodLogVal;
	}
	else
	{
		cerr << "\nInvalid criterion or no criterion selected: " << criterion
				<< endl << "Selecting default criterion: AIC\n";
		criterionVal = 2 * nEstimatedParameters - 2 * likehoodLogVal;
	}

//check if it is a valid number
	if (isnan(criterionVal))
	{
		criterionVal = datum::inf;
	}

	return (criterionVal);

}

inline double DataProcessor::logLikehood(const vec& data,
		const string& functionName, const vec& paramVet)
{
	double param1 = paramVet(0);
	double param2 = paramVet(1);
	double m = data.size();
	double L = .0;
	int i = 0;
	vec data_prob = zeros<vec>(size(data));

	if (functionName == "weibull")
	{
		for (i = 0; i < m; i++)
			data_prob(i) = pdf_weibull(data(i), param1, param2);
	}
	else if (functionName == "normal")
	{
		for (i = 0; i < m; i++)
			data_prob(i) = pdf_normal(data(i), param1, param2);
	}
	else if (functionName == "uniform")
	{
		data_prob = ones<vec>(size(data)) * pdf_uniform(param1, param2);
	}
	else if (functionName == "exponential")
	{
		for (i = 0; i < m; i++)
			data_prob(i) = pdf_exponential(data(i), param1);
	}
	else if (functionName == "pareto")
	{
		for (i = 0; i < m; i++)
			data_prob(i) = pdf_pareto(data(i), param1, param2);
	}
	else if (functionName == "cauchy")
	{
		for (i = 0; i < m; i++)
			data_prob(i) = pdf_cauchy(data(i), param1, param2);
	}
	else
	{
		cerr << "Warning, no valid stochastic function selected: "
				<< functionName << endl
				<< "Likehood logarithm seted to -Inf (worst as possible)\n";
		return (datum::inf);
	}

	L = sum(log(data_prob));
	return (L);

}

inline double DataProcessor::pdf_weibull(double x, double alpha, double betha)
{
	if (x >= 0)
		return ((alpha / pow(betha, alpha)) * pow(x, alpha - 1.0)
				* exp(-(pow(x / betha, alpha))));
	else
		return (.0);
}

inline double DataProcessor::cdf_weibull(double x, double alpha, double betha)
{
	if (x >= 0)
		return (1.0 - exp(-pow(x / betha, alpha)));
	else
		return (.0);
}

inline double DataProcessor::pdf_exponential(double x, double lambda)
{
	if (x >= 0)
		return (lambda * exp(-lambda * x));
	else
		return (.0);

}

inline double DataProcessor::cdf_exponential(double x, double lambda)
{
	if (x >= 0)
		return (1.0 - exp(-lambda * x));
	else
		return (.0);
}

inline double DataProcessor::pdf_pareto(double x, double alpha, double xm)
{
	if (x >= xm)
		return (alpha * pow(xm, alpha) / (pow(x, alpha + 1.0)));
	else
		return (.0);
}

inline double DataProcessor::cdf_pareto(double x, double alpha, double xm)
{
	if (x >= xm)
		return (1.0 - pow(xm / x, alpha));
	else
		return (.0);
}

inline double DataProcessor::pdf_cauchy(double x, double gamma, double x0)
{
	return (1 / (datum::pi * gamma)
			* (pow(gamma, 2) / (pow(x - x0, 2) + pow(gamma, 2))));
}

inline double DataProcessor::cdf_cauchy(double x, double gamma, double x0)
{
	return ((1 / datum::pi) * atan((x - x0) / (gamma)) + 0.5);
}

inline double DataProcessor::pdf_normal(double x, double mu, double sigma)
{
	return (1 / (sqrt(2 * datum::pi) * sigma)
			* exp(-0.5 * pow((x - mu) / sigma, 2)));
}

inline double DataProcessor::cdf_normal(double x, double mu, double sigma)
{
	return (0.5 * (1 + erf((x - mu) / (sigma * sqrt(2)))));
}

inline double DataProcessor::pdf_uniform(double x_min, double x_max)
{
	return (1.0 / (x_max - x_min));
}

inline double DataProcessor::cdf_uniform(double x, double x_min, double x_max)
{
	if (x < x_min)
		return (0);
	else if (x > x_max)
		return (1);
	else
		return ((x - x_min) / (x_max - x_min));
}

bool DataProcessor::isRealPositive(double val)
{
	if (val < 0)
		return (false);
	if (isnan(val))
		return (false);
	if (isinf(val))
		return (false);
	return (true);
}

bool DataProcessor::isRealPositive(double val1, double val2)
{
	if (val1 < 0)
		return (false);
	if (val2 < 0)
		return (false);
	if (isnan(val1))
		return (false);
	if (isinf(val1))
		return (false);
	if (isnan(val2))
		return (false);
	if (isinf(val2))
		return (false);
	return (true);
}

void DataProcessor::printTestResult(string testName, bool result)
{
	string outRes;
	if (result == true)
		outRes = "ok";
	else
		outRes = "failed";

	cout << testName << ":\t\t" << outRes << endl;
}

bool DataProcessor::compareDouble(double val1, double val2)
{
	double error = 0;

	error = fabs(double(val1) - double(val2));
	if (error > double(1e-5))
		return (false);
	else
		return (true);

}

bool DataProcessor::compareDouble(double val1, double val2, double acErr)
{
	double error = 0;

	error = fabs(double(val1) - double(val2));
	if (error > double(acErr))
		return (false);
	else
		return (true);

}

bool DataProcessor::compareMat(mat& mat1, mat& mat2)
{
	int mat1_nc = mat1.n_cols;
	int mat1_nr = mat1.n_rows;
	int mat2_nc = mat2.n_cols;
	int mat2_nr = mat2.n_rows;

	if ((mat1_nc != mat2_nc) || (mat1_nr != mat2_nr))
	{
		cout << "mat1.n_cols and mat2.n_cols dont match. " << mat1_nc << ":"
				<< mat2_nc << " " << mat1_nr << ":" << mat2_nr << endl;
		return (false);
	}
	for (int i = 0; i < mat1_nr; i++)
	{
		for (int j = 0; j < mat1_nc; j++)
		{
			if (compareDouble(mat1(i, j), mat2(i, j), 1e-5))
			{
				//cout << " mat1(" << i << ", " << j << " ) = " << mat1(i,j);
				//cout << " mat2(" << i << ", " << j << " ) = " << mat2(i,j);
				return (false);
			}
		}
	}

	return (true);
}

void DataProcessor::save_data_on_file(const string& fileName, const mat& vet1,
		const mat& vet2)
{
	string file_out = fileName + ".txt";
	int m = vet1.size();

	ofstream writeOnFile(file_out, ios::out);
	if (!writeOnFile)
	{
		cerr << "file could not be oppeded" << endl;
		exit(1);
	}

	for (int i = 0; i < m; i++)
	{
		writeOnFile << vet1(i) << " " << vet2(i) << endl;
	}

}

void DataProcessor::save_data_on_file(const string& fileName,
		list<double>& theList)
{
	string file_out = fileName + ".txt";
//int m = theList.size();

	ofstream writeOnFile(file_out, ios::out);
	if (!writeOnFile)
	{
		cerr << "file could not be oppeded" << endl;
		exit(1);
	}

	for (list<double>::iterator it = theList.begin(); it != theList.end(); it++)
	{
		//list<T>::iterator it = randInt_list1.begin();
		writeOnFile << *it << " \n";
	}
}

bool DataProcessor::test_Mode()
{

	int randint1[] =
	{ 1, 8, 1, 13, 9, 2, 1, 15, 4, 3, 10, 4, 13, 2, 14, 7, 7 };
	int randint1_mode = 1;
	list<int> l_randint1;
	vectorC_to_list(&l_randint1, randint1, 17);

	int randint2[] =
	{ 5, 3, 6, 8, 14, 9, 11, 9, 11, 15, 8, 7, 12, 13, 14 };
	int randint2_mode = 8;
	list<int> l_randint2;
	vectorC_to_list(&l_randint2, randint2, 15);

	int randint3[] =
	{ 9, 10, 6, 3, 13, 13, 14, 7, 7, 1, 5, 1, 8, 5, 9, 2, 3, 4, 5, 6, 6, 6 };
	int randint3_mode = 6;
	list<int> l_randint3;
	vectorC_to_list(&l_randint3, randint3, 22);

	int randint4[] =
	{ 2, 1 };
	int randint4_mode = 1;
	list<int> l_randint4;
	vectorC_to_list(&l_randint4, randint4, 2);

	int randint5[] =
	{ 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
	int randint5_mode = 1;
	list<int> l_randint5;
	vectorC_to_list(&l_randint5, randint5, 10);

	if (mode(&l_randint1) != randint1_mode)
		return (false);
	if (mode(&l_randint2) != randint2_mode)
		return (false);
	if (mode(&l_randint3) != randint3_mode)
		return (false);
	if (mode(&l_randint4) != randint4_mode)
		return (false);
	if (mode(&l_randint5) != randint5_mode)
		return (false);

	return (true);
}

bool DataProcessor::test_QuickSort()
{
	int randIntA[] =
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int randIntA_size = 18;
	int randIntA_sorted[] =
	{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };
	int randIntB[] =
	{ 9, 2, 3, 3, 12, 15, 8, 14, 2, 7, 4, 6, 10, 2, 12 };
	int randIntB_size = 15;
	int randIntB_sorted[] =
	{ 2, 2, 2, 3, 3, 4, 6, 7, 8, 9, 10, 12, 12, 14, 15 };
	double randDoubleA[] =
	{ 0.20872, 12.25501, 5.88210, 2.77962, 8.74916, 2.97454, 4.39641, 12.19733,
			4.66014, 4.52477, 5.19375, 12.11314, 13.35458, 11.56941, 6.59435 };
	int randDoubleA_size = 15;
	double randDoubleA_sorted[] =
	{ 0.20872, 2.77962, 2.97454, 4.39641, 4.52477, 4.66014, 5.19375, 5.88210,
			6.59435, 8.74916, 11.56941, 12.11314, 12.19733, 12.25501, 13.35458 };

	quickSort(randIntA, 0, randIntA_size - 1);

	if (isEqual(randIntA, randIntA_sorted, randIntA_size) == false)
	{
		return (false);
	}

	quickSort(randIntB, 0, randIntB_size - 1);
	if (isEqual(randIntB, randIntB_sorted, randIntB_size) == false)
	{
		return (false);
	}

	quickSort(randDoubleA, 0, randDoubleA_size - 1);
	if (isEqual(randDoubleA, randDoubleA_sorted, randDoubleA_size) == false)
	{
		return (false);
	}

	return (true);

}

bool DataProcessor::test_list_tocvector()
{
	list<int> randInt_list1 =
	{ 9, 2, 3, 3, 12, 15, 8, 14, 2, 7, 4, 6, 10, 2, 12 };
	list<double> randInt_list2 =
	{ 2.2, 2.111, 3.16, 6.66, 13, 2.61 };
	list<int> randInt_list3 =
	{ 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1 };

	int* cvet1 = NULL;
	double* cvet2 = NULL;
	int* cvet3 = NULL;

	cvet1 = list_to_cvector(&randInt_list1);
	cvet2 = list_to_cvector(&randInt_list2);
	cvet3 = list_to_cvector(&randInt_list3);

	int i = 0;
	for (list<int>::iterator it = randInt_list1.begin();
			it != randInt_list1.end(); it++)
	{
		if (cvet1[i] != *it)
		{
			return (false);
		}
		//cout << *it << " ";
		i++;
	}
	i = 0;
	for (list<double>::iterator it = randInt_list2.begin();
			it != randInt_list2.end(); it++)
	{
		if (cvet2[i] != *it)
		{
			return (false);
		}
		//cout << *it << " ";
		i++;
	}
	i = 0;
	for (list<int>::iterator it = randInt_list3.begin();
			it != randInt_list3.end(); it++)
	{
		if (cvet3[i] != *it)
		{
			return (false);
		}
		//cout << *it << " ";
		i++;
	}

	delete_cvector(cvet1);
	delete_cvector(cvet2);
	delete_cvector(cvet3);

	return (true);
}

bool DataProcessor::test_empiricalCdf()
{
	list<double> interArrival_sample =
	{ 5.0000e-07, 1.5000e-06, 1.5000e-06, 2.5000e-06, 2.5000e-06, 3.5000e-06,
			4.5000e-06, 7.5000e-06, 1.2500e-05, 1.9500e-05, 3.1500e-05,
			4.3500e-05, 6.0500e-05, 7.6500e-05, 1.1050e-04, 1.4050e-04,
			1.8150e-04, 2.1150e-04, 2.3350e-04, 2.4350e-04, 2.5650e-04,
			3.0350e-04, 4.1250e-04, 1.3655e-03, 1.1593e-02, 3.2483e-02,
			8.1235e-02, 2.0939e-01, 5.0755e-01, 1.7351e+00 };
	vec emCdf_octave =
	{ 0.033333, 0.100000, 0.100000, 0.166667, 0.166667, 0.200000, 0.233333,
			0.266667, 0.300000, 0.333333, 0.366667, 0.400000, 0.433333,
			0.466667, 0.500000, 0.533333, 0.566667, 0.600000, 0.633333,
			0.666667, 0.700000, 0.733333, 0.766667, 0.800000, 0.833333,
			0.866667, 0.900000, 0.933333, 0.966667, 1.000000 };
//emCdf_octave = emCdf_octave.t();

	vec* emCdf_cpp = empiricalCdf(interArrival_sample);

	int list_size = interArrival_sample.size();
	for (int i = 0; i < list_size; i++)
	{
		if (!compareDouble(emCdf_octave(i), (*emCdf_cpp)(i), 1e-6))
		{
			cout << "cdf error @ " << "i:" << i << " res:" << (*emCdf_cpp)(i, 0)
					<< " expected:" << emCdf_octave(i, 0) << endl;
			return (false);
		}
	}

	delete emCdf_cpp;
	return (true);
}

bool DataProcessor::test_computeCost()
{
	list<double> interArrival_sample =
	{ 5.0000e-07, 1.5000e-06, 1.5000e-06, 2.5000e-06, 2.5000e-06, 3.5000e-06,
			4.5000e-06, 7.5000e-06, 1.2500e-05, 1.9500e-05, 3.1500e-05,
			4.3500e-05, 6.0500e-05, 7.6500e-05, 1.1050e-04, 1.4050e-04,
			1.8150e-04, 2.1150e-04, 2.3350e-04, 2.4350e-04, 2.5650e-04,
			3.0350e-04, 4.1250e-04, 1.3655e-03, 1.1593e-02, 3.2483e-02,
			8.1235e-02, 2.0939e-01, 5.0755e-01, 1.7351e+00 };
	mat* X = featureMatrix(interArrival_sample);
	vec emCdf_octave =
	{ 0.033333, 0.100000, 0.100000, 0.166667, 0.166667, 0.200000, 0.233333,
			0.266667, 0.300000, 0.333333, 0.366667, 0.400000, 0.433333,
			0.466667, 0.500000, 0.533333, 0.566667, 0.600000, 0.633333,
			0.666667, 0.700000, 0.733333, 0.766667, 0.800000, 0.833333,
			0.866667, 0.900000, 0.933333, 0.966667, 1.000000 };
//emCdf_octave = emCdf_octave.t();
	double J_expected1 = 0.17535;
	double J_expected2 = 1.5341;
	double J_expected3 = 9.0244;

	vec theta = zeros(2);

	double J = computeCost(*X, emCdf_octave, theta);
	if (!compareDouble(J, J_expected1, 0.001))
	{
		//cout << "J1 = " << J << "\nJ_expected1 = "<< J_expected1 << endl;
		return (false);
	}
	theta(0) = 2;
	theta(1) = 2;
	J = computeCost(*X, emCdf_octave, theta);
	if (!compareDouble(J, J_expected2, 0.001))
	{
		//cout << "J2 = " << J << "\nJ_expected2 = "<< J_expected2 << endl;
		return (false);
	}
	theta(0) = 1.5;
	theta(1) = 12;
	J = computeCost(*X, emCdf_octave, theta);
	if (!compareDouble(J, J_expected3, 0.001))
	{
		//cout << "J3 = " << J << "\nJ_expected3 = "<< J_expected3 << endl;
		return (false);
	}

	delete X;
	return (true);
}

bool DataProcessor::test_informationCriterion()
{
//mat M;
//M.load("CppAlgorithms/data_sample.txt");
//vec interArrival = sort(M.col(3)) + min_time;
	vec interArrival = sort(interArrivalSample) + min_time;

	int m = interArrival.size();
	list<double> list_interArrival;
	vec* interArrivalCdf;
	for (int i = 0; i < m; i++)
	{
		list_interArrival.push_back(interArrival(i));
	}
	interArrivalCdf = empiricalCdf(list_interArrival);

//gradient descendent
	int iterations = 1500;
	double learning_rate = 0.01;
	double ex_bic = -33712.7042;
	double ex_aic = -33724.6567;
	vec theta = zeros(2);
	vec J_history = zeros(iterations);

	vec y = log(-log(1.0 + diferential - *interArrivalCdf));
	vec x = log(interArrival);
	mat* X = featureMatrix(x);

	gradientDescendent(*X, y, learning_rate, iterations, theta, J_history);

	double weibull_alpha = theta(1);
	double weibull_betha = exp(-theta(0) / theta(1));
	vec paramVec =
	{ weibull_alpha, weibull_betha };
	double weibull_bic = informationCriterion(interArrival, "weibull", paramVec,
			"bic");
	double weibull_aic = informationCriterion(interArrival, "weibull", paramVec,
			"aic");

//cout << "BIC: " << weibull_bic << " x " << ex_bic << endl;
//cout << "AIC: " << weibull_aic << " x " << ex_aic << endl;
	if (!compareDouble(weibull_bic, ex_bic, 0.001))
		return (false);
	else if (!compareDouble(weibull_aic, ex_aic, 0.001))
		return (false);

	return (true);
}

bool DataProcessor::test_pdf_weibull()
{
	double alpha = 1.5000;
	double betha = 0.3;
	vec data =
	{ 0.010000, 2.300000, 0.500000 };
	vec dataprob_eval1 =
	{ 9.0733e-01, 8.3571e-09, 7.5066e-01 };
	vec dataprob_test = zeros(3);

	dataprob_test(0) = pdf_weibull(data(0), alpha, betha);
	dataprob_test(1) = pdf_weibull(data(1), alpha, betha);
	dataprob_test(2) = pdf_weibull(data(2), alpha, betha);

	if (compareDouble(dataprob_test(0), dataprob_eval1(0))
			&& compareDouble(dataprob_test(1), dataprob_eval1(1))
			&& compareDouble(dataprob_test(2), dataprob_eval1(2)))
	{
		return (true);
	}

	return (false);
}

bool DataProcessor::test_cdf_weibull()
{
	double alpha = 0.5;
	double betha = 1.5;
	vec data =
	{ 0.010000, 2.300000, 0.500000 };
	vec dataprob_eval1 =
	{ 0.078405, 0.710117, 0.438616 };
	vec dataprob_test = zeros(3);

	dataprob_test(0) = cdf_weibull(data(0), alpha, betha);
	dataprob_test(1) = cdf_weibull(data(1), alpha, betha);
	dataprob_test(2) = cdf_weibull(data(2), alpha, betha);
//cout << dataprob_test(0) << " : " << dataprob_test(1) << " : " << dataprob_test(2) << endl;

	if (compareDouble(dataprob_test(0), dataprob_eval1(0))
			&& compareDouble(dataprob_test(1), dataprob_eval1(1))
			&& compareDouble(dataprob_test(2), dataprob_eval1(2)))
	{
		return (true);
	}

	return (false);
}

bool DataProcessor::test_gradientDescendent()
{
//mat M;
//M.load("CppAlgorithms/data_sample.txt");
//vec interArrival = sort(M.col(3)) + min_time;
	vec interArrival = sort(interArrivalSample) + min_time;

	int m = interArrival.size();
	list<double> list_interArrival;
	vec* interArrivalCdf;
	for (int i = 0; i < m; i++)
	{
		list_interArrival.push_back(interArrival(i));
	}
	interArrivalCdf = empiricalCdf(list_interArrival);

//gradient descendent
	int iterations = 1500;
	double learning_rate = 0.01;
	vec theta = zeros(2);
	vec J_history = zeros(iterations);

	vec y = log(-log(1.0 + diferential - *interArrivalCdf));
	vec x = log(interArrival);
	mat* X = featureMatrix(x);

	gradientDescendent(*X, y, learning_rate, iterations, theta, J_history);

//expected values
	double Jlast = 0.083359;
	double theta0 = 1.660453;
	double theta1 = 0.248811;
	double acErr = 0.05;
//cout << "res: " << J_history(iterations - 1) << "     " << "exp: " << Jlast
//		<< endl;
//cout << "res: " << theta(0) << "     " << "exp: " << theta0 << endl;
//cout << "res: " << theta(1) << "     " << "exp: " << theta1 << endl;

	if (!compareDouble(J_history(iterations - 1), Jlast, acErr))
	{
		return (false);
	}
	else if (!compareDouble(theta(0), theta0, acErr))
	{
		return (false);
	}
	else if (!compareDouble(theta(1), theta1, acErr))
	{
		return (false);
	}

	delete interArrivalCdf;
	delete X;
	return (true);

}

bool DataProcessor::test_pdf_exponential()
{
	vec data =
	{ 0.010000, 2.300000, 0.500000 };

	double lambda1 = 0.50000;
	vec val1 =
	{ 0.49751, 0.15832, 0.38940 };
	vec res1 = zeros<vec>(3);

	double lambda2 = 3.0;
	vec val2 =
	{ 2.9113366, 0.0030234, 0.6693905 };
	vec res2 = zeros<vec>(3);

	for (int i = 0; i < 3; i++)
	{
		//exponential
		res1(i) = pdf_exponential(data(i), lambda1);
		res2(i) = pdf_exponential(data(i), lambda2);

		if (!compareDouble(res1(i), val1(i)))
		{
			return (false);
		}
		if (!compareDouble(res2(i), val2(i)))
		{
			return (false);
		}

	}

	return (true);
}

bool DataProcessor::test_cdf_exponential()
{
	vec data =
	{ 0.010000, 2.300000, 0.500000 };

	double lambda1 = 0.50000;
	vec val1 =
	{ 0.0049875, 0.6833632, 0.2211992 };
	vec res1 = zeros<vec>(3);

	double lambda2 = 3.0;
	vec val2 =
	{ 0.029554, 0.998992, 0.776870 };
	vec res2 = zeros<vec>(3);

	for (int i = 0; i < 3; i++)
	{
		//exponential
		res1(i) = cdf_exponential(data(i), lambda1);
		res2(i) = cdf_exponential(data(i), lambda2);

		if (!compareDouble(res1(i), val1(i)))
		{
			return (false);
		}
		if (!compareDouble(res2(i), val2(i)))
		{
			return (false);
		}
	}

	return (true);
}

bool DataProcessor::test_pdf_pareto()
{

	vec data =
	{ 0.010000, 2.300000, 0.500000 };

	double alpha1 = 0.50000;
	double xm1 = 1.0;
	vec val1 =
	{ 0, 0.14334, 0 };
	vec res1 = zeros<vec>(3);
	double xm2 = 0.00001;
	double alpha2 = 3.0;
	vec val2 =
	{ 3.0000e-07, 1.0720e-16, 4.8000e-14 };
	vec res2 = zeros<vec>(3);

	for (int i = 0; i < 3; i++)
	{
		//exponential
		res1(i) = pdf_pareto(data(i), alpha1, xm1);
		res2(i) = pdf_pareto(data(i), alpha2, xm2);

		//cout << "res1(i) " << res1(i) << ", res2(i) " << res2(i) << endl;
		if (!compareDouble(res1(i), val1(i)))
		{
			return (false);
		}
		if (!compareDouble(res2(i), val2(i)))
		{
			return (false);
		}
	}

	return (true);
}

bool DataProcessor::test_cdf_pareto()
{

	vec data =
	{ 0.010000, 2.300000, 0.500000 };

	double alpha1 = 0.50000;
	double xm1 = 0.001;
	vec val1 =
	{ 0.68377, 0.97915, 0.95528 };
	vec res1 = zeros<vec>(3);
	double xm2 = 0.2;
	double alpha2 = 1;
	vec val2 =
	{ 0, 0.91304, 0.60000 };
	vec res2 = zeros<vec>(3);

	for (int i = 0; i < 3; i++)
	{
		//exponential
		res1(i) = cdf_pareto(data(i), alpha1, xm1);
		res2(i) = cdf_pareto(data(i), alpha2, xm2);

		//cout << "res1(i) " << res1(i) << ", res2(i) " << res2(i) << endl;
		if (!compareDouble(res1(i), val1(i)))
		{
			return (false);
		}
		if (!compareDouble(res2(i), val2(i)))
		{
			return (false);
		}
	}
	return (true);
}

bool DataProcessor::test_pdf_cauchy()
{

	vec data =
	{ 0.010000, 2.300000, 0.500000 };
	double gamma1 = 0.50000;
	double x01 = 0.001;

	vec val1 =
	{ 0.636414, 0.028752, 0.318947 };
	vec res1 = zeros<vec>(3);
	double x02 = 2;
	double gamma2 = 2;

	vec val2 =
	{ 0.079976, 0.155653, 0.101859 };
	vec res2 = zeros<vec>(3);

	for (int i = 0; i < 3; i++)
	{
		//exponential
		res1(i) = pdf_cauchy(data(i), gamma1, x01);
		res2(i) = pdf_cauchy(data(i), gamma2, x02);

		//cout << "res1(i) " << res1(i) << ", res2(i) " << res2(i) << endl;
		if (!compareDouble(res1(i), val1(i)))
		{
			return (false);
		}
		if (!compareDouble(res2(i), val2(i)))
		{
			return (false);
		}
	}
	return (true);
}

bool DataProcessor::test_cdf_cauchy()
{

	vec data =
	{ 0.010000, 2.300000, 0.500000 };
	double gamma1 = 0.50000;
	double x01 = 0.001;

	vec val1 =
	{ 0.50573, 0.93183, 0.74968 };
	vec res1 = zeros<vec>(3);
	double x02 = 2;
	double gamma2 = 2;

	vec val2 =
	{ 0.25080, 0.54739, 0.29517 };
	vec res2 = zeros<vec>(3);

	for (int i = 0; i < 3; i++)
	{
		//exponential
		res1(i) = cdf_cauchy(data(i), gamma1, x01);
		res2(i) = cdf_cauchy(data(i), gamma2, x02);

		//cout << "res1(i) " << res1(i) << ", res2(i) " << res2(i) << endl;
		if (!compareDouble(res1(i), val1(i)))
		{
			return (false);
		}
		if (!compareDouble(res2(i), val2(i)))
		{
			return (false);
		}
	}
	return (true);
}

bool DataProcessor::test_fitModelsInterArrival()
{
//mat M;
//M.load("CppAlgorithms/data_sample.txt");
//vec interArrival = sort(M.col(3)) + min_time;
	vec interArrival = sort(interArrivalSample) + min_time;

	int m = interArrival.size();
	list<double> list_interArrival;
	vec* interArrivalCdf;
	for (int i = 0; i < m; i++)
	{
		list_interArrival.push_back(interArrival(i));
	}
	interArrivalCdf = empiricalCdf(list_interArrival);

//list of expected values
	double weibull_alpha = 0.24881;
	double weibull_betha = 0.0012639;
	double normal_mu = 0.054792;
	double normal_sigma = 0.205602;
	double exponentialMe_lambda = 18.250783;
	double exponentialLr_lambda = 2.023115;
	double paretoLr_alpha = 0.225451;
	double paretoLr_xm = 0.000000;
	double paretoMlh_alpha = 0.171674;
	double paretoMlh_xm = 0.000000;
	double cauchy_gamma = 0.000067;
	double cauchy_x0 = 0.005750;
	double acErr = 0.001;

	vec paramVec = zeros<vec>(2);
	vec infoCriterion = zeros<vec>(2);

	weibullFitting(interArrival, *interArrivalCdf, paramVec, infoCriterion);
	if (!compareDouble(paramVec(0), weibull_alpha, acErr))
	{
		cout << "weibullFitting alpha" << endl;
		return (false);
	}
	if (!compareDouble(paramVec(1), weibull_betha, acErr))
	{
		cout << "weibullFitting betha" << endl;
		return (false);
	}

	normalFitting(interArrival, paramVec, infoCriterion);
	if (!compareDouble(paramVec(0), normal_mu, acErr))
	{
		cout << "normalFitting mu" << endl;
		return (false);
	}
	if (!compareDouble(paramVec(1), normal_sigma, acErr))
	{
		cout << "normalFitting sigma" << endl;
		return (false);
	}

	exponentialMeFitting(interArrival, paramVec, infoCriterion);
	if (!compareDouble(paramVec(0), exponentialMe_lambda, acErr))
	{
		cout << "exponentialLrFitting lambda" << endl;
		return (false);
	}

	exponentialLrFitting(interArrival, *interArrivalCdf, paramVec,
			infoCriterion);
	if (!compareDouble(paramVec(0), exponentialLr_lambda, acErr))
	{
		cout << exponentialLr_lambda << ":" << paramVec(0) << endl;
		cout << "exponentialLrFitting lambda" << endl;
		return (false);
	}

	paretoLrFitting(interArrival, *interArrivalCdf, paramVec, infoCriterion);
	if (!compareDouble(paramVec(0), paretoLr_alpha, acErr))
	{
		cout << "paretoLrFitting alpha" << endl;
		return (false);
	}
	if (!compareDouble(paramVec(1), paretoLr_xm, acErr))
	{
		cout << "paretoLrFitting xm" << endl;
		return (false);
	}

	paretoMlhFitting(interArrival, *interArrivalCdf, paramVec, infoCriterion);
	if (!compareDouble(paramVec(0), paretoMlh_alpha, acErr))
	{
		cout << "paretoMlhFitting alpha" << endl;
		return (false);
	}
	if (!compareDouble(paramVec(1), paretoMlh_xm, acErr))
	{
		cout << "paretoMlhFitting xm" << endl;
		return (false);
	}

	cauchyFitting(interArrival, *interArrivalCdf, paramVec, infoCriterion);
	if (!compareDouble(paramVec(0), cauchy_gamma, acErr))
	{
		cout << "cauchyFitting gamma " << paramVec(0) << endl;
		return (false);
	}
	if (!compareDouble(paramVec(1), cauchy_x0, acErr))
	{
		cout << "cauchyFitting x0 " << paramVec(1) << "::" << cauchy_x0 << endl;
		return (false);
	}

	delete interArrivalCdf;

	return (true);
}

bool DataProcessor::test_modelSelection()
{

//mat M;
//M.load("CppAlgorithms/data_sample.txt");
//vec interArrival = sort(M.col(3)) + min_time;
	vec interArrival = sort(interArrivalSample) + min_time;
	int m = interArrival.size();

	list<double> list_interArrival;
	vec* interArrivalCdf;
	for (int i = 0; i < m; i++)
	{
		list_interArrival.push_back(interArrival(i));
	}
	interArrivalCdf = empiricalCdf(list_interArrival);

	list<StochasticModelFit>* modelList = NULL;
	modelList = fitModelsInterArrival(list_interArrival, "aic");

//int numberOfModels = modelVet[0].size;
//for (int i = 0; i < numberOfModels; i++)
//	cout << modelVet[i].modelName << " " << "modelVet[" << i << "] = "
//			<< modelVet[i].aic << endl;

	delete interArrivalCdf;
	delete modelList;
	return (true);
}

void DataProcessor::regression_tests()
{
	RegressionTests rt = RegressionTests();
	min_time = 5e-7;

	rt.printHeader("class DataProcessor");
	rt.printTestResult("QuickSort", test_QuickSort());
	rt.printTestResult("List Mode", test_Mode());
	rt.printTestResult("ListToVector", test_list_tocvector());
	rt.printTestResult("Empirical CDF", test_empiricalCdf());
	rt.printTestResult("Cost Function", test_computeCost());

	rt.printTestResult("Weibull PDF", test_pdf_weibull());
	rt.printTestResult("Weibull CDF", test_cdf_weibull());
	rt.printTestResult("Gradient Descendent", test_gradientDescendent());

	rt.printTestResult("Information Criterion", test_informationCriterion());
	rt.printTestResult("Exponential PDF", test_pdf_exponential());
	rt.printTestResult("Exponential CDF", test_cdf_exponential());
	rt.printTestResult("PDF Pareto", test_pdf_pareto());
	rt.printTestResult("Pareto CDF", test_cdf_pareto());
	rt.printTestResult("PDF Cauchy", test_pdf_cauchy());
	rt.printTestResult("CDF Cauchy", test_cdf_cauchy());
	rt.printTestResult("All fitting", test_fitModelsInterArrival());
	rt.printTestResult("Model selection", test_modelSelection());
	rt.printTestResult("calcOnOff times", test_calcOnOff());
	rt.printTestResult("calcOnOff2 times", test_calcOnOff2());
//rt.wait_int();

}

//void DataProcessor::calcOnOff(list<time_sec>& deltaVet, const time_sec cut_time,
//		const time_sec min_on_time, list<time_sec>* onTimes,
//		list<time_sec>* offTimes)
/*
 void DataProcessor::calcOnOff(list<time_sec>& deltaVet, const time_sec cut_time,
 const time_sec min_on_time, vector<time_sec>* onTimes,
 vector<time_sec>* offTimes)
 {
 // log
 MESSER_LOG_INIT(ERROR);
 MESSER_DEBUG("<%s, %s>");

 // data structures
 unsigned int m = deltaVet.size();
 list<time_sec> arrivalVet;
 cumulativeDistribution(deltaVet, &arrivalVet);
 time_sec* arrival_time = list_to_cvector(&arrivalVet);
 time_sec* delta_time = list_to_cvector(&deltaVet);
 list<time_sec> onOff;

 // vars inity
 time_sec timebuffer = 0;
 unsigned int last_off = 0;
 unsigned int i = 0;
 unsigned int j = 0;

 if (m == 0)
 {
 onOff.push_back(min_on_time);
 }
 else
 {

 for (i = 0; i < m; i++)
 {
 if ((delta_time[i] > cut_time) || (i == (m - 1)))
 {

 if (i == 0) // the first times is off
 {
 j++;
 onOff.push_back(min_on_time);
 j++;
 onOff.push_back(delta_time[i]);
 last_off = i;
 MESSER_DEBUG("the first times is off @ <%s, %s>");
 }
 else if (i == (m - 1))
 {
 if (last_off == m) //last is session-off
 {
 j++;
 onOff.push_back(min_on_time);
 }
 else // base last case
 {
 j++;
 onOff.push_back(
 arrival_time[i] - arrival_time[last_off]);
 }
 }
 else
 {
 if (j == 0) // base first case
 {
 j++;
 onOff.push_back(arrival_time[i - 1]);
 j++;
 onOff.push_back(delta_time[i]);
 last_off = i;
 }
 else // base case
 {
 j++;
 timebuffer = arrival_time[i - 1]
 - arrival_time[last_off];
 if (timebuffer < min_on_time)
 {
 onOff.push_back(min_on_time);
 }
 else
 {
 onOff.push_back(timebuffer);
 }
 j++;
 onOff.push_back(delta_time[i]);
 last_off = i;
 }

 }

 }
 }
 }

 MESSER_DEBUG("onOff.size() = %d @<%s, %s>", onOff.size());
 m = onOff.size();
 if (m == 0)
 {
 cerr << "Somenthing went wrong, onOff.size() is zero.";
 MESSER_FATAL("Somenthing went wrong, onOff.size() is zero. @ <%s, %s>");
 }

 i = 0;
 for (list<time_sec>::iterator it = onOff.begin(); it != onOff.end(); it++)
 {
 i++;
 if ((i % 2) == 1)
 {
 onTimes->push_back(*it);
 }
 else
 {
 offTimes->push_back(*it);
 }
 }

 MESSER_DEBUG("onTimes->size() = %d @<%s, %s>", onTimes->size());
 MESSER_DEBUG("offTimes->size() = %d @<%s, %s>", offTimes->size());
 delete_cvector(arrival_time);
 delete_cvector(delta_time);
 }
 */

void DataProcessor::calcOnOff(list<time_sec>& deltaVet, const time_sec cut_time,
		const time_sec min_on_time, vector<time_sec>* onTimes,
		vector<time_sec>* offTimes)
{
// log
//MESSER_LOG_INIT(ERROR);
//MESSER_DEBUG("<%s, %s>");

// data structures
	unsigned int m = deltaVet.size();
	list<time_sec> arrivalVet;
	cumulativeDistribution(deltaVet, &arrivalVet);
	time_sec* arrival_time = list_to_cvector(&arrivalVet);
	time_sec* delta_time = list_to_cvector(&deltaVet);
	list<time_sec> onOff;

// vars inity
	time_sec timebuffer = 0;
	unsigned int last_off = 0;
	unsigned int i = 0;
	unsigned int j = 0;

	if (m == 0)
	{
		onOff.push_back(min_on_time);
	}
	else
	{
		for (i = 0; i < m; i++)
		{
			if (delta_time[i] > cut_time)
			{
				if (i == 0) // the first times is off
				{
					j++;
					onOff.push_back(min_on_time);
					j++;
					onOff.push_back(delta_time[i]);
					last_off = i;
					//MESSER_DEBUG("the first times is off @ <%s, %s>");
				}
				else
				{
					if (j == 0) // base first case
					{
						j++;
						onOff.push_back(arrival_time[i - 1]);
						j++;
						onOff.push_back(delta_time[i]);
						last_off = i;
					}
					else // base case
					{
						j++;
						timebuffer = arrival_time[i - 1]
								- arrival_time[last_off];
						if (timebuffer < min_on_time)
						{
							onOff.push_back(min_on_time);
						}
						else
						{
							onOff.push_back(timebuffer);
						}
						j++;
						onOff.push_back(delta_time[i]);
						last_off = i;
					}

				}

			}
		}

		if (last_off == (m - 1)) //last is session-off
		{
			j++;
			onOff.push_back(min_on_time);
		}
		else // base last case
		{
			j++;
			if (last_off != 0)
			{
				onOff.push_back(arrival_time[m - 1] - arrival_time[last_off]);
			}
			else
			{
				onOff.push_back(arrival_time[m - 1]);
			}

		}

	}

//DEBUG
	char charList[CHAR_LARGE_BUFFER];
	list2str(onOff, charList);
//MESSER_DEBUG("onOff = %s @<%s, %s>", charList);
//RegressionTests rt;
//rt.wait_int();

//MESSER_DEBUG("onOff.size() = %d @<%s, %s>", onOff.size());
	m = onOff.size();
	if (m == 0)
	{
		cerr << "Somenthing went wrong, onOff.size() is zero.";
		//MESSER_FATAL("Somenthing went wrong, onOff.size() is zero. @ <%s, %s>");
	}

	i = 0;
	for (list<time_sec>::iterator it = onOff.begin(); it != onOff.end(); it++)
	{
		i++;
		if ((i % 2) == 1)
		{
			onTimes->push_back(*it);
		}
		else
		{
			offTimes->push_back(*it);
		}
	}

//MESSER_DEBUG("onTimes->size() = %d @<%s, %s>", onTimes->size());
//MESSER_DEBUG("offTimes->size() = %d @<%s, %s>", offTimes->size());
	delete_cvector(arrival_time);
	delete_cvector(delta_time);
}

void DataProcessor::setSessionOnOffTimes(list<time_sec>& interArrivalTimes)
{

}

void DataProcessor::calcOnOff(list<time_sec>& deltaVet,
		list<packet_size>& psList, const time_sec cut_time,
		const time_sec min_on_time, vector<time_sec>* onTimes,
		vector<time_sec>* offTimes, vector<unsigned int>* pktCounter,
		vector<double>* fileSize)
{
// log
//MESSER_LOG_INIT(ERROR);
//MESSER_DEBUG("<%s, %s>");

// data structures
	unsigned int m = deltaVet.size();
	list<time_sec> arrivalVet;
	cumulativeDistribution(deltaVet, &arrivalVet);
	time_sec* arrival_time = list_to_cvector(&arrivalVet);
	time_sec* delta_time = list_to_cvector(&deltaVet);
	list<time_sec> onOff;

// vars inity
	unsigned int pktCounterSum = 0;
	double fileSizeSum = 0;
	time_sec timebuffer = 0;
	unsigned int last_off = 0;
	unsigned int i = 0;
	unsigned int j = 0;
	list<packet_size>::iterator psList_it = psList.begin();

	if (m == 0)
	{
		pktCounterSum++;
		fileSizeSum = *psList_it;
		onOff.push_back(min_on_time);
		pktCounter->push_back(pktCounterSum);
		fileSize->push_back(fileSizeSum);
	}
	else
	{
		for (i = 0; i < m; i++)
		{
			pktCounterSum++;
			fileSizeSum = fileSizeSum + *psList_it;
			psList_it++;
			if (delta_time[i] > cut_time)
			{
				if (i == 0) // the first times is off
				{
					j++;
					onOff.push_back(min_on_time);
					j++;
					onOff.push_back(delta_time[i]);
					last_off = i;
					// packet counter and file size
					pktCounter->push_back(pktCounterSum);
					fileSize->push_back(fileSizeSum);
					pktCounterSum = 0;
					fileSizeSum = 0;
					//MESSER_DEBUG("the first times is off @ <%s, %s>");
				}
				else
				{
					if (j == 0) // base first case
					{
						j++;
						onOff.push_back(arrival_time[i - 1]);
						j++;
						onOff.push_back(delta_time[i]);
						last_off = i;
						// packet counter and file size
						pktCounter->push_back(pktCounterSum);
						fileSize->push_back(fileSizeSum);
						pktCounterSum = 0;
						fileSizeSum = 0;
					}
					else // base case
					{
						j++;
						timebuffer = arrival_time[i - 1]
								- arrival_time[last_off];
						if (timebuffer < min_on_time)
						{
							onOff.push_back(min_on_time);
						}
						else
						{
							onOff.push_back(timebuffer);
						}
						j++;
						onOff.push_back(delta_time[i]);
						last_off = i;
						// packet counter and file size
						pktCounter->push_back(pktCounterSum);
						fileSize->push_back(fileSizeSum);
						pktCounterSum = 0;
						fileSizeSum = 0;
					}

				}

			}
		}

		pktCounterSum++;
		fileSizeSum = fileSizeSum + *psList_it;
		pktCounter->push_back(pktCounterSum);
		fileSize->push_back(fileSizeSum);
		if (last_off == (m - 1)) //last is session-off
		{
			j++;
			onOff.push_back(min_on_time);
		}
		else // base last case
		{
			j++;
			if (last_off != 0)
			{
				onOff.push_back(arrival_time[m - 1] - arrival_time[last_off]);
			}
			else
			{
				onOff.push_back(arrival_time[m - 1]);
			}

		}

	}

//DEBUG
	char charList[CHAR_LARGE_BUFFER];
	list2str(onOff, charList);
//MESSER_DEBUG("onOff = %s @<%s, %s>", charList);
//RegressionTests rt;
//rt.wait_int();

//MESSER_DEBUG("onOff.size() = %d @<%s, %s>", onOff.size());
	m = onOff.size();
	if (m == 0)
	{
		cerr << "Somenthing went wrong, onOff.size() is zero.";
		//MESSER_FATAL("Somenthing went wrong, onOff.size() is zero. @ <%s, %s>");
	}

	i = 0;
	for (list<time_sec>::iterator it = onOff.begin(); it != onOff.end(); it++)
	{
		i++;
		if ((i % 2) == 1)
		{
			onTimes->push_back(*it);
		}
		else
		{
			offTimes->push_back(*it);
		}
	}

//MESSER_DEBUG("onTimes->size() = %d @<%s, %s>", onTimes->size());
//MESSER_DEBUG("offTimes->size() = %d @<%s, %s>", offTimes->size());
	delete_cvector(arrival_time);
	delete_cvector(delta_time);
}

void DataProcessor::calcOnOff(list<time_sec>& deltaVet,
		list<unsigned int>& psList, const time_sec cut_time,
		const time_sec min_on_time, vector<time_sec>* onTimes,
		vector<time_sec>* offTimes, vector<unsigned int>* pktCounter,
		vector<unsigned int>* fileSize)
{
// log
//MESSER_LOG_INIT(ERROR);
//MESSER_DEBUG("<%s, %s>");

// data structures
	unsigned int m = deltaVet.size();
	list<time_sec> arrivalVet;
	cumulativeDistribution(deltaVet, &arrivalVet);
	time_sec* arrival_time = list_to_cvector(&arrivalVet);
	time_sec* delta_time = list_to_cvector(&deltaVet);
	list<time_sec> onOff;

// vars inity
	unsigned int pktCounterSum = 0;
	double fileSizeSum = 0;
	time_sec timebuffer = 0;
	unsigned int last_off = 0;
	unsigned int i = 0;
	unsigned int j = 0;
	list<unsigned int>::iterator psList_it = psList.begin();

	if (m == 0)
	{
		pktCounterSum++;
		fileSizeSum = *psList_it;
		onOff.push_back(min_on_time);
		pktCounter->push_back(pktCounterSum);
		fileSize->push_back(fileSizeSum);
	}
	else
	{
		for (i = 0; i < m; i++)
		{
			pktCounterSum++;
			fileSizeSum = fileSizeSum + *psList_it;
			psList_it++;
			if (delta_time[i] > cut_time)
			{
				if (i == 0) // the first times is off
				{
					j++;
					onOff.push_back(min_on_time);
					j++;
					onOff.push_back(delta_time[i]);
					last_off = i;
					// packet counter and file size
					pktCounter->push_back(pktCounterSum);
					fileSize->push_back(fileSizeSum);
					pktCounterSum = 0;
					fileSizeSum = 0;
					//MESSER_DEBUG("the first times is off @ <%s, %s>");
				}
				else
				{
					if (j == 0) // base first case
					{
						j++;
						onOff.push_back(arrival_time[i - 1]);
						j++;
						onOff.push_back(delta_time[i]);
						last_off = i;
						// packet counter and file size
						pktCounter->push_back(pktCounterSum);
						fileSize->push_back(fileSizeSum);
						pktCounterSum = 0;
						fileSizeSum = 0;
					}
					else // base case
					{
						j++;
						timebuffer = arrival_time[i - 1]
								- arrival_time[last_off];
						if (timebuffer < min_on_time)
						{
							onOff.push_back(min_on_time);
						}
						else
						{
							onOff.push_back(timebuffer);
						}
						j++;
						onOff.push_back(delta_time[i]);
						last_off = i;
						// packet counter and file size
						pktCounter->push_back(pktCounterSum);
						fileSize->push_back(fileSizeSum);
						pktCounterSum = 0;
						fileSizeSum = 0;
					}

				}

			}
		}

		pktCounterSum++;
		fileSizeSum = fileSizeSum + *psList_it;
		pktCounter->push_back(pktCounterSum);
		fileSize->push_back(fileSizeSum);
		if (last_off == (m - 1)) //last is session-off
		{
			j++;
			onOff.push_back(min_on_time);
		}
		else // base last case
		{
			j++;
			if (last_off != 0)
			{
				onOff.push_back(arrival_time[m - 1] - arrival_time[last_off]);
			}
			else
			{
				onOff.push_back(arrival_time[m - 1]);
			}

		}

	}

//DEBUG
	char charList[CHAR_LARGE_BUFFER];
	list2str(onOff, charList);
//MESSER_DEBUG("onOff = %s @<%s, %s>", charList);
//RegressionTests rt;
//rt.wait_int();

//MESSER_DEBUG("onOff.size() = %d @<%s, %s>", onOff.size());
	m = onOff.size();
	if (m == 0)
	{
		cerr << "Somenthing went wrong, onOff.size() is zero.";
		//MESSER_FATAL("Somenthing went wrong, onOff.size() is zero. @ <%s, %s>");
	}

	i = 0;
	for (list<time_sec>::iterator it = onOff.begin(); it != onOff.end(); it++)
	{
		i++;
		if ((i % 2) == 1)
		{
			onTimes->push_back(*it);
		}
		else
		{
			offTimes->push_back(*it);
		}
	}

//MESSER_DEBUG("onTimes->size() = %d @<%s, %s>", onTimes->size());
//MESSER_DEBUG("offTimes->size() = %d @<%s, %s>", offTimes->size());
	delete_cvector(arrival_time);
	delete_cvector(delta_time);
}

bool DataProcessor::test_calcOnOff()
{
//MESSER_LOG_INIT(DEBUG);
	SimitarWorkspace workspace = SimitarWorkspace();
	char exp_interarrival_times[CHAR_BUFFER];
	strcpy(exp_interarrival_times, workspace.test_exponential_times().c_str());
	time_sec min_on_time = 0.1;
	double accErr = 0.01;

////////////////////////////////////////////////////////////////////////////
/// Trivial test
////////////////////////////////////////////////////////////////////////////

	time_sec session_cut_time1 = 2.9;
	vec delta_sample_1 =
	{ 0.1, 3, 2, 0.2, 5, 1.1, 0.5, 4, 0.9 };
	vec test1_expected_on =
	{ 0.10000, 2.20000, 1.60000, 0.90000 };
	vec test1_expected_off =
	{ 3.00000, 5.00000, 4.00000 };

	unsigned int size_deltaSample1 = delta_sample_1.size();
	vector<time_sec> onTimes;
	vector<time_sec> offTimes;
	list<time_sec> list_deltaSample1;
	unsigned int i = 0;
	for (i = 0; i < size_deltaSample1; i++)
	{
		list_deltaSample1.push_back(delta_sample_1(i));
	}
	calcOnOff(list_deltaSample1, session_cut_time1, min_on_time, &onTimes,
			&offTimes);

	for (i = 0; i < test1_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test1_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Trivial test) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test1_expected_off(i));

			return (false);
		}
	}

	for (i = 0; i < test1_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test1_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"%dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test1_expected_on(i));
			return (false);
		}
	}

///////////////////////////////////////////////////////////////////////////
/// 10000 ramdom generated entries, exponentially distributed
///////////////////////////////////////////////////////////////////////////

	onTimes.clear();
	offTimes.clear();
	time_sec session_cut_time2 = 7;
	vec delta_sample_2;
//delta_sample_2.load("data/regression-tests/exp_interarrival_times.txt");
	delta_sample_2.load(exp_interarrival_times);
	vec test2_expected_on =
	{ 801.89, 896.71, 764.22, 398.49, 2823.59, 213.07, 1034.47, 2972.63 };
	vec test2_expected_off =
	{ 7.3502, 7.3181, 7.2477, 10.3901, 7.3315, 8.9811, 7.5889 };
	unsigned int size_deltaSample2 = delta_sample_2.size();
	list<time_sec> list_deltaSample2;
	i = 0;
	for (i = 0; i < size_deltaSample2; i++)
	{
		list_deltaSample2.push_back(delta_sample_2(i));
	}

	calcOnOff(list_deltaSample2, session_cut_time2, min_on_time, &onTimes,
			&offTimes);

	for (i = 0; i < test2_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test2_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test2_expected_off(i));
			return (false);
		}
	}

	for (i = 0; i < test2_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test2_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test2_expected_on(i));
			return (false);
		}
	}

///////////////////////////////////////////////////////////////////////////
/// Bord case 1
///////////////////////////////////////////////////////////////////////////
	onTimes.clear();
	offTimes.clear();
	time_sec session_cut_time3 = 7;
	vec delta_sample_3 =
	{ 30.00551800, 30.00527800, 30.00600200, 30.00594800, 30.00571400 };
	vec test3_expected_on =
	{ 0.10000, 0.10000, 0.10000, 0.10000, 0.10000, 0.10000 };
	vec test3_expected_off =
	{ 30.006, 30.005, 30.006, 30.006, 30.006 };
	unsigned int size_deltaSample3 = delta_sample_3.size();
	list<time_sec> list_deltaSample3;
	i = 0;
	for (i = 0; i < size_deltaSample3; i++)
	{
		list_deltaSample3.push_back(delta_sample_3(i));
	}

	calcOnOff(list_deltaSample3, session_cut_time3, min_on_time, &onTimes,
			&offTimes);

	for (i = 0; i < test3_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test3_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Bord case 1) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test3_expected_off(i));
			return (false);
		}
	}

	for (i = 0; i < test3_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test3_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Bord case 1) %dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test3_expected_on(i));
			return (false);
		}
	}

///////////////////////////////////////////////////////////////////////////
/// Bord case 2
///////////////////////////////////////////////////////////////////////////
	onTimes.clear();
	offTimes.clear();
	time_sec session_cut_time4 = 7;
	vec delta_sample_4 =
	{ 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1,
			0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };
	vec test4_expected_on =
	{ 2.1000 };
	vec test4_expected_off =
	{ };
	unsigned int size_deltaSample4 = delta_sample_4.size();
	list<time_sec> list_deltaSample4;
	i = 0;
	for (i = 0; i < size_deltaSample4; i++)
	{
		list_deltaSample4.push_back(delta_sample_4(i));
	}

	calcOnOff(list_deltaSample4, session_cut_time4, min_on_time, &onTimes,
			&offTimes);

	for (i = 0; i < test4_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test4_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Bord case 2) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test4_expected_off(i));
			return (false);
		}
	}

	for (i = 0; i < test4_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test4_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Bord case 2) %dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test4_expected_on(i));
			return (false);
		}
	}

//RegressionTests rt;
//rt.wait_int();

	return (true);
}

bool DataProcessor::test_calcOnOff2()
{
	SimitarWorkspace workspace = SimitarWorkspace();
	char exp_interarrival_times[CHAR_BUFFER];
	strcpy(exp_interarrival_times, workspace.test_exponential_times().c_str());
//MESSER_LOG_INIT(DEBUG);
	time_sec min_on_time = 0.1;
	double accErr = 0.01;

////////////////////////////////////////////////////////////////////////////
/// Trivial test
////////////////////////////////////////////////////////////////////////////

	time_sec session_cut_time1 = 2.9;
	vec delta_sample_1 =
	{ 0.1, 3, 2, 0.2, 5, 1.1, 0.5, 4, 0.9 };
	vec psList_sample_1 = ones(delta_sample_1.size() + 1, 1) * 3;
	vec test1_expected_on =
	{ 0.10000, 2.20000, 1.60000, 0.90000 };
	vec test1_expected_off =
	{ 3.00000, 5.00000, 4.00000 };
	vec test1_expected_conter =
	{ 2, 3, 3, 2 };
	vec test1_expected_fsize =
	{ 6, 9, 9, 6 };

	unsigned int size_deltaSample1 = delta_sample_1.size();
	unsigned int size_psSample1 = psList_sample_1.size();
	list<time_sec> list_deltaSample1;
	list<time_sec> list_psListSample1;
	vector<time_sec> onTimes;
	vector<time_sec> offTimes;
	vector<unsigned int> pktCounter;
	vector<double> fileSize;
	unsigned int i = 0;
	for (i = 0; i < size_deltaSample1; i++)
	{
		list_deltaSample1.push_back(delta_sample_1(i));
	}
	i = 0;
	for (i = 0; i < size_psSample1; i++)
	{
		list_psListSample1.push_back(psList_sample_1(i));
	}
	calcOnOff(list_deltaSample1, list_psListSample1, session_cut_time1,
			min_on_time, &onTimes, &offTimes, &pktCounter, &fileSize);

	for (i = 0; i < test1_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test1_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Trivial test) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test1_expected_off(i));

			return (false);
		}
	}

	for (i = 0; i < test1_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test1_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Trivial test) %dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test1_expected_on(i));
			return (false);
		}
	}

	for (i = 0; i < test1_expected_fsize.size(); i++)
	{

		if (!compareDouble(fileSize[i], test1_expected_fsize(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Trivial test) %dth file size do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, fileSize[i], test1_expected_fsize(i));

			return (false);
		}
	}

	for (i = 0; i < test1_expected_conter.size(); i++)
	{

		if (!compareDouble(pktCounter[i], test1_expected_conter(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Trivial test) %dth pktCounter do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, pktCounter[i], test1_expected_conter(i));

			return (false);
		}
	}

///////////////////////////////////////////////////////////////////////////
/// 10000 ramdom generated entries, exponentially distributed
///////////////////////////////////////////////////////////////////////////

	pktCounter.clear();
	fileSize.clear();
	onTimes.clear();
	offTimes.clear();
	time_sec session_cut_time2 = 7;
	vec delta_sample_2;
//delta_sample_2.load("data/regression-tests/exp_interarrival_times.txt");
	delta_sample_2.load(exp_interarrival_times);
	vec psList_sample2 = ones(delta_sample_2.size() + 1, 1) * 3;
	vec test2_expected_on =
	{ 801.89, 896.71, 764.22, 398.49, 2823.59, 213.07, 1034.47, 2972.63 };
	vec test2_expected_off =
	{ 7.3502, 7.3181, 7.2477, 10.3901, 7.3315, 8.9811, 7.5889 };
	vec test2_expected_conter =
	{ 814, 930, 787, 389, 2872, 222, 1005, 2982 };
	vec test2_expected_fsize =
	{ 2442, 2790, 2361, 1167, 8616, 666, 3015, 8946 };
	unsigned int size_deltaSample2 = delta_sample_2.size();
	unsigned int size_psSample2 = psList_sample2.size();
	list<time_sec> list_deltaSample2;
	list<time_sec> list_psListSample2;
	i = 0;
	for (i = 0; i < size_deltaSample2; i++)
	{
		list_deltaSample2.push_back(delta_sample_2(i));
	}
	i = 0;
	for (i = 0; i < size_psSample2; i++)
	{
		list_psListSample2.push_back(psList_sample2(i));
	}

	calcOnOff(list_deltaSample2, list_psListSample2, session_cut_time2,
			min_on_time, &onTimes, &offTimes, &pktCounter, &fileSize);
	for (i = 0; i < test2_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test2_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test1_expected_off(i));

			return (false);
		}
	}

	for (i = 0; i < test2_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test2_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test2_expected_on(i));
			return (false);
		}
	}

	for (i = 0; i < test2_expected_fsize.size(); i++)
	{

		if (!compareDouble(fileSize[i], test2_expected_fsize(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth file size do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, fileSize[i], test2_expected_fsize(i));

			return (false);
		}
	}

	for (i = 0; i < test2_expected_conter.size(); i++)
	{

		if (!compareDouble(pktCounter[i], test2_expected_conter(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth pktCounter do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, pktCounter[i], test2_expected_conter(i));

			return (false);
		}
	}

///////////////////////////////////////////////////////////////////////////
/// Bord case 1
///////////////////////////////////////////////////////////////////////////
	pktCounter.clear();
	fileSize.clear();
	onTimes.clear();
	offTimes.clear();
	time_sec session_cut_time3 = 7;
	vec delta_sample_3 =
	{ 30.00551800, 30.00527800, 30.00600200, 30.00594800, 30.00571400 };
	vec psList_sample3 = ones(delta_sample_3.size() + 1, 1) * 3;
	vec test3_expected_on =
	{ 0.10000, 0.10000, 0.10000, 0.10000, 0.10000, 0.10000 };
	vec test3_expected_off =
	{ 30.006, 30.005, 30.006, 30.006, 30.006 };
	vec test3_expected_conter =
	{ 1, 1, 1, 1, 1, 1 };
	vec test3_expected_fsize =
	{ 3, 3, 3, 3, 3, 3 };
	unsigned int size_deltaSample3 = delta_sample_3.size();
	unsigned int size_psSample3 = psList_sample3.size();
	list<time_sec> list_deltaSample3;
	list<time_sec> list_psListSample3;
	i = 0;
	for (i = 0; i < size_deltaSample3; i++)
	{
		list_deltaSample3.push_back(delta_sample_3(i));
	}
	i = 0;
	for (i = 0; i < size_psSample3; i++)
	{
		list_psListSample3.push_back(psList_sample3(i));
	}

	calcOnOff(list_deltaSample3, list_psListSample3, session_cut_time3,
			min_on_time, &onTimes, &offTimes, &pktCounter, &fileSize);
	for (i = 0; i < test3_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test3_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test3_expected_off(i));

			return (false);
		}
	}

	for (i = 0; i < test3_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test3_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test3_expected_on(i));
			return (false);
		}
	}

	for (i = 0; i < test3_expected_fsize.size(); i++)
	{

		if (!compareDouble(fileSize[i], test3_expected_fsize(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth file size do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, fileSize[i], test3_expected_fsize(i));

			return (false);
		}
	}

	for (i = 0; i < test3_expected_conter.size(); i++)
	{

		if (!compareDouble(pktCounter[i], test3_expected_conter(i), accErr))
		{
			//MESSER_ERROR(
			//		"(10000 ramdom entries) %dth pktCounter do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, pktCounter[i], test3_expected_conter(i));

			return (false);
		}
	}

///////////////////////////////////////////////////////////////////////////
/// Bord case 2
///////////////////////////////////////////////////////////////////////////
	onTimes.clear();
	offTimes.clear();
	time_sec session_cut_time4 = 7;
	vec delta_sample_4 =
	{ 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1,
			0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };
	vec test4_expected_on =
	{ 2.1000 };
	vec test4_expected_off =
	{ };
	unsigned int size_deltaSample4 = delta_sample_4.size();
	list<time_sec> list_deltaSample4;
	i = 0;
	for (i = 0; i < size_deltaSample4; i++)
	{
		list_deltaSample4.push_back(delta_sample_4(i));
	}

	calcOnOff(list_deltaSample4, session_cut_time4, min_on_time, &onTimes,
			&offTimes);

	for (i = 0; i < test4_expected_off.size(); i++)
	{

		if (!compareDouble(offTimes[i], test4_expected_off(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Bord case 2) %dth Off time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, offTimes[i], test4_expected_off(i));
			return (false);
		}
	}

	for (i = 0; i < test4_expected_on.size(); i++)
	{

		if (!compareDouble(onTimes[i], test4_expected_on(i), accErr))
		{
			//MESSER_ERROR(
			//		"(Bord case 2) %dth On time do not match\tResult::Expected = %f::%f\t<%s, %s>",
			//		i, onTimes[i], test4_expected_on(i));
			return (false);
		}
	}

//RegressionTests rt;
//rt.wait_int();

	return (true);
}

