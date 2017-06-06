/*
 * PrintCharFlow.h
 *
 *  Created on: 9 de mai de 2017
 *      Author: anderson
 */

#ifndef PRINTCHARFLOW_H_
#define PRINTCHARFLOW_H_

#include "DummyFlow.h"

class PrintCharFlow: public DummyFlow
{
public:
	PrintCharFlow()
	{

	}
	virtual ~PrintCharFlow()
	{

	}
	inline void flowGenerate(const counter& flowId, const time_sec& onTime,
			const unsigned int& npackets, const uint& nbytes,
			const string& netInterface)
	{
		pthread_mutex_t lock;
		pthread_mutex_init(&lock, NULL);
		pthread_mutex_lock(&lock);
		printf("%d", flowId);
		pthread_mutex_unlock(&lock);
		pthread_mutex_destroy(&lock);

		/*

		uint64_t  wait_us = time_t(onTime * 1.0e6);
		struct timespec start, end;
		uint64_t delta_us;

		pthread_mutex_t lock;
		pthread_mutex_init(&lock, NULL);
		pthread_mutex_lock(&lock);
		printf("%d", flowId);
		pthread_mutex_unlock(&lock);
		pthread_mutex_destroy(&lock);

		//printf("%d", flowId);
		unsigned int i = 0;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		for(;;)
		{
			i++;
			if((i % 100000) == 0)
			{
				//printf("%d", flowId);
			}
			clock_gettime(CLOCK_MONOTONIC_RAW, &end);
			delta_us = (end.tv_sec - start.tv_sec) * 1000000
						+ (end.tv_nsec - start.tv_nsec) / 1000;
			if(delta_us <  wait_us){
				continue;
			}
			else
			{
				break;
			}
		}
		*/

	}
};

#endif /* PRINTCHARFLOW_H_ */