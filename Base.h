#ifndef HWZ_SERVERDFC_Base_H
#define HWZ_SERVERDFC_CBase_H

#include <stdio.h>
#include <string>
#include <syslog.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


void TestFileInDirectory();
void GetFileInDirectory(const std::string &strDir,std::string &strOutFiles);
void TestFileInDirectoryRecursion();
void GetFileInDirectoryRecursion(const std::string &strDir,std::string &strOutFiles);
void TestFileType();
void GetFileType(const std::string &strDir,std::string &strOutType);

void TestTempFile();
void TempFile();

void TestFork();

void TestSetPPID_Init();

void TestOrphanChild();

int daemonize();
void daemon_exit(int iFlage);
#endif