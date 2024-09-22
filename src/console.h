#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "column.h"
#include "location-list.h"
#include "unit.h"
#include "query.h"

extern std::string save_location; /* File generated relative to current directory*/
extern std::string help; /* User manual*/
extern std::string location_list_header; /* Column names display on command line*/

namespace cli {

std::string get_option(std::vector<std::string> args, std::string option);

/* Confirms existence of option */
bool has_option(std::vector<std::string> args, std::string option);

void write(std::string path, std::string content);

/* Processes query related commands & performs actions based on subcommand */
void handle_query_command(std::vector<std::string> args);

/* Processes location related commands & performs actions based on subcommand */
void handle_location_command(std::vector<std::string> args);

enum ExecStatus { Continue, Stop };

/* Generic parameters that apply to multiple commands*/
ExecStatus handle_generic_parameters(std::vector<std::string> args);

/* 'wda query' (query - subcommand) 'wda locations' (locations-subcommand)*/
std::vector<std::string>::iterator find_subcommand_start(std::vector<std::string> &args);

void usage();

/* Uses all the previous namespace cli functions
to parse and process command-line arguments
& execute corresponding command handlers*/
int handle_command(int argc, char *argv[]);

} // namespace cli
