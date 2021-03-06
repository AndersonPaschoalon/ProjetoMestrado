/*
 * SimitarWorkspace.cpp
 *
 *  Created on: 27 de jul de 2017
 *      Author: anderson
 */

#include "SimitarWorkspace.h"

// generic iostream mutex
std::mutex simitar_iostream_mutex;
// ethernet interface mutex
std::mutex simitar_eth_mutex;

SimitarWorkspace::SimitarWorkspace()
{
	get_env_var(_dir_workspace, "SIMITAR_WORKSPACE");
	get_env_var(_dir_test, "SIMITAR_TEST_DATA_DIR");
	get_env_var(_dir_xml, "SIMITAR_CTD_XML_DIR");
	get_env_var(_database_version, "SIMITAR_DATABASE_VERSION");
	get_env_var(_database_v1, "SIMITAR_DATABASE_V1");
	get_env_var(_database_v2, "SIMITAR_DATABASE_V2");
	get_env_var(_test_ctd_xml, "SIMITAR_TEST_CTD_XML");
	get_env_var(_test_ctd_xml_copy, "SIMITAR_TEST_COPY_CTD_XML");
	get_env_var(_test_ctd_xml_copy2, "SIMITAR_TEST_COPY2_CTD_XML");
	get_env_var(_test_exponential_times, "SIMITAR_TEST_EXPONENTIAL_TIMES");
	get_env_var(_test_uniform_times, "SIMITAR_TEST_UNIFORM_TIMES");
	get_env_var(_test_ipmac_file, "SIMITAR_TEST_IPMAC");
	get_env_var(_test_ctd_ipmac_file, "SIMITAR_TEST_CTD_XML_IPMAC");
	get_env_var(_version_tag, "SIMITAR_VERSION_TAG");
	get_env_var(_version_name, "SIMITAR_VERSION_NAME");
	get_env_var(_version_name, "SIMITAR_VERSION_NAME");
}

SimitarWorkspace::~SimitarWorkspace()
{
	// Nothing to do
}

void SimitarWorkspace::get_env_var(char varDst[], const char varName[])
{
	if (const char* env_p = std::getenv(varName))
	{
		strcpy(varDst, env_p);
	}
	else
	{
		PLOG_ERROR << ERRORMSG_CANNOT_READ_ENVIMENMENT_VARIABLE << varName;
		exit(ERROR_CANNOT_READ_ENVIMENMENT_VARIABLE);
	}
}

std::string SimitarWorkspace::dir_workspace()
{
	return std::string(_dir_workspace);
}

std::string SimitarWorkspace::dir_test()
{
	return std::string(_dir_test);
}

std::string SimitarWorkspace::dir_xml()
{
	return std::string(_dir_xml);
}

std::string SimitarWorkspace::database_v1()
{
	return std::string(_database_v1);
}

std::string SimitarWorkspace::database_v2()
{
	return std::string(_database_v2);
}

std::string SimitarWorkspace::test_database()
{
	return std::string(_database_v1);
}

std::string SimitarWorkspace::test_ipmac_file()
{
	return std::string(_test_ipmac_file);
}

std::string SimitarWorkspace::test_ctd_xml()
{
	return std::string(_test_ctd_xml);
}

std::string SimitarWorkspace::test_ctd_xml_copy()
{
	return std::string(_test_ctd_xml_copy);
}

std::string SimitarWorkspace::test_ctd_xml_copy2()
{
	return std::string(_test_ctd_xml_copy2);
}

std::string SimitarWorkspace::test_exponential_times()
{
	return std::string(_test_exponential_times);
}

std::string SimitarWorkspace::test_uniform_times()
{
	return std::string(_test_uniform_times);
}

std::string SimitarWorkspace::test_ctd_ipmac()
{
	return std::string(_test_ctd_ipmac_file);
}

std::string SimitarWorkspace::version_tag()
{
	return std::string(_version_tag);
}

std::string SimitarWorkspace::version_name()
{
	return std::string(_version_name);
}

std::string SimitarWorkspace::database_version()
{
	return std::string(_database_version);
}

void SimitarWorkspace::test_()
{
	std::cout << "_dir_workspace:" << _dir_workspace << std::endl;
	std::cout << "_dir_test:" << _dir_test << std::endl;
	std::cout << "_dir_xml:" << _dir_xml << std::endl;
	std::cout << "_database_v1:" << _database_v1 << std::endl;
	std::cout << "_database_v2:" << _database_v2 << std::endl;
	std::cout << "_test_ipmac_file:" << _test_ipmac_file << std::endl;
	std::cout << "_test_ctd_xml:" << _test_ctd_xml << std::endl;
	std::cout << "_test_ctd_xml_copy:" << _test_ctd_xml_copy << std::endl;
	std::cout << "_test_ctd_xml_copy2:" << _test_ctd_xml_copy2 << std::endl;
	std::cout << "_test_exponential_times:" << _test_exponential_times
			<< std::endl;
	std::cout << "_test_uniform_times:" << _test_uniform_times << std::endl;

}


