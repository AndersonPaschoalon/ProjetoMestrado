/*
 * NetworkTrace.cpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: apaschoalon
 */

#include "NetworkTrace.h"

#ifndef CDT_V2

const char * NetworkTrace::LABEL_TRACE = "trace";
const char * NetworkTrace::LABEL_TRACE_NAME = "info_tracename";
const char * NetworkTrace::LABEL_CAPTURE_DATE = "info_captureDate";
const char * NetworkTrace::LABEL_COMMENTARIES = "info_commentaries";
const char * NetworkTrace::LABEL_TRAFFIC_ENGINE = "trafficGenEngine";
const char * NetworkTrace::LABEL_NUMBER_OF_FLOWS = "n_flows";
const char * NetworkTrace::LABEL_FLOW = "flow";
const char * NetworkTrace::LABEL_FLOW_ID = "flow_id";
const char * NetworkTrace::LABEL_FLOW_START_DELAY = "start_delay";
const char * NetworkTrace::LABEL_FLOW_DURATION = "duration";
const char * NetworkTrace::LABEL_FLOW_DS_BYTE = "ds_byte";
const char * NetworkTrace::LABEL_FLOW_NUMBER_KBYTES = "n_kbytes";
const char * NetworkTrace::LABEL_FLOW_NUMBER_PACKETS = "n_packets";
const char * NetworkTrace::LABEL_LINK_LAYER = "link_layer";
const char * NetworkTrace::LABEL_MAC_SRC = "mac_src";
const char * NetworkTrace::LABEL_MAC_DST = "mac_dst";
const char * NetworkTrace::LABEL_NETWORK_LAYER = "network_layer";
const char * NetworkTrace::LABEL_TTL = "ttl";
const char * NetworkTrace::LABEL_SRC_IP = "src_ip";
const char * NetworkTrace::LABEL_DST_IP = "dst_ip";
const char * NetworkTrace::LABEL_TRANSPORT_LAYER = "transport_layer";
const char * NetworkTrace::LABEL_SRCPORT = "src_port";
const char * NetworkTrace::LABEL_DSTPORT = "dst_port";
const char * NetworkTrace::LABEL_SCTP_ASSOCIATION_ID = "association_id";
const char * NetworkTrace::LABEL_SCTP_MAX_STREAMS = "max_streams";
const char * NetworkTrace::LABEL_APPLICATION_LAYER = "application_layer";
const char * NetworkTrace::LABEL_INTERPACKETTIMES = "inter_packet_times";
const char * NetworkTrace::LABEL_INTERFILE = "inter_file_times";
const char * NetworkTrace::LABEL_INTERSESSION = "inter_session_times";
const char * NetworkTrace::LABEL_SESSION = "session_times";
const char * NetworkTrace::LABEL_SESSION_ON = "on_times";
const char * NetworkTrace::LABEL_SESSION_OFF = "off_times";
const char * NetworkTrace::LABEL_SESSION_NPACKETS = "n_packets";
const char * NetworkTrace::LABEL_SESSION_NBYTES = "n_bytes";
const char * NetworkTrace::LABEL_PACKETSIZES = "packet_sizes";
const char * NetworkTrace::LABEL_PSMODE1 = "ps_mode1";
const char * NetworkTrace::LABEL_PSMODE2 = "ps_mode2";
const char * NetworkTrace::LABEL_STOCHASTIC_MODEL = "stochastic_model";
const char * NetworkTrace::LABEL_STOCHASTIC_MODEL_NAME = "name";
const char * NetworkTrace::LABEL_PARAM1 = "param1";
const char * NetworkTrace::LABEL_PARAM2 = "param2";
const char * NetworkTrace::LABEL_AIC = "aic";
const char * NetworkTrace::LABEL_BIC = "bic";
const char * NetworkTrace::LABEL_NPACKETS = "n_packets";
const char * NetworkTrace::LABEL_NKBYTES = "n_kbytes";

#else // new version CDT_V2

// Trace Object
const char * NetworkTrace::LABEL_TRACE = "TRACE";
const char * NetworkTrace::LABEL_TRACE_NAME = "trace_name ";
const char * NetworkTrace::LABEL_CAPTURE_DATE = "capture_date";
const char * NetworkTrace::LABEL_COMMENTARIES = "comment";
const char * NetworkTrace::LABEL_TRAFFIC_ENGINE = "tg"; // TODO - DEPRECATED
const char * NetworkTrace::LABEL_NUMBER_OF_FLOWS = "n_flows";

// Flow Object
// - tag flow
const char * NetworkTrace::LABEL_FLOW = "F";
const char * NetworkTrace::LABEL_FLOW_ID = "id";
const char * NetworkTrace::LABEL_FLOW_START_DELAY = "s";
const char * NetworkTrace::LABEL_FLOW_DURATION = "d";
const char * NetworkTrace::LABEL_FLOW_DS_BYTE = "b"; // TODO - DEPRECATED
const char * NetworkTrace::LABEL_FLOW_NUMBER_KBYTES = "k";
const char * NetworkTrace::LABEL_FLOW_NUMBER_PACKETS = "n";
// - tag link_layer
const char * NetworkTrace::LABEL_LINK_LAYER = "L";
const char * NetworkTrace::LABEL_MAC_SRC = "s";
const char * NetworkTrace::LABEL_MAC_DST = "d";
// - tag network_layer
const char * NetworkTrace::LABEL_NETWORK_LAYER = "N";
const char * NetworkTrace::LABEL_TTL = "t";
const char * NetworkTrace::LABEL_SRC_IP = "s";
const char * NetworkTrace::LABEL_DST_IP = "d";
// - tag transport-layer
const char * NetworkTrace::LABEL_TRANSPORT_LAYER = "T";
const char * NetworkTrace::LABEL_SRCPORT = "s";
const char * NetworkTrace::LABEL_DSTPORT = "d";
const char * NetworkTrace::LABEL_SCTP_ASSOCIATION_ID = "i"; // TODO DEPRECATED
const char * NetworkTrace::LABEL_SCTP_MAX_STREAMS = "m";    // TODO DEPRECATED
// - tag application_layer
const char * NetworkTrace::LABEL_APPLICATION_LAYER = "A";
// - tag inter_packet_times
const char * NetworkTrace::LABEL_INTERPACKETTIMES = "I";
// - tag session_times
const char * NetworkTrace::LABEL_SESSION = "S";
const char * NetworkTrace::LABEL_SESSION_ON = "o";
const char * NetworkTrace::LABEL_SESSION_OFF = "f";
const char * NetworkTrace::LABEL_SESSION_NPACKETS = "n";
const char * NetworkTrace::LABEL_SESSION_NBYTES = "b";
// - tag packet_sizes/ps_mode1/ps_mode2
const char * NetworkTrace::LABEL_PACKETSIZES = "P";
const char * NetworkTrace::LABEL_PSMODE1 = "Q";
const char * NetworkTrace::LABEL_PSMODE2 = "R";
const char * NetworkTrace::LABEL_NPACKETS = "p";
const char * NetworkTrace::LABEL_NKBYTES = "q";
// - tag stochastic_model
const char * NetworkTrace::LABEL_STOCHASTIC_MODEL = "M";
const char * NetworkTrace::LABEL_STOCHASTIC_MODEL_NAME = "n";
const char * NetworkTrace::LABEL_PARAM1 = "p";
const char * NetworkTrace::LABEL_PARAM2 = "q";
const char * NetworkTrace::LABEL_AIC = "a";
const char * NetworkTrace::LABEL_BIC = "b";

// TODO Tiny Flow Object
const char * NetworkTrace::TAG_TINY_FLOW = "Y";
const char * NetworkTrace::PROP_TINYFLOW_PROTOCOL_LINK_LAYER = "l";
const char * NetworkTrace::PROP_TINYFLOW_PROTOCOL_NETWORK_LAYER = "n";
const char * NetworkTrace::PROP_TINYFLOW_NETWORK_ADDRESS_DESTINATION = "d";
const char * NetworkTrace::PROP_TINYFLOW_NETWORK_ADDRESS_SOURCE = "s";
const char * NetworkTrace::PROP_TINYFLOW_PROTOCOL_TRANSPORT = "t";
const char * NetworkTrace::PROP_TINYFLOW_THROUGHPUT = "h";
const char * NetworkTrace::PROP_TINYFLOW_NUMBER_OF_PACKETS_MODE1 = "m1";
const char * NetworkTrace::PROP_TINYFLOW_NUMBER_OF_PACKETS_MODE2 = "m2";
const char * NetworkTrace::PROP_TINYFLOW_AVERAGE_PACKET_SIZE_MODE1 = "p";
const char * NetworkTrace::PROP_TINYFLOW_AVERAGE_PACKET_SIZE_MODE2 = "q";

#endif


NetworkTrace::NetworkTrace()
{
	PLOG_DEBUG << "NetworkTrace() constructor default";
}

NetworkTrace::NetworkTrace(const string& fileName,
		const string& trafficGenEngine)
{
	PLOG_DEBUG << "NetworkTrace() file constructor;  args fileName:" << fileName
						<< ", trafficGenEngine:" << trafficGenEngine;
	// init vars
	long int nflows = 0;
	string strBuffer;
	unsigned int uintBuffer;
	unsigned long int lintBuffer;
	time_sec timeBuffer;
	port_number portBuffer = 0;
	Protocol pBuffer = Protocol();
	list<StochasticModelFit>* modelList = NULL;
	// init XML
	xml_document<> doc;
	xml_node<> * root_node;

	// Read the xml file into a vector
	std::ifstream theFile(fileName);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)),
			std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc
	doc.parse<0>(&buffer[0]);

	// Find our root node
	root_node = doc.first_node(LABEL_TRACE);

	info_tracename = root_node->first_attribute(LABEL_TRACE_NAME)->value();
	info_commentaries = root_node->first_attribute(LABEL_COMMENTARIES)->value();
	info_captureDate = root_node->first_attribute(LABEL_CAPTURE_DATE)->value();

	charvet2type(root_node->first_attribute(LABEL_NUMBER_OF_FLOWS)->value(), nflows);

	uint fcounter = 0;
	for (xml_node<> * flow_node = root_node->first_node(LABEL_FLOW); flow_node;
			flow_node = flow_node->next_sibling())
	{

		/// Create flow
		NetworkFlow* netFlow = NetworkFlowFactory::make_flow(trafficGenEngine);

		PLOG_VERBOSE << "Factoring flow[" << fcounter
							<< "]: NetworkFlowFactory::make_flow("
							<< trafficGenEngine << ")";
		netFlow->setFlowId(fcounter);
		fcounter++;

		/// Flow Settings
		charvet2type(flow_node->first_attribute(LABEL_FLOW_START_DELAY)->value(),
				timeBuffer);
		netFlow->setFlowStartDelay(timeBuffer);

		charvet2type(flow_node->first_attribute(LABEL_FLOW_DURATION)->value(),
				timeBuffer);
		netFlow->setFlowDuration(timeBuffer);

		charvet2type(flow_node->first_attribute(LABEL_FLOW_DS_BYTE)->value(),
				uintBuffer);
		netFlow->setFlowDsByte(uintBuffer);

		charvet2type(flow_node->first_attribute(LABEL_FLOW_NUMBER_KBYTES)->value(),
				lintBuffer);
		netFlow->setNumberOfKbytes(lintBuffer);

		charvet2type(flow_node->first_attribute(LABEL_FLOW_NUMBER_PACKETS)->value(),
				lintBuffer);
		netFlow->setNumberOfPackets(lintBuffer);

		// Link layer
		xml_node<> * link_node = flow_node->first_node(LABEL_LINK_LAYER);
		pBuffer = Protocol(link_node->value());
		netFlow->setLinkProtocol(pBuffer.get());
		netFlow->setMacAddr(link_node->first_attribute(LABEL_MAC_SRC)->value(),
				link_node->first_attribute(LABEL_MAC_DST)->value());

		// Network layer
		xml_node<> * network_node = flow_node->first_node(LABEL_NETWORK_LAYER);
		pBuffer = Protocol(network_node->value());
		netFlow->setNetworkProtocol(pBuffer.get());
		netFlow->setNetworkSrcAddr(
				network_node->first_attribute(LABEL_SRC_IP)->value());
		netFlow->setNetworkDstAddr(
				network_node->first_attribute(LABEL_DST_IP)->value());
		charvet2type(network_node->first_attribute(LABEL_TTL)->value(), uintBuffer);
		netFlow->setNetworkTtl(uintBuffer);

		// Transport layer
		xml_node<> * transport_node = flow_node->first_node(LABEL_TRANSPORT_LAYER);

		pBuffer = Protocol(transport_node->value());
		netFlow->setTransportProtocol(pBuffer.get());

		charvet2type(transport_node->first_attribute(LABEL_SRCPORT)->value(),
				portBuffer);
		netFlow->setTransportSrcPort(portBuffer);

		charvet2type(transport_node->first_attribute(LABEL_DSTPORT)->value(),
				portBuffer);
		netFlow->setTransportDstPort(portBuffer);

		// Application
		xml_node<> * application_node = flow_node->first_node(
				LABEL_APPLICATION_LAYER);
		pBuffer = Protocol(application_node->value());
		netFlow->setApplicationProtocol(pBuffer.get());

		// Inter packet times
		xml_node<> * interpacket_node = flow_node->first_node(
				LABEL_INTERPACKETTIMES);

		modelList = new list<StochasticModelFit>;
		for (xml_node<> * model_node = interpacket_node->first_node(
				LABEL_STOCHASTIC_MODEL); model_node;
				model_node = model_node->next_sibling())
		{
			double p1 = 0;
			double p2 = 0;
			double aic = 0;
			double bic = 0;
			charvet2type(model_node->first_attribute(LABEL_PARAM1)->value(), p1);
			charvet2type(model_node->first_attribute(LABEL_PARAM2)->value(), p2);
			charvet2type(model_node->first_attribute(LABEL_AIC)->value(), aic);
			charvet2type(model_node->first_attribute(LABEL_BIC)->value(), bic);
			modelList->push_back(
					StochasticModelFit(
							model_node->first_attribute(LABEL_STOCHASTIC_MODEL_NAME)->value(), p1,
							p2, aic, bic));
		}
		netFlow->setInterDepertureTimeModels(modelList);

		// Sessions On/Off
		xml_node<> * session_node = flow_node->first_node(LABEL_SESSION);
		vector<time_sec>* onTimes = new vector<time_sec>;
		vector<time_sec>* offTimes = new vector<time_sec>;

		vector<unsigned int>* pktCounter = new vector<unsigned int>;
		vector<unsigned int>* fileSize = new vector<unsigned int>;

		charvet2type(session_node->first_attribute(LABEL_SESSION_ON)->value(),
				*onTimes);
		charvet2type(session_node->first_attribute(LABEL_SESSION_OFF)->value(),
				*offTimes);

		charvet2type(
				session_node->first_attribute(LABEL_SESSION_NPACKETS)->value(),
				*pktCounter);
		charvet2type(
				session_node->first_attribute(LABEL_SESSION_NBYTES)->value(),
				*fileSize);
		netFlow->setSessionTimesOnOff(onTimes, offTimes, pktCounter, fileSize);

		// Packet sizes
		xml_node<> * packetsizes_nodes = flow_node->first_node("packet_sizes");

		charvet2type(packetsizes_nodes->first_attribute("n_packets")->value(),
				lintBuffer);
		netFlow->setNumberOfPackets(lintBuffer);

		charvet2type(packetsizes_nodes->first_attribute("n_kbytes")->value(),
				lintBuffer);
		netFlow->setNumberOfKbytes(lintBuffer);

		long int npacketsm1 = 0;
		long int nkbytesm1 = 0;
		long int npacketsm2 = 0;
		long int nkbytesm2 = 0;
		list<StochasticModelFit>* psm1 = new list<StochasticModelFit>;
		list<StochasticModelFit>* psm2 = new list<StochasticModelFit>;

		// Packet size, mode 1
		xml_node<> * psmode1_node = packetsizes_nodes->first_node("ps_mode1");
		charvet2type(psmode1_node->first_attribute("n_packets")->value(),
				npacketsm1);
		charvet2type(psmode1_node->first_attribute("n_kbytes")->value(),
				nkbytesm1);
		for (xml_node<> * model_node = psmode1_node->first_node(
				"stochastic_model"); model_node;
				model_node = model_node->next_sibling())
		{
			double p1 = 0;
			double p2 = 0;
			double aic = 0;
			double bic = 0;
			charvet2type(model_node->first_attribute(LABEL_PARAM1)->value(), p1);
			charvet2type(model_node->first_attribute(LABEL_PARAM2)->value(), p2);
			charvet2type(model_node->first_attribute(LABEL_AIC)->value(), aic);
			charvet2type(model_node->first_attribute(LABEL_BIC)->value(), bic);
			psm1->push_back(
					StochasticModelFit(
							model_node->first_attribute(LABEL_STOCHASTIC_MODEL_NAME)->value(), p1,
							p2, aic, bic));
		}

		// Packet size, mode 2
		xml_node<> * psmode2_node = packetsizes_nodes->first_node(LABEL_PSMODE2);
		charvet2type(psmode2_node->first_attribute(LABEL_NPACKETS)->value(),
				npacketsm2);
		charvet2type(psmode2_node->first_attribute(LABEL_NKBYTES)->value(),
				nkbytesm2);
		for (xml_node<> * model_node = psmode2_node->first_node(
				LABEL_STOCHASTIC_MODEL); model_node;
				model_node = model_node->next_sibling())
		{
			double p1 = 0;
			double p2 = 0;
			double aic = 0;
			double bic = 0;
			charvet2type(model_node->first_attribute(LABEL_PARAM1)->value(), p1);
			charvet2type(model_node->first_attribute(LABEL_PARAM2)->value(), p2);
			charvet2type(model_node->first_attribute(LABEL_AIC)->value(), aic);
			charvet2type(model_node->first_attribute(LABEL_BIC)->value(), bic);
			psm2->push_back(
					StochasticModelFit(
							model_node->first_attribute(LABEL_STOCHASTIC_MODEL_NAME)->value(), p1,
							p2, aic, bic));
		}

		netFlow->setPacketSizeModel(psm1, psm2, nkbytesm1, nkbytesm2,
				npacketsm1, npacketsm2);

		/// Store flow on trace
		pushback_Netflow(netFlow);
		//netFlow->print();
	}

	PLOG_DEBUG << "NetworkTrace.getNumberOfFlows() = " << getNumberOfFlows();

}

NetworkTrace::~NetworkTrace()
{
	PLOG_DEBUG << "Destructor ~NetworkTrace().networkFlow.size() = "
						<< networkFlow.size();

	for (unsigned int i = 0; i < networkFlow.size(); i++)
	{
		PLOG_VERBOSE << "delete networkFlow[" << i << "]";
		delete networkFlow[i];
	}
	networkFlow.clear();

}

const std::string& NetworkTrace::getInfoCaptureDate() const
{
	return info_captureDate;
}

void NetworkTrace::setInfoCaptureDate(const std::string& infoCaptureDate)
{
	info_captureDate = infoCaptureDate;
}

const std::string& NetworkTrace::getInfoCommentaries() const
{
	return info_commentaries;
}

void NetworkTrace::setInfoCommentaries(const std::string& infoCommentaries)
{
	info_commentaries = infoCommentaries;
}

const std::string& NetworkTrace::getInfoTracename() const
{
	return info_tracename;
}

void NetworkTrace::setInfoTracename(const std::string& infoTracename)
{
	info_tracename = infoTracename;
}

int NetworkTrace::writeToFile(void) const
{
	int returnFlag = 0;

	writeToFile(info_tracename);

	return returnFlag;
}

int NetworkTrace::writeToFile(const std::string& fileName) const
{
	PLOG_DEBUG << "writeToFile:" << fileName;
	unsigned int nPsM1Fittings = 3;
	unsigned int nPsM2Fittings = 3;
	unsigned int nFittings = 0;
	unsigned int nFlows = getNumberOfFlows();
	unsigned int i = 0;
	unsigned int j = 0;
	char tracename[CHAR_BUFFER] = "";
	char capdate[CHAR_BUFFER] = "";
	char commentaries[CHAR_BUFFER] = "";
	char charfileName[CHAR_BUFFER] = "";
	char numberFlows[CHAR_BUFFER] = "";
	flowData* fd = NULL;
	Protocol proto;
	StochasticModelFit sf;

	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	xml_node<>* trace = doc.allocate_node(node_element, LABEL_TRACE);

	string2charvet(info_tracename, tracename);
	trace->append_attribute(
			doc.allocate_attribute(LABEL_TRACE_NAME, tracename));

	string2charvet(info_captureDate, capdate);
	trace->append_attribute(
			doc.allocate_attribute(LABEL_CAPTURE_DATE, capdate));

	string2charvet(info_commentaries, commentaries);
	trace->append_attribute(
			doc.allocate_attribute(LABEL_COMMENTARIES, commentaries));

	sprintf(numberFlows, "%d", nFlows);
	trace->append_attribute(
			doc.allocate_attribute(LABEL_NUMBER_OF_FLOWS, numberFlows));

	doc.append_node(trace);

	// Allocate values
	fd = new flowData[nFlows];
	for (i = 0; i < nFlows; i++)
	{
		nFittings = networkFlow[i]->getNumberOfInterdepertureTimeModels();
		fd[i].interPkt = new modelData[nFittings];
		fd[i].n_interPktModels = nFittings;

		nPsM1Fittings = networkFlow[i]->getNumberOfPsMode1Models();
		nPsM2Fittings = networkFlow[i]->getNumberOfPsMode2Models();

		fd[i].psMode1 = new modelData[nPsM1Fittings];
		fd[i].psMode2 = new modelData[nPsM2Fittings];
		fd[i].n_psM1Models = nPsM1Fittings;
		fd[i].n_psM2Models = nPsM2Fittings;

	}

	for (i = 0; i < nFlows; i++)
	{
		//flow
		sprintf(fd[i].flow_start_delay, "%f",
				networkFlow[i]->getFlowStartDelay());
		sprintf(fd[i].flow_duration, "%f", networkFlow[i]->getFlowDuration());
		sprintf(fd[i].flow_ds_byte, "%d", networkFlow[i]->getFlowDsByte());
		sprintf(fd[i].flow_nkbytes, "%ld", networkFlow[i]->getNumberOfKbytes());
		sprintf(fd[i].flow_npackets, "%ld",
				networkFlow[i]->getNumberOfPackets());

		// Link layer
		Protocol(networkFlow[i]->getLinkProtocol()).get(fd[i].link_p);
		string2charvet(networkFlow[i]->getMacSrcAddr(), fd[i].mac_src);
		string2charvet(networkFlow[i]->getMacDstAddr(), fd[i].mac_dst);

		// Network layer
		Protocol(networkFlow[i]->getNetworkProtocol()).get(fd[i].net_p);
		string2charvet(networkFlow[i]->getNetworkSrcAddr(), fd[i].net_addrsrc);
		string2charvet(networkFlow[i]->getNetworkDstAddr(), fd[i].net_addrdst);
		sprintf(fd[i].net_ttl, "%d", networkFlow[i]->getNetworkTtl());

		// Transport layer
		Protocol(networkFlow[i]->getTransportProtocol()).get(fd[i].trans_p);
		sprintf(fd[i].trans_psrc, "%d", networkFlow[i]->getTransportSrcPort());
		sprintf(fd[i].trans_pdst, "%d", networkFlow[i]->getTransportDstPort());
		if (networkFlow[i]->getTransportProtocol() == PROTOCOL__SCTP)
		{
			sprintf(fd[i].trans_sctpid, "%d",
					networkFlow[i]->getTransportSctpAssociationId());
			sprintf(fd[i].trans_sctpmax, "%d",
					networkFlow[i]->getTransportSctpMaxStreams());
		}
		else
		{
			fd[i].trans_sctpid[0] = '\0';
			fd[i].trans_sctpmax[0] = '\0';
		}

		// Application layer
		Protocol(networkFlow[i]->getApplicationProtocol()).get(fd[i].app_p);

		// Inter deperture times
		nFittings = networkFlow[i]->getNumberOfInterdepertureTimeModels();

		//MESSER_DEBUG("nFittings=%d, fd[%d].n_interPktModels=%d  @<%s, %s>",
		//		nFittings, i, fd[i].n_interPktModels);

		for (j = 0; j < nFittings; j++)
		{
			sf = networkFlow[i]->getInterDepertureTimeModel(j);
			sf.strModelName(fd[i].interPkt[j].name);
			sprintf(fd[i].interPkt[j].aic, "%f", sf.aic());
			sprintf(fd[i].interPkt[j].bic, "%f", sf.bic());
			sprintf(fd[i].interPkt[j].param1, "%.15f", sf.param1());
			sprintf(fd[i].interPkt[j].param2, "%.15f", sf.param2());
		}

		vector2str(*networkFlow[i]->getSessionOnVector(),
				fd[i].session_OnTimes);
		vector2str(*networkFlow[i]->getSessionOffVector(),
				fd[i].session_OffTimes);

		vector2str(*networkFlow[i]->getSessionOnBytesVector(),
				fd[i].session_nBytes);
		vector2str(*networkFlow[i]->getSessionOnPacketsVector(),
				fd[i].session_nPackets);

		//Packet size mode1
		nPsM1Fittings = networkFlow[i]->getNumberOfPsMode1Models();

		for (j = 0; j < nPsM1Fittings; j++)
		{
			sf = networkFlow[i]->getPacketSizeModelMode1(j);
			sf.strModelName(fd[i].psMode1[j].name);
			sprintf(fd[i].psMode1[j].aic, "%f", sf.aic());
			sprintf(fd[i].psMode1[j].bic, "%f", sf.bic());
			sprintf(fd[i].psMode1[j].param1, "%f", sf.param1());
			sprintf(fd[i].psMode1[j].param2, "%f", sf.param2());
		}
		sprintf(fd[i].psMode1_Nkbytes, "%ld",
				networkFlow[i]->getNkbytesMode1());
		sprintf(fd[i].psMode1_Npackets, "%ld",
				networkFlow[i]->getNpacketsMode1());
		sprintf(fd[i].psMode2_Nkbytes, "%ld",
				networkFlow[i]->getNkbytesMode2());
		sprintf(fd[i].psMode2_Npackets, "%ld",
				networkFlow[i]->getNpacketsMode2());

		//Packet size mode 2
		nPsM2Fittings = networkFlow[i]->getNumberOfPsMode2Models();
		for (j = 0; j < nPsM2Fittings; j++)
		{
			sf = networkFlow[i]->getPacketSizeModelMode2(j);
			sf.strModelName(fd[i].psMode2[j].name);
			sprintf(fd[i].psMode2[j].aic, "%f", sf.aic());
			sprintf(fd[i].psMode2[j].bic, "%f", sf.bic());
			sprintf(fd[i].psMode2[j].param1, "%f", sf.param1());
			sprintf(fd[i].psMode2[j].param2, "%f", sf.param2());
		}
		sprintf(fd[i].psMode2_Nkbytes, "%ld",
				networkFlow[i]->getNkbytesMode2());
		sprintf(fd[i].psMode2_Npackets, "%ld",
				networkFlow[i]->getNpacketsMode2());
		sprintf(fd[i].psMode2_Nkbytes, "%ld",
				networkFlow[i]->getNkbytesMode2());
		sprintf(fd[i].psMode2_Npackets, "%ld",
				networkFlow[i]->getNpacketsMode2());

		//Reset data structure conters
		networkFlow[i]->resetCounters();

	}

	for (i = 0; i < nFlows; i++)
	{
		xml_node<>* flow = doc.allocate_node(node_element, LABEL_FLOW);

		// flow
		flow->append_attribute(
				doc.allocate_attribute(LABEL_FLOW_START_DELAY,
						fd[i].flow_start_delay));
		flow->append_attribute(
				doc.allocate_attribute(LABEL_FLOW_DURATION,
						fd[i].flow_duration));
		flow->append_attribute(
				doc.allocate_attribute(LABEL_FLOW_DS_BYTE, fd[i].flow_ds_byte));
		flow->append_attribute(
				doc.allocate_attribute(LABEL_FLOW_NUMBER_KBYTES,
						fd[i].flow_nkbytes));
		flow->append_attribute(
				doc.allocate_attribute(LABEL_FLOW_NUMBER_PACKETS,
						fd[i].flow_npackets));

		//Link layer
		xml_node<>* link_layer = doc.allocate_node(node_element,
				LABEL_LINK_LAYER);
		flow->append_node(link_layer);

		link_layer->value(fd[i].link_p);
		link_layer->append_attribute(
				doc.allocate_attribute(LABEL_MAC_SRC, fd[i].mac_src));
		link_layer->append_attribute(
				doc.allocate_attribute(LABEL_MAC_DST, fd[i].mac_dst));

		//Network layer
		xml_node<>* network_layer = doc.allocate_node(node_element,
				LABEL_NETWORK_LAYER);
		flow->append_node(network_layer);
		network_layer->value(fd[i].net_p);
		network_layer->append_attribute(
				doc.allocate_attribute(LABEL_SRC_IP, fd[i].net_addrsrc));
		network_layer->append_attribute(
				doc.allocate_attribute(LABEL_DST_IP, fd[i].net_addrdst));
		network_layer->append_attribute(
				doc.allocate_attribute(LABEL_TTL, fd[i].net_ttl));

		//Tranport layer
		xml_node<>* transport_layer = doc.allocate_node(node_element,
				LABEL_TRANSPORT_LAYER);
		flow->append_node(transport_layer);
		transport_layer->value(fd[i].trans_p);
		transport_layer->append_attribute(
				doc.allocate_attribute(LABEL_DSTPORT, fd[i].trans_pdst));
		transport_layer->append_attribute(
				doc.allocate_attribute(LABEL_SRCPORT, fd[i].trans_psrc));

		if (networkFlow[i]->getTransportProtocol() == PROTOCOL__SCTP)
		{
			network_layer->append_attribute(
					doc.allocate_attribute(LABEL_SCTP_ASSOCIATION_ID,
							fd[i].trans_sctpid));
			network_layer->append_attribute(
					doc.allocate_attribute(LABEL_SCTP_MAX_STREAMS,
							fd[i].trans_sctpmax));
		}

		// Application layer
		xml_node<>* application_layer = doc.allocate_node(node_element,
				LABEL_APPLICATION_LAYER);
		flow->append_node(application_layer);
		application_layer->value(fd[i].app_p);

		// Inter packet time
		xml_node<>* interPacketTimes = doc.allocate_node(node_element,
				LABEL_INTERPACKETTIMES);

		nFittings = networkFlow[i]->getNumberOfInterdepertureTimeModels();
		for (j = 0; j < nFittings; j++)
		{

			xml_node<>* stochastic_model = doc.allocate_node(node_element,
					LABEL_STOCHASTIC_MODEL);

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_STOCHASTIC_MODEL_NAME,
							fd[i].interPkt[j].name));
			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_AIC, fd[i].interPkt[j].aic));
			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_BIC, fd[i].interPkt[j].bic));

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_PARAM1,
							fd[i].interPkt[j].param1));

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_PARAM2,
							fd[i].interPkt[j].param2));

			interPacketTimes->append_node(stochastic_model);
		}
		flow->append_node(interPacketTimes);

		// Session Times
		xml_node<>* sessionOnOffTimes = doc.allocate_node(node_element,
				LABEL_SESSION);
		sessionOnOffTimes->append_attribute(
				doc.allocate_attribute(LABEL_SESSION_ON,
						fd[i].session_OnTimes));
		sessionOnOffTimes->append_attribute(
				doc.allocate_attribute(LABEL_SESSION_OFF,
						fd[i].session_OffTimes));

		sessionOnOffTimes->append_attribute(
				doc.allocate_attribute(LABEL_SESSION_NPACKETS,
						fd[i].session_nPackets));
		sessionOnOffTimes->append_attribute(
				doc.allocate_attribute(LABEL_SESSION_NBYTES,
						fd[i].session_nBytes));

		flow->append_node(sessionOnOffTimes);

		//Packet Size models
		xml_node<>* packetSizes = doc.allocate_node(node_element,
				LABEL_PACKETSIZES);
		flow->append_node(packetSizes);

		packetSizes->append_attribute(
				doc.allocate_attribute(LABEL_NPACKETS, fd[i].flow_npackets));
		packetSizes->append_attribute(
				doc.allocate_attribute(LABEL_NKBYTES, fd[i].flow_nkbytes));

		//Packet sizes mode 1
		xml_node<>* packetSizeMode1 = doc.allocate_node(node_element,
				LABEL_PSMODE1);
		packetSizes->append_node(packetSizeMode1);
		packetSizeMode1->append_attribute(
				doc.allocate_attribute(LABEL_NPACKETS, fd[i].psMode1_Npackets));
		packetSizeMode1->append_attribute(
				doc.allocate_attribute(LABEL_NKBYTES, fd[i].psMode1_Nkbytes));
		nPsM1Fittings = networkFlow[i]->getNumberOfPsMode1Models();
		for (j = 0; j < nPsM1Fittings; j++)
		{
			xml_node<>* stochastic_model = doc.allocate_node(node_element,
					LABEL_STOCHASTIC_MODEL);

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_STOCHASTIC_MODEL_NAME,
							fd[i].psMode1[j].name));
			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_AIC, fd[i].psMode1[j].aic));
			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_BIC, fd[i].psMode1[j].bic));

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_PARAM1,
							fd[i].psMode1[j].param1));

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_PARAM2,
							fd[i].psMode1[j].param2));

			packetSizeMode1->append_node(stochastic_model);
		}

		//Packet sizes mode 2
		xml_node<>* packetSizeMode2 = doc.allocate_node(node_element,
				LABEL_PSMODE2);
		packetSizes->append_node(packetSizeMode2);

		packetSizeMode2->append_attribute(
				doc.allocate_attribute(LABEL_NPACKETS, fd[i].psMode2_Npackets));
		packetSizeMode2->append_attribute(
				doc.allocate_attribute(LABEL_NKBYTES, fd[i].psMode2_Nkbytes));

		nPsM2Fittings = networkFlow[i]->getNumberOfPsMode2Models();
		for (j = 0; j < nPsM2Fittings; j++)
		{
			xml_node<>* stochastic_model = doc.allocate_node(node_element,
					LABEL_STOCHASTIC_MODEL);

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_STOCHASTIC_MODEL_NAME,
							fd[i].psMode2[j].name));
			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_AIC, fd[i].psMode2[j].aic));
			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_BIC, fd[i].psMode2[j].bic));

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_PARAM1,
							fd[i].psMode2[j].param1));

			stochastic_model->append_attribute(
					doc.allocate_attribute(LABEL_PARAM2,
							fd[i].psMode2[j].param2));

			packetSizeMode2->append_node(stochastic_model);
		}
		trace->append_node(flow);
	}

	// Convert doc to string if needed
	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	// Save to file
	string2charvet(fileName, charfileName);
	std::ofstream file_stored(charfileName);
	file_stored << doc;
	file_stored.close();
	doc.clear();

	PLOG_DEBUG << "Free memory allocated";
	//Free memory allocated
	for (i = 0; i < nFlows; i++)
	{
		delete[] fd[i].interPkt;
		fd[i].interPkt = NULL;

		delete[] fd[i].psMode1;
		fd[i].psMode1 = NULL;
		delete[] fd[i].psMode2;
		fd[i].psMode2 = NULL;
	}
	delete[] fd;
	fd = NULL;
	PLOG_DEBUG << "Memory was freed successfully!";

	return (0);

}

long int NetworkTrace::getNumberOfFlows() const
{
	return (networkFlow.size());
}

const std::string NetworkTrace::toString() const
{
	std::string tostring = info_tracename + info_captureDate
			+ info_commentaries;
	return (tostring);
}

int NetworkTrace::pushback_Netflow(NetworkFlow* vetNetFlow)
{
	networkFlow.push_back(vetNetFlow);
	return (0);
}

int NetworkTrace::exec(const std::string& networkInterface)
{
	int size = this->getNumberOfFlows();
	int i = 0;
	std::thread* th_flw = new std::thread[size];

	PLOG_DEBUG << "NetworkTrace::exec() >> this->getNumberOfFlows():"
						<< this->getNumberOfFlows();

	for (i = 0; i < size; i++)
	{
		PLOG_VERBOSE << "Creating flow thread networkFlow[" << i
							<< "]->flowThread()";
		th_flw[i] = networkFlow[i]->flowThread(networkInterface);

	}
	for (i = 0; i < size; i++)
	{
		PLOG_VERBOSE << "Joining flow thread networkFlow[" << i
							<< "]->flowThread()";
		th_flw[i].join();
	}

	delete[] th_flw;

	return 0;

}

void NetworkTrace::server(const std::string& networkInterface)
{

	if (this->getNumberOfFlows() >= 0)
	{
		PLOG_VERBOSE << "Executing networkFlow[0]->server()";
		networkFlow[0]->server(networkInterface);
	}
	else
	{
		PLOG_ERROR << "Error while executing networkFlow[0]->server()."
							<< "Number of flow is: "
							<< this->getNumberOfFlows();
		exit(ERROR_NULL_POINTER);
	}
}

void NetworkTrace::clientServerIps(const char* filename,
		const char* etherInterface, bool set_mac)
{
	char localhost[CHAR_BUFFER];
	char ether[CHAR_BUFFER];
	int is_if_empty = strcmp(etherInterface, "");

	PLOG_DEBUG << "clientServerIps filename:" << filename
				<< ", etherInterface:" << etherInterface;
	if (is_if_empty == 0)
	{
		getLocalIfIp(ether, localhost);
	}
	else
	{
		getLocalIp(etherInterface, localhost);
	}

	setFileIpMac(filename, localhost, set_mac);
}

void NetworkTrace::clientServerIps(const char* serverIpAddr,
		const char* serverMacAddr, const char* etherInterface)
{
	uint nflows = uint(getNumberOfFlows());
	char localhost[CHAR_BUFFER];
	char ether[CHAR_BUFFER];
	int is_if_empty_ether = strcmp(etherInterface, "");
	int is_if_empty_mac = strcmp(serverMacAddr, "");

	if (is_if_empty_ether == 0)
	{
		getLocalIfIp(ether, localhost);
	}
	else
	{
		getLocalIp(etherInterface, localhost);
	}

	PLOG_DEBUG << "Is interface empty? " << is_if_empty_ether;
	PLOG_DEBUG << "Is MAC empty? " << is_if_empty_mac;

	for (uint i = 0; i < nflows; i++)
	{
		PLOG_DEBUG << "Changing flow " << i;

		networkFlow[i]->setNetworkSrcAddr(localhost);
		networkFlow[i]->setNetworkDstAddr(serverIpAddr);
		if (is_if_empty_mac != 0)
			networkFlow[i]->setMacDstAddr(serverMacAddr);

		PLOG_DEBUG << "Flow[" << i << "] IPsrc:"
							<< networkFlow[i]->getNetworkSrcAddr() << "IPdst:"
							<< networkFlow[i]->getNetworkDstAddr();
	}

}

inline int NetworkTrace::getLocalIfIp(char* interface, char* ipaddr)
{
	struct ifaddrs *ifaddr, *ifa;
	int s;
	char host[NI_MAXHOST];

	PLOG_ERROR << "getLocalIfIp for interface:" << interface <<
			", ipaddr:" << ipaddr;
	if (getifaddrs(&ifaddr) == -1)
	{
		PLOG_ERROR << "Error, cant execute getifaddrs()";
		exit(ERROR_BAD_VALUE);
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{

		if (ifa->ifa_addr == NULL)
			continue;

		s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host,
		NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

		if ((strcmp(ifa->ifa_name, "lo") != 0)
				&& (ifa->ifa_addr->sa_family == AF_INET))
		{
			if (s != 0)
			{
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				return (-2);
			}
			strcpy(interface, ifa->ifa_name);
			strcpy(ipaddr, host);
			break;
		}
	}

	freeifaddrs(ifaddr);
	return (0);
}

inline int NetworkTrace::getLocalIp(const char* interface, char* ipaddr)
{
	struct ifaddrs *ifaddr, *ifa;
	int s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1)
	{
		PLOG_ERROR << "Error, cant execute getifaddrs()";
		exit(ERROR_BAD_VALUE);
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;

		s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host,
		NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

		if ((strcmp(ifa->ifa_name, interface) == 0)
				&& (ifa->ifa_addr->sa_family == AF_INET))
		{
			if (s != 0)
			{
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				return (-2);
			}

			strcpy(ipaddr, host);
			break;
		}
	}

	freeifaddrs(ifaddr);
	return (0);
}

void NetworkTrace::setFileIpMac(const char* filename, const char* localhost,
		bool set_mac)
{
	uint nflows = uint(getNumberOfFlows());
	CsvFile file_csv(filename);
	std::string ip_addr;
	std::string mac_addr;

	for (uint i = 0; i < nflows;)
	{
		do
		{
			ip_addr = file_csv.column(0);
			mac_addr = file_csv.column(1);
			networkFlow[i]->setNetworkSrcAddr(localhost);
			networkFlow[i]->setNetworkDstAddr(ip_addr);
			if (set_mac == true)
				networkFlow[i]->setMacDstAddr(mac_addr);
			i++;

			if (i >= nflows)
				break;
		} while (file_csv.next_line());
		file_csv.reset();
	}
}

void NetworkTrace::regression_tests()
{
	RegressionTests rt = RegressionTests();

	rt.printHeader("class NetworkTrace");
	rt.printTestResult("Read and Write to the XML", test_readWrite2XML());
	rt.printTestResult("test On/Off Vector sizes consistency",
			test_OnOffSizes());
	rt.printTestResult("FileIpMac", test_setFileIpMac());
	rt.printTestResult("Local IP", test_getLocalIp());
}

bool NetworkTrace::test_readWrite2XML()
{
	SimitarWorkspace workspace = SimitarWorkspace();
	bool noError = true;
	FILE* in;
	char buff[CHAR_BUFFER];
	const char mode[] = "r";
	std::string the_command = "diff " + workspace.test_ctd_xml() + " "
			+ workspace.test_ctd_xml_copy2();
	char command[CHAR_LARGE_BUFFER];
	strcpy(command, the_command.c_str());
	//const char command[] =
	//		"diff data/regression-tests/test-trace.xml data/regression-tests/copy2-test-trace.xml";
	//const char command[] = "ls -lahn data/regression-tests/";

	//NetworkTrace tempTrace = NetworkTrace(
	//		"data/regression-tests/test-trace.xml", "D-ITG");
	//tempTrace.writeToFile("data/regression-tests/copy2-test-trace.xml");
	NetworkTrace tempTrace = NetworkTrace(workspace.test_ctd_xml(), "D-ITG");
	tempTrace.writeToFile(workspace.test_ctd_xml_copy2());

	if (!(in = popen(command, mode)))
	{
		return (false);
	}
	// If there is any differerence captured by diff, it prints in the
	// stdout, and exists indicating test failed
	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		if (strcmp(buff, "") != 0)
		{
			fprintf(stderr, "diff:%s", buff);
			noError = false;
		}
	}
	pclose(in);

	return (noError);
}

bool NetworkTrace::test_OnOffSizes()
{
	SimitarWorkspace workspace = SimitarWorkspace();
	//NetworkTrace tempTrace = NetworkTrace(
	//		"data/regression-tests/test-trace.xml", "D-ITG");
	NetworkTrace tempTrace = NetworkTrace(workspace.test_ctd_xml(), "D-ITG");
	uint i = 0;
	vector<time_sec>* onvector;
	vector<time_sec>* offvector;
	vector<uint>* pktvector;
	vector<uint>* bytesvector;

	for (i = 0; i < tempTrace.networkFlow.size(); i++)
	{

		onvector = tempTrace.networkFlow[i]->getSessionOnVector();
		offvector = tempTrace.networkFlow[i]->getSessionOffVector();
		pktvector = tempTrace.networkFlow[i]->getSessionOnPacketsVector();
		bytesvector = tempTrace.networkFlow[i]->getSessionOnBytesVector();

		if (onvector->size() != pktvector->size())
		{
			cout << "flow[" << i << "]: OnVector.size()=" << onvector->size()
					<< " OnPacketsVector.size()=" << pktvector->size() << endl;
			return (false);
		}
		if (onvector->size() != bytesvector->size())
		{
			cout << "flow[" << i << "]: OnVector.size()=" << onvector->size()
					<< " OnBytesVector.size()=" << bytesvector->size() << endl;
			return (false);
		}
		if (unsigned(onvector->size() - 1) != unsigned(offvector->size()))
		{
			cout << "flow[" << i << "]: OnVector.size()=" << onvector->size()
					<< " OffVector.size()=" << offvector->size() << endl;
			return (false);
		}
	}

	return (true);

}

bool NetworkTrace::test_setFileIpMac()
{
	SimitarWorkspace workspace = SimitarWorkspace();
	//NetworkTrace tempTrace = NetworkTrace(
	//		"data/regression-tests/test-trace.xml", "D-ITG");
	NetworkTrace tempTrace = NetworkTrace(workspace.test_ctd_xml(), "D-ITG");

	//tempTrace.setFileIpMac("data/regression-tests/ipmac.txt", "10.1.1.1", true);
	tempTrace.setFileIpMac(workspace.test_ipmac_file().c_str(), "10.1.1.1",
			true);

	//tempTrace.writeToFile("changed-ips-macs-trace.xml");
	tempTrace.writeToFile(workspace.test_ctd_ipmac());

	return (true);
}

bool NetworkTrace::test_getLocalIp()
{
	char local_host[CHAR_BUFFER];
	char local_ether[CHAR_BUFFER];

	PLOG_DEBUG << "test_getLocalIp()";
	getLocalIfIp(local_ether, local_host);

	//cout << "localhost: " << local_host << endl;
	//cout << "ether: " << local_ether << endl;
	PLOG_DEBUG <<  "localhost: " << local_host;
	PLOG_DEBUG << "ether: " << local_ether ;


	return (true);
}

