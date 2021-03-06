//============================================================================
// Name        : Main.cpp
// Author      : Anderson Paschoalon
// Version     :
// Copyright   : Your copyright notice
// Description : Trace Analyzer SIMITAR component, Ansi-style
//============================================================================

// Deps
#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>
#include <vector>
// Simitar
#include <DatabaseInterface.h>
#include <TraceDbManager.h>
#include <DataProcessor.h>
#include <DummyFlow.h>
#include <NetworkTrace.h>
#include <Defines.h>
#include <Protocol.h>
#include <cfunctions.h>
#include <SimitarWorkspace.h>

void cli_error_messege(std::string arg, std::string arg_name,
		std::string expected_values);

bool cli_check_val(std::string val, std::vector<std::string> expectedVals);

int main(int argc, char** argv)
{
	try
	{
		// msgs
		char progDescription[] =
				"SIMITAR trace-analyzer component. It take as input a measured trace from sniffer, \
and generate a Compact Trace Descriptor in the XML format. The Compact Trace Descriptor files are \
stored inside the SIMITAR workspace in the directory `data/xml/`, and may be edited manually. ";

		// version
		char version[CHAR_BUFFER];
		SimitarWorkspace workspace = SimitarWorkspace();
		std::string str_version = workspace.version_tag() + ":"
				+ workspace.version_name();
		strcpy(version, str_version.c_str());

		//vars
		//time_scale dpTimeScale;

		TCLAP::CmdLine cmd(progDescription, ' ', version);

		TCLAP::ValueArg<std::string> trace_arg("t", "trace",
				"Trace used to generate the Compact Trace Descriptor", true, "",
				"string");
		//TCLAP::ValueArg<std::string> timescale_arg("s", "time-scale",
		//		"Time scale of the Compact Trace Descriptor. `seconds` or `milliseconds`. Default: `milliseconds``",
		//		false, "milliseconds", "string");
		TCLAP::ValueArg<double> minon_arg("n", "min-on",
				"min_on_time constant used by the algorithm calcOnOff. Defines the minimum time of a packet train. Default: 0.1s",
				false, 0.1, "double");
		TCLAP::ValueArg<double> minoff_arg("f", "min-off",
				"sesssion_cut_time constant used by the algorithm calcOnOff. Defines the minimum OFF time between sessions. Default: 30.0s",
				false, 30.0, "double");
		TCLAP::ValueArg<std::string> criterion_arg("c", "criterion",
				"Criterion used to sort the stochastic models, `aic` or `bic`. Default: `aic`",
				false, "aic", "string");
		TCLAP::ValueArg<int> minpkts_arg("p", "min-pkts",
				"Minimum numbers of packets of a flow that will allow stochastic fitting. Flow with less packets will use just the constant model. Default: 30.",
				false, 30, "int");

		cmd.add(trace_arg);
		//cmd.add(timescale_arg);
		cmd.add(minon_arg);
		cmd.add(minoff_arg);
		cmd.add(criterion_arg);
		cmd.add(minpkts_arg);

		cmd.parse(argc, argv);

		std::string trace_name = trace_arg.getValue();
		//std::string timescale_val = timescale_arg.getValue();
		double on_val = minon_arg.getValue();
		double off_val = minoff_arg.getValue();
		std::string crit_val = criterion_arg.getValue();
		int pkts_val = minpkts_arg.getValue();
		//std::string timescaleSufix;

		//if (!cli_check_val(timescale_val,
		//{ "milliseconds", "seconds" }))
		//{
		//	cli_error_messege(timescale_val, "time-scale",
		//			"`milliseconds` or `seconds`");
		//	return (-1);
		//}
		if (!cli_check_val(crit_val,
		{ "aic", "bic" }))
		{
			cli_error_messege(crit_val, "criterion", "`aic` or `bic`");
			return (-1);
		}

		//dpTimeScale = (timescale_val == "seconds") ? seconds : milliseconds;
		//timescaleSufix = (timescale_val == "seconds")? ".sec" : ".ms";

		//if (timescale_val == "milliseconds")
		//	dpTimeScale = milliseconds;
		//else if (timescale_val == "seconds")
		//	dpTimeScale = seconds;

		if (workspace.database_version() == "1")
		{
			DatabaseInterface dbif = DatabaseInterface();
			NetworkTrace traceSec = NetworkTrace();
			NetworkTrace traceMs = NetworkTrace();

			DataProcessor dp = DataProcessor(seconds, on_val, off_val,
					pkts_val, crit_val);
			dp.calculate(trace_name, &dbif, &traceSec);
			traceSec.writeToFile(workspace.dir_xml() + "/" + ".sec" + ".xml");

			dp = DataProcessor(milliseconds, on_val, off_val,
					pkts_val, crit_val);
			dp.calculate(trace_name, &dbif, &traceMs);
			traceSec.writeToFile(workspace.dir_xml() + "/" + trace_name +".ms" + ".xml");

		}
		else if (workspace.database_version() == "2")
		{
			TraceDbManager database = TraceDbManager(workspace.database_v2());
			NetworkTrace traceSec = NetworkTrace();
			NetworkTrace traceMs = NetworkTrace();

			DataProcessor dp = DataProcessor(seconds, on_val, off_val,
					pkts_val, crit_val);
			dp.calculate_v2(trace_name, &database, &traceSec);
			traceSec.writeToFile(workspace.dir_xml() + "/" + trace_name + ".sec" + ".xml");

			dp = DataProcessor(milliseconds, on_val, off_val,
					pkts_val, crit_val);
			dp.calculate_v2(trace_name, &database, &traceMs);
			traceMs.writeToFile(workspace.dir_xml() + "/" + trace_name + ".ms" + ".xml");
		}

	} catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "trace-analyzer Error: " << e.error() << " for arg "
				<< e.argId() << std::endl;
	}

	return 0;
}

void cli_error_messege(std::string arg, std::string arg_name,
		std::string expected_values)
{
	std::cerr << "trace-analyzer Error: invalid argument " << arg << " for "
			<< arg_name << ". Expected " << expected_values
			<< ". Use --help for help." << std::endl;

}

bool cli_check_val(std::string val, std::vector<std::string> expectedVals)
{
	uint m = expectedVals.size();

	for (uint i = 0; i < m; i++)
		if (val == expectedVals[i])
			return (true);

	return (false);
}

