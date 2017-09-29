/*
 * DummyFlow.cpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: apaschoalon
 */

#include "Defines.h"
#include "DummyFlow.h"

DummyFlow::DummyFlow() :
		time_scale_factor(1.0)
{
}

DummyFlow::~DummyFlow()
{
}

void DummyFlow::print()
{
}

int DummyFlow::server()
{
	printf("[Server-mode operation]\n");
	return (0);
}

void DummyFlow::flowStart()
{
	PLOG_INIT(debug);

	/// flow-related vars
	std::string netInterface = "";
	time_sec sec_startDelay = getFlowStartDelay(); // times in seconds
	//time_t usec_startDelay = time_t(sec_startDelay * 1.0e6); // time in useconds
	uint fid = int(getFlowId());
	/// loop vars
	time_t usec_offTime = 0; // time in useconds
	time_sec sec_onTime = 0;
	time_sec sec_offTime = 0;
	uint nbytes = 0;
	uint npackets = 0;

	fsleep(sec_startDelay/time_scale_factor);
	//usleep(usec_startDelay);
	while (1)
	{
		sec_onTime = getSessionOnTime_next();
		npackets = getSessionOnTime_nPackets();
		nbytes = getSessionOnTime_nBytes();

		flowGenerate(fid, sec_onTime, npackets, nbytes, netInterface);

		sec_offTime = getSessionOffTime_next();
		if (sec_offTime == 0)
		{
			break;
		}
		//usec_offTime = time_t(sec_offTime * 1.0e6);
		//usleep(usec_offTime);
		fsleep(sec_offTime/time_scale_factor);

	}

}

void DummyFlow::flowGenerate(const counter& flowId, const time_sec& onTime,
		const uint& npackets, const uint& nbytes, const string& netInterface)
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
	flow_str_print += "Flow" + std::to_string(flowId) + "> Duration:"
			+ std::to_string(getFlowDuration()) + ", Start-delay:"
			+ std::to_string(getFlowStartDelay()) + "s" + ", N.packets: "
			+ std::to_string(getNumberOfPackets());

	////////////////////////////////////////////////////////////////////////////
	/// Link-layer protocol
	////////////////////////////////////////////////////////////////////////////
	flow_str_print += " Link[" + Protocol(this->getLinkProtocol()).str() + "]";

	////////////////////////////////////////////////////////////////////////////
	/// Network-layer protocol
	////////////////////////////////////////////////////////////////////////////
	flow_str_print += " Network[" + Protocol(this->getNetworkProtocol()).str()
			+ "]";

	////////////////////////////////////////////////////////////////////////////
	/// Transport-layer protocol
	////////////////////////////////////////////////////////////////////////////
	flow_str_print += " Transport["
			+ Protocol(this->getTransportProtocol()).str() + "]";

	// Application protocol
	flow_str_print += " Application["
			+ Protocol(this->getApplicationProtocol()).str() + "]";

	////////////////////////////////////////////////////////////////////////////
	/// Inter-deperture model
	////////////////////////////////////////////////////////////////////////////
	flow_str_print += " Inter-deperture["
			+ this->getInterDepertureTimeModel(0).strModelName() + "]";

	////////////////////////////////////////////////////////////////////////////
	/// Packet-size model
	////////////////////////////////////////////////////////////////////////////
	flow_str_print += " Packet-size-Mode1["
			+ this->getPacketSizeModelMode1(0).strModelName() + "]";
	flow_str_print += " Packet-size-Mode2["
			+ this->getPacketSizeModelMode2(0).strModelName() + "]";

	simitar_iostream_mutex.lock();
	printf("%s\n", flow_str_print.c_str());
	simitar_iostream_mutex.unlock();

}

void DummyFlow::fsleep(time_sec sleep_time)
{
	// TODO initialize this value in the constructor
	sleep_method sleepMethod = method_usleep;

	if (sleepMethod == method_select)
	{
		struct timeval tv;
		tv.tv_sec = int(sleep_time);
		tv.tv_usec = int((sleep_time - floor(sleep_time)) * 1e6);
		int ret;
		do
		{
			ret = select(1, NULL, NULL, NULL, &tv);
		} while ((ret == -1) && (errno == EINTR)); //select is interruped too

	}
	else if (sleepMethod == method_pooling)
	{
		uint64_t delta_us;
		struct timespec start, end;
		uint64_t wait_us = time_t(sleep_time * 1.0e6);
		for (;;)
		{
			clock_gettime(CLOCK_MONOTONIC_RAW, &end);
			delta_us = (end.tv_sec - start.tv_sec) * 1000000
					+ (end.tv_nsec - start.tv_nsec) / 1000;
			if (delta_us < wait_us)
			{
				continue;
			}
			else
			{
				break;
			}
		}

	}
	else // method_usleep
	{
		time_t  usec_delay = time_t(sleep_time * 1.0e6);
		usleep(usec_delay);
	}

}
