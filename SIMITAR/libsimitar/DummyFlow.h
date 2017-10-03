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
	 * @return
	 */
	int server();

	/**
	 *
	 * @param flowId
	 * @param onTime
	 * @param npackets
	 * @param nbytes
	 * @param netInterface
	 */
	void flowGenerate(const counter& flowId, const time_sec& onTime,
			const uint& npackets, const uint& nbytes,
			const string& netInterface);

	void flowStart();

	std::thread flowThread()
	{
		return std::thread([=]
		{	flowStart();});
	}

private:

	/**
	 *
	 * @param sleep_time
	 */
	void fsleep(time_sec sleep_time);

};

#endif /* DUMMYFLOW_H_ */
