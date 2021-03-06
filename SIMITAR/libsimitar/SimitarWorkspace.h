/*
 * SimitarWorkspace.h
 *
 *  Created on: 26 de jul de 2017
 *      Author: anderson
 */

#ifndef SIMITARWORKSPACE_H_
#define SIMITARWORKSPACE_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>         // std::thread
#include <mutex>          // std::mutex

#include "Defines.h"
#include "PlogMacros.h"

// generic iostream mutex
extern std::mutex simitar_iostream_mutex;
// ethernet interface mutex
extern std::mutex simitar_eth_mutex;

class SimitarWorkspace
{
public:
	/**
	 * Store environment variables of simitar framework.
	 * The env variables must be set before using this class
	 */
	SimitarWorkspace();

	/**
	 *
	 */
	virtual ~SimitarWorkspace();

	/**
	 *
	 * @return
	 */
	std::string dir_workspace();

	/**
	 *
	 * @return
	 */
	std::string dir_test();

	/**
	 *
	 * @return
	 */
	std::string dir_xml();

	std::string database_version();

	/**
	 *
	 * @return
	 */
	std::string database_v1();

	/**
	 *
	 * @return
	 */
	std::string database_v2();

	/**
	 *
	 * @return
	 */
	std::string test_database();

	/***
	 *
	 * @return
	 */
	std::string test_ipmac_file();

	/**
	 *
	 * @return
	 */
	std::string test_ctd_xml();

	/**
	 *
	 * @return
	 */
	std::string test_ctd_xml_copy();

	/**
	 *
	 * @return
	 */
	std::string test_ctd_xml_copy2();

	/**
	 *
	 * @return
	 */
	std::string test_exponential_times();

	/**
	 *
	 * @return
	 */
	std::string test_uniform_times();

	/**
	 *
	 * @return
	 */
	std::string test_ctd_ipmac();

	/**
	 *
	 * @return
	 */
	std::string version_tag();

	/**
	 *
	 * @return
	 */
	std::string version_name();

	/**
	 *
	 */
	void test_();

private:

	char _dir_workspace[CHAR_BUFFER];
	char _dir_test[CHAR_BUFFER];
	char _dir_xml[CHAR_BUFFER];
	char _database_version[CHAR_BUFFER];
	char _database_v1[CHAR_BUFFER];
	char _database_v2[CHAR_BUFFER];
	//char _test_database[CHAR_BUFFER];
	char _test_ipmac_file[CHAR_BUFFER];
	char _test_ctd_xml[CHAR_BUFFER];
	char _test_ctd_xml_copy[CHAR_BUFFER];
	char _test_ctd_xml_copy2[CHAR_BUFFER];
	char _test_exponential_times[CHAR_BUFFER];
	char _test_uniform_times[CHAR_BUFFER];
	char _test_ctd_ipmac_file[CHAR_BUFFER];
	char _version_tag[CHAR_BUFFER];
	char _version_name[CHAR_BUFFER];

	void get_env_var(char varDst[], const char varName[]);

};

#endif /* SIMITARWORKSPACE_H_ */
