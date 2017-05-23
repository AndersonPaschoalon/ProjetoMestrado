/*
 * DitgFlow.h
 *
 *  Created on: 16 de set de 2016
 *      Author: apaschoalon
 */

#ifndef DITGFLOW_H_
#define DITGFLOW_H_

#include "NetworkFlow.h"
#include "DummyFlow.h"

class DitgFlow: public DummyFlow
{
public:
	/**
	 *
	 */
	DitgFlow()
	{
		//TODO
	}
	/**
	 *
	 */
	virtual ~DitgFlow()
	{
		//TODO
	}

	/**
	 *
	 */
	void flowGenerate(const counter& flowId, const time_sec& onTime,
			const uint& npackets, const uint& nbytes,
			const string& netInterface)
	{
		//TODO Criar um metodo que retorna o IP de eth0 (default) da maquina atual
		char host[] = "10.0.0.1";
		char ethernetInterface[] = "h1-eth0";

		////////////////////////////////////////////////////////////////////////////
		///Initialization
		////////////////////////////////////////////////////////////////////////////

		string strCommand;
		char command[CHAR_BUFFER];

		int rc = 0;

		////////////////////////////////////////////////////////////////////////////
		/// D-ITG settings parser
		////////////////////////////////////////////////////////////////////////////

		/// Flow-level settings

		//duration
		strCommand += " -t " + std::to_string(onTime);
		strCommand += " -z " + std::to_string(npackets);
		strCommand += " -k " + std::to_string(nbytes / 1024);
		//DS byte
		strCommand += " -b " + std::to_string(getFlowDsByte());
		//Guarantee the mean packet rate
		strCommand += " -j 1";

		/// Network-layer settings
		strCommand += " -f " + std::to_string(getNetworkTtl());
		//destination and sources IPv4/IPv6 address
		strCommand += " -a " + getNetworkDstAddr();
		//strCommand += " -sa " + getNetworkSrcAddr();

		/// Transport-layer settings
		if (this->getTransportProtocol() == PROTOCOL__TCP)
		{
			strCommand += " -T TCP -D ";
			strCommand += " -rp " + std::to_string(getTransportDstPort());
			strCommand += " -sp " + std::to_string(getTransportSrcPort());
		}
		else if (this->getTransportProtocol() == PROTOCOL__UDP)
		{
			strCommand += " -T UDP ";
			strCommand += " -rp " + std::to_string(getTransportDstPort());
			strCommand += " -sp " + std::to_string(getTransportSrcPort());
		}
		else if (this->getTransportProtocol() == PROTOCOL__ICMP)
		{
			//TODO set ICMP type
			strCommand += " -T ICMP ";
		}
		else if (this->getTransportProtocol() == PROTOCOL__SCTP)
		{
			strCommand += " -T SCTP "
					+ std::to_string(getTransportSctpAssociationId());
		}
		else if (this->getTransportProtocol() == PROTOCOL__DCCP)
		{
			strCommand += " -T DCCP ";
		}
		else
		{
			strCommand += " -T UDP -rp "
					+ std::to_string(this->randTranportPort());
		}

		/// Inter-departure time options
		unsigned int i = 0;
		for (;; i++)
		{
			//StochasticModelFit idtModel =
			//		this->getInterDepertureTimeModel_next();
			StochasticModelFit idtModel = this->getInterDepertureTimeModel(i);

			if ((idtModel.modelName() == WEIBULL)
					|| (idtModel.modelName() == NORMAL)
					|| (idtModel.modelName() == EXPONENTIAL_LINEAR_REGRESSION)
					|| (idtModel.modelName() == EXPONENTIAL_MEAN)
					|| (idtModel.modelName() == PARETO_LINEAR_REGRESSION)
					|| (idtModel.modelName() == PARETO_MAXIMUM_LIKEHOOD)
					|| (idtModel.modelName() == CAUCHY)
					|| (idtModel.modelName() == CONSTANT))
			{
				if (idtModel.modelName() == WEIBULL)
				{
					//idtModel.param1 = alpha (shape)
					//idtModel.param2 = betha (scale)
					strCommand += " -W " + std::to_string(idtModel.param1())
							+ " " + std::to_string(idtModel.param2());
				}
				else if (idtModel.modelName() == NORMAL)
				{
					//idtModel.param1 = mu (shape)
					//idtModel.param2 = sigma dev
					strCommand += " -N " + std::to_string(idtModel.param1())
							+ " " + std::to_string(idtModel.param2());
				}
				else if ((idtModel.modelName() == EXPONENTIAL_LINEAR_REGRESSION)
						|| (idtModel.modelName() == EXPONENTIAL_MEAN))
				{
					//idtModel.param1 = mean
					strCommand += " -E " + std::to_string(idtModel.param1());
				}
				else if ((idtModel.modelName() == PARETO_LINEAR_REGRESSION)
						|| ((idtModel.modelName() == PARETO_MAXIMUM_LIKEHOOD)))
				{
					//idtModel.param1 = alpha (shape)
					//idtModel.param2 = xm (scale)
					double param2;
					if (idtModel.param2() < 0.000001)
						param2 = 0.000001;
					else
						param2 = idtModel.param2();

					strCommand += " -V " + std::to_string(idtModel.param1())
							+ " " + std::to_string(param2);
				}
				else if (idtModel.modelName() == CAUCHY)
				{
					//idtModel.param1 = gamma (scale)
					//idtModel.param2 = xm (shape - location)
					double param1;
					if (idtModel.param1() < 0.000001)
						param1 = 0.000001;
					else
						param1 = idtModel.param1();

					strCommand += " -Y " + std::to_string(idtModel.param2())
							+ " " + std::to_string(param1);

					//strCommand += " -Y " + std::to_string(idtModel.param2())
					//		+ " " + std::to_string(idtModel.param1());
				}
				else //CONSTANT
				{
					strCommand += " -C " + std::to_string(idtModel.param1());
				}
				break;
			}
		}

		/// Packet size options

		//First model: Contant
		//strCommandMode1 = strCommand + " -c "
		//		+ std::to_string(this->getPacketSizeModelMode1_next().param1());
		//First model: Contant
		//strCommandMode2 = strCommand + " -c "
		//		+ std::to_string(this->getPacketSizeModelMode2_next().param1());

		//if(getPacketSizeModelMode2(0).modelName())
		//
		//strCommandMode1 = strCommand + " -c "
		//		+ std::to_string(this->getPacketSizeModelMode1(0).param1());
		//First model: Contant
		//strCommandMode2 = strCommand + " -c "
		//		+ std::to_string(this->getPacketSizeModelMode2(0).param1());

		/// Generate C string

		//command = new char[strCommand.length() + 1];
		//strcpy(command, strCommand.c_str());

		//commandMode1 = new char[strCommandMode1.length() + 1];
		//strcpy(commandMode1, strCommandMode1.c_str());

		//commandMode2 = new char[strCommandMode2.length() + 1];
		//strcpy(commandMode2, strCommandMode2.c_str());

		////////////////////////////////////////////////////////////////////////////
		/// D-ITG: generatte flow
		////////////////////////////////////////////////////////////////////////////

		strcpy(command, strCommand.c_str());
		cout << "flow" << flowId << ">" << "command" << strCommand.c_str()
				<< endl;
		rc = DITGsend(host, command);
		if (rc != 0)
		{
			perror("Error on  DITGsend() @ DummyFlow::flowGenerate()");
			printf("\nDITGsend() return value was %d\n", rc);
			errno = EAGAIN;
			exit(EXIT_FAILURE);
		}

		/*
		 if (this->getNpacketsMode1() > 0)
		 {
		 //send mode one
		 rc = DITGsend(host, commandMode1);
		 if (rc != 0)
		 {
		 perror("Error on  DITGsend() @ DummyFlow::flowGenerate()");
		 printf("\nDITGsend() return value was %d\n", rc);
		 errno = EAGAIN;
		 exit(EXIT_FAILURE);
		 }

		 }
		 if (this->getNpacketsMode2() > 0)
		 {
		 //send mode two
		 rc = DITGsend(host, commandMode2);
		 if (rc != 0)
		 {
		 perror("Error on  DITGsend() @ DummyFlow::flowGenerate()");
		 printf("\nDITGsend() return value was %d\n", rc);
		 errno = EAGAIN;
		 exit(EXIT_FAILURE);
		 }

		 }

		 */
		////////////////////////////////////////////////////////////////////////////
		/// Ending
		////////////////////////////////////////////////////////////////////////////
		//delete[] command;
		//delete[] commandMode1;
		//delete[] commandMode2;
#ifdef DEBUG
		cout << "D-ITG command: " << command << endl;
#endif

		return;
	}
private:
};

#endif /* DITGFLOW_H_ */
