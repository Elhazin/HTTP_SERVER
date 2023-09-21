#ifndef get_hpp
#define get_hpp



#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <sys/stat.h> 
#include <sys/types.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <map>
#include <netinet/tcp.h> 
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <sys/stat.h> 
#include <sys/types.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <map>
#include <netinet/tcp.h>
#include   "/nfs/homes/abouzanb/Desktop/HTTP_SERVER/HTTP_SERVER/includes/Servers.hpp"
#include "../includes/conf_parser.hpp"

class info{
	public :
	info(int sock)
	{
		socket = sock;
		readed = 0;
		size = 0;
		file = NULL;
		status = 0;
		anas = 0;

	}
	int anas;
	int socket;
	int size;
	int readed;
	int status;
	std::string path;
	std::string containte;
	std::ifstream *file;
	std::string buffer_to_send;
};

class method_get
{
	private :
	std::map<std::string, std::string> map;
	std::map<std::string, std::string> extansion_handling;
	std::string path;
	std::string route;
	std::string extansion;
	bool auto_index;
	std::vector<std::string> index;
	public :
	int size;
	std::ifstream *file;
	std::string url;
	info &infa;
	Directives keep;
	std::map<int , std::string> erros_page;
	
	method_get(Directives k, std::string l, info &inf);
	
    void get_allowed();
	void check_if_method_is_get();
	void get_check_path();
	void folder_handling();
	void file_handling();
	void handle_auto_index();
	void set_error_404();
	void set_error_403();
	void send_indexing(DIR *dir);
	void check_location();
	void set_error_500();
};


#endif