#ifndef _MYSH_HPP
#define _MYSH_HPP



#include <iostream>
#include <vector>

#include "tokenizer.hpp"
#include "builtin.hpp"

using namespace std;

#define DELIM 					" \n\t\r\a\0"
#define SPECIAL_DELIM			"|&;()<>"



class Command {
public:
	Command(void);
	~Command(void);
	friend ostream& operator<<(ostream& os, Command& cmd);

	vector<string> argv;
	bool bg_flag;
};

extern Tokenizer				g_tokenizer;



string readLine(void);
void parseLine(string& line, vector<Command>& output);
int executeCommand(Command const& cmd);
int executeSystem(Command const& cmd);



#endif //_MYSH_HPP