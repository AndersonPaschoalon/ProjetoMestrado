/*
 * NetworkTrace.h
 *
 *  Created on: 14 de jun de 2016
 *      Author: apaschoalon
 */

#ifndef NETWORKTRACE_H_
#define NETWORKTRACE_H_

//external libs
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <vector>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <rapidxml/rapidxml_iterators.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>

//#include "rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;
using std::string;
using std::cout;
using std::cerr;
using std::clog;
using std::cin;
using std::endl;
using std::vector;

//local libs
#include "Defines.h"
#include "cfunctions.h"
#include "NetworkFlow.h"
#include "Protocol.h"
#include "RegressionTests.h"
#include "MesserLog.h"

//#define LOG_LEVEL INFO
#define LOG_LEVEL INFO

//namespaces
typedef struct flow_data_struct flowData;
typedef struct model_data_struct modelData;

class NetworkTrace
{
public:

	/**
	 *
	 */
	//NetworkFlow* netFlow;
	vector<NetworkFlow*> networkFlow;

	/**
	 *
	 */
	NetworkTrace();

	/**
	 * Contructor, autoset from a file
	 */
	NetworkTrace(const string& fileName);

	/**
	 * Destructor. Free any stored memory
	 */
	virtual ~NetworkTrace();

	/**
	 * TODO: this method should be private. I just have to pass a pointer
	 * to the database interface, and the experiment name. The object
	 * DataProcessor should be hide and instantiated by the NetworkTrace
	 * overloaded constructor:
	 * => NetworkTrace(string experimentName, DatabaseInterface db)
	 * -----
	 * This method take as input a network flow object, make a copy by
	 * value of this data structure, and store in the flow list.
	 */
	int pushback_Netflow(NetworkFlow* netFlow);

	/**
	 *
	 *
	 */
	int writeToFile(const string& fileName) const;

	/**
	 * call wirteToFile, but uses getInfoTracename() for the file name.
	 *
	 */
	int writeToFile(void) const;

	//##########################################################################
	//Getters and Setters
	//##########################################################################

	/**
	 * Returns a string information about the class
	 */
	const string toString() const;

	/**
	 *
	 */
	const string& getInfoCaptureDate() const;

	/**
	 *
	 */
	void setInfoCaptureDate(const string& infoCaptureDate);

	/**
	 *
	 */
	const string& getInfoCaptureInterface() const;

	/**
	 *
	 */
	void setInfoCaptureInterface(const string& infoCaptureInterface);

	/**
	 *
	 */
	const string& getInfoCommentaries() const;

	/**
	 *
	 */
	void setInfoCommentaries(const string& infoCommentaries);

	/**
	 *
	 */
	const string& getInfoTracename() const;

	/**
	 *
	 */
	void setInfoTracename(const string& infoTracename);

	/**
	 *
	 */
	long int getNumberOfFlows() const;

	/**
	 *
	 * @param verbose
	 * @return
	 */
	int exec(bool verbose);

	/**
	 *
	 * @return
	 */
	const string& getTrafficGenEngine() const
	{
		return m_trafficGenEngine;
	}

	/**
	 *
	 * @param trafficGenEngine
	 */
	void setTrafficGenEngine(const string& trafficGenEngine)
	{
		m_trafficGenEngine = trafficGenEngine;
	}

	void regression_tests();

private:

	//atributes
	string m_trafficGenEngine;
	string info_tracename;
	string info_captureInterface;
	string info_captureDate;
	string info_commentaries;

	//constants
	static const char * LABEL_TRACE;
	static const char * LABEL_TRACE_NAME;
	static const char * LABEL_TRAFFIC_ENGINE;
	static const char * LABEL_CAPTURE_DATE;
	static const char * LABEL_COMMENTARIES;
	static const char * LABEL_NUMBER_OF_FLOWS;
	static const char * LABEL_FLOW;
	static const char * LABEL_FLOW_START_DELAY;
	static const char * LABEL_FLOW_DURATION;
	static const char * LABEL_FLOW_DS_BYTE;
	static const char * LABEL_FLOW_NUMBER_KBYTES;
	static const char * LABEL_FLOW_NUMBER_PACKETS;
	static const char * LABEL_LINK_LAYER;
	static const char * LABEL_NETWORK_LAYER;
	static const char * LABEL_TTL;
	static const char * LABEL_SRC_IP;
	static const char * LABEL_DST_IP;
	static const char * LABEL_TRANSPORT_LAYER;
	static const char * LABEL_SRCPORT;
	static const char * LABEL_DSTPORT;
	static const char * LABEL_SCTP_ASSOCIATION_ID;
	static const char * LABEL_SCTP_MAX_STREAMS;
	static const char * LABEL_APPLICATION_LAYER;
	static const char * LABEL_INTERPACKETTIMES;
	static const char * LABEL_INTERARRIVAL;
	static const char * LABEL_INTERFILE;
	static const char * LABEL_INTERSESSION;
	static const char * LABEL_PACKETSIZES;
	static const char * LABEL_PSMODE1;
	static const char * LABEL_PSMODE2;
	static const char * LABEL_STOCHASTIC_MODEL;
	static const char * LABEL_STOCHASTIC_MODEL_NAME;
	static const char * LABEL_PARAM1;
	static const char * LABEL_PARAM2;
	static const char * LABEL_AIC;
	static const char * LABEL_BIC;
	static const char * LABEL_NPACKETS;
	static const char * LABEL_NKBYTES;

	//methods
	void string2charvet(const string s, char* vetc) const;

	//
	bool test_string2charvet();

};

struct model_data_struct
{
	char name[CHAR_BUFFER];
	char param1[CHAR_BUFFER];
	char param2[CHAR_BUFFER];
	char aic[CHAR_BUFFER];
	char bic[CHAR_BUFFER];
};

struct flow_data_struct
{
	//Inter packet times

	unsigned int session_onoffStrLen;
	char* session_OnOffTimes;

	unsigned int n_interFileModels;
	modelData* interFileOn;
	modelData* interFileOff;

	unsigned int n_interPktModels;
	modelData* interPkt;

	//Packet sizes

	unsigned int n_psM1Models;
	unsigned int n_psM2Models;
	modelData* psMode1;
	modelData* psMode2;
	char psMode1_Nkbytes[CHAR_BUFFER];
	char psMode1_Npackets[CHAR_BUFFER];
	char psMode2_Nkbytes[CHAR_BUFFER];
	char psMode2_Npackets[CHAR_BUFFER];

	//Flow

	char flow_duration[CHAR_BUFFER];
	char flow_start_delay[CHAR_BUFFER];
	char flow_ds_byte[CHAR_BUFFER];
	char flow_npackets[CHAR_BUFFER];
	char flow_nkbytes[CHAR_BUFFER];

	//Protocols

	char link_p[CHAR_BUFFER];
	char net_p[CHAR_BUFFER];
	char net_addrsrc[CHAR_BUFFER];
	char net_addrdst[CHAR_BUFFER];
	char net_ttl[CHAR_BUFFER];
	char trans_p[CHAR_BUFFER];
	char trans_psrc[CHAR_BUFFER];
	char trans_pdst[CHAR_BUFFER];
	char trans_sctpid[CHAR_BUFFER];
	char trans_sctpmax[CHAR_BUFFER];
	char app_p[CHAR_BUFFER];

};

#endif /* NETWORKTRACE_H_ */