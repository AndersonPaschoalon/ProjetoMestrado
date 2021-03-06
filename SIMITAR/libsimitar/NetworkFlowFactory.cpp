/*
 * NetworkFlowFactory.cpp
 *
 *  Created on: 14 de set de 2017
 *      Author: anderson
 */

#include "NetworkFlowFactory.h"

NetworkFlowFactory::NetworkFlowFactory()
{

}

NetworkFlowFactory::~NetworkFlowFactory()
{
	PLOG_VERBOSE << "~NetworkFlowFactory()";
}


NetworkFlow* NetworkFlowFactory::make_flow(const std::string& choise)
{
	std::string choise_lc;

	// Allocate the destination space
	choise_lc.resize(choise.size());
	// Convert the source string to lower case storing the result in destination string
	std::transform(choise.begin(), choise.end(), choise_lc.begin(), ::tolower);

	if (choise_lc == "dummy")
	{
		PLOG_VERBOSE << "Creating DummyFlow";
		return new DummyFlow;
	}
	else if(choise_lc == "ditg")
	{
		PLOG_VERBOSE << "Creating DitgFlow";
		return new DitgFlow;
	}
	else if(choise_lc == "iperf")
	{
		PLOG_VERBOSE << "Creating IperfFlow";
		return new IperfFlow;
	}
	else if((choise_lc == "tins") || (choise_lc == "libtins"))
	{
		PLOG_VERBOSE << "Creating TinsFLow";
		return new TinsFlow;
	}
	else if (choise_lc == "ostinato"){
		PLOG_VERBOSE << "Creating OstinatoFLow";
		printf("TODO\n");
	}

	PLOG_WARN << ERRORMSG_BAD_VALUE << choise ;
	PLOG_INFO << "Instantiating DummyFlow as default";
	return new DummyFlow;
}

std::string NetworkFlowFactory::support_tools()
{
	return("dummy, ditg, iperf, tins");
}
