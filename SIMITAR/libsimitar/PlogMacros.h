/*
 * PlogMacros.h
 *
 *  Created on: 24 de jul de 2017
 *      Author: anderson
 */

#ifndef PLOGMACROS_H_
#define PLOGMACROS_H_

#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>
#include <plog/Init.h>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>



/**
 *  Loglevel list for PLOG_INIT
enum Severity
{
    none = 0,   //!< none
    fatal = 1,  //!< fatal
    error = 2,  //!< error
    warning = 3,//!< warning
    info = 4,   //!< info
    debug = 5,  //!< debug
    verbose = 6 //!< verbose
};
 */
#define PLOG_INIT(logLevel)\
		char* _plog_logfile_env = std::getenv("SIMITAR_LOGFILE");\
		char _fileName[500];\
		if(_plog_logfile_env)\
		{ \
			strcpy(_fileName, _plog_logfile_env);\
			static plog::RollingFileAppender<plog::CsvFormatter> fileAppender(_fileName,\
					800000, 2);\
			static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;\
			plog::init(plog::logLevel, &fileAppender).addAppender(&consoleAppender); \
		}\
		else\
		{\
			static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;\
			plog::init(plog::logLevel, &consoleAppender);\
			LOG_ERROR  << "[" << __FILE__ << "] " << "Can't find env variable SIMITAR_LOGFILE";\
		}

#define PLOG_NONE LOG_NONE  << "[" << __FILE__ << "] "
#define PLOG_FATAL LOG_FATAL  << "[" << __FILE__ << "] "
#define PLOG_ERROR LOG_ERROR  << "[" << __FILE__ << "] "
#define PLOG_WARN LOG_WARNING  << "[" << __FILE__ << "] "
#define PLOG_INFO LOG_INFO  << "[" << __FILE__ << "] "
#define PLOG_DEBUG LOG_DEBUG  << "[" << __FILE__ << "] "
#define PLOG_VERBOSE LOG_VERBOSE  << "[" << __FILE__ << "] "

#define SUCCESS 									0	/* Success*/
#define ERROR_INVALID_ASSIGNMENT 					1	/* Invalid atribuition*/
#define ERROR_ENOMEM 								2	/* Out of memory */
#define ERROR_DIVISION_BY_ZERO 						3	/* Try to divide by zero */
#define ERROR_BAD_VALUE  							4	/* Invalid value or argument*/
#define ERROR_NULL_POINTER 							5	/* Memory not allocated */
#define ERROR_GENERAL_EXECUTION_ERROR 				9	/* Generic execution error */
#define ERROR_CANNOT_READ_FILE 						10	/* Can't find/read file */
#define ERROR_CANNOT_READ_ENVIMENMENT_VARIABLE 		11	/* Can't find env variable*/
#define ERROR_CANNOT_WRITE_FILE 					12	/* Can't write file */
#define ERROR_GENERAL_IO_ERROR 						19	/* Generic I/O error */
#define ERROR_CANNOT_CREATE_THREAD 					20	/* Can't create a new thread*/
#define ERROR_CANNOT_CREATE_PROCESS 				21	/* Can't create a new process*/
#define ERROR_EACCES 								22	/* Permission denied on performing operation*/
#define ERROR_GENERAL_OS_ERROR						29	/* Operational system generic error*/

#define ERRORMSG_INVALID_ASSIGNMENT 					"Invalid attribution: "
#define ERRORMSG_ENOMEM 								"Out of memory: "
#define ERRORMSG_DIVISION_BY_ZERO 						"Try to divide by zero: "
#define ERRORMSG_BAD_VALUE  							"Invalid value or argument: "
#define ERRORMSG_NULL_POINTER 							"Memory not allocated: "
#define ERRORMSG_GENERAL_EXECUTION_ERROR 				"Generic execution error: "
#define ERRORMSG_CANNOT_READ_FILE 						"Can't find/read file: "
#define ERRORMSG_CANNOT_READ_ENVIMENMENT_VARIABLE 		"Can't find env variable: "
#define ERRORMSG_CANNOT_WRITE_FILE 						"Can't write file: "
#define ERRORMSG_GENERAL_IO_ERROR 						"Generic I/O error: "
#define ERRORMSG_CANNOT_CREATE_THREAD 					"Can't create a new thread: "
#define ERRORMSG_CANNOT_CREATE_PROCESS 					"Can't create a new process: "
#define ERRORMSG_EACCES 								"Permission denied on performing operation: "
#define ERRORMSG_GENERAL_OS_ERROR						"Operational system generic error: "

#define WAIT_KEY printf("\npress ENTER to continue\n"); getchar();

#endif /* PLOGMACROS_H_ */
