#ifndef SECONDARY_H
#define SECONDARY_H

#pragma once
#include "globals.h"


void parseInputs(int argc, char* argv[]);


/********************* COMMUNICATION AND HELPERS *********************/
void start_m();
void end_m(std::string str);
void start_time();
void end_time(std::string str);
void start_rounds();
void end_rounds(std::string str);
void aggregateCommunication();
void print_usage(const char * bin);
double diff(timespec start, timespec end);
void deleteObjects();
#endif