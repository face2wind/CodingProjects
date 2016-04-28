#ifndef _COMMAND_ANALYSER_HPP_
#define _COMMAND_ANALYSER_HPP_

#include <string>

enum CommandType
{
	COMMAND_TYPE_MIN = 0,

	COMMAND_TYPE_IS_SERVER = COMMAND_TYPE_MIN,
	COMMAND_TYPE_IS_CLIENT,
	COMMAND_TYPE_IS_SHOW_VERSION,
	COMMAND_TYPE_LIST_FILE,
	COMMAND_TYPE_GET_FILE,
	COMMAND_TYPE_SET_HOST,
	COMMAND_TYPE_SET_PORT,
	COMMAND_TYPE_SET_MULTIDOWNLOAD,

	COMMAND_TYPE_MAX
};

class CommandAnalyser
{
public:
	CommandAnalyser(int argc, char **argv);
	~CommandAnalyser();
	
    bool CheckAllParams();

	static void ShowHelp();

	const std::string GetStringParam(CommandType);
	int GetIntParam(CommandType);

	bool CheckCommandState(CommandType);

protected:
	bool m_command_state_list[COMMAND_TYPE_MAX + 1];
	std::string m_command_param_list[COMMAND_TYPE_MAX + 1];
};

#endif
