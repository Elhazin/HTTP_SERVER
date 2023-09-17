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
#include "/nfs/homes/abouzanb/Desktop/workingnow/methods/get.hpp"
#define P 8080
 




void method_get::set_error_403()
{
	this->file = new std::ifstream(path.c_str(),  std::ios::binary);
	std::stringstream ss ;
	std::string ext;
	ext = extansion_handling["403"];
	ss << this->size;
	ext += ss.str();
	ext += "\r\n";
	ext += "\r\n";
	this->infa.buffer_to_send = ext;
	this->infa.status = 1;
	infa.file = file;
}


void method_get::file_handling()
{
	this->file = new std::ifstream(path.c_str());
	std::stringstream ss ;
	std::string ext;
	size_t i = path.find_last_of(".");
	if (i == std::string::npos)
		ext = ".html";
	ext =  path.substr(i, path.size());
	std::string extansion =  extansion_handling[ext];
	ss << this->size;
	extansion += ss.str();
	extansion += "\r\n";
	extansion += "\r\n";
	this->infa.buffer_to_send = extansion;
	this->infa.status = 1;
	infa.file = file;
}

void method_get::set_error_404()
{
	struct stat st;
	this->file = new std::ifstream(path.c_str(),  std::ios::binary);
	stat(path.c_str() , &st);
	this->size = st.st_size;
	infa.size = st.st_size;
	std::stringstream ss ;
	std::string ext;
	ext = extansion_handling["404"];
	ss << this->size;
	ext += ss.str();
	ext += "\r\n";
	ext += "\r\n";
	this->infa.buffer_to_send = ext;
	this->infa.status = 1;
	infa.file = file;
}

// CRETAE THE FILE THAT WOULD CONTAING THE INDEIND OF THE FILE 
void method_get::send_indexing(DIR *dir)
{
	
	struct dirent *dp;
	std::ofstream  fill("/nfs/homes/abouzanb/Desktop/workingnow/indexing.html");
    if (!fill.is_open())   
    {
        std::cout << "Error opening file";
        exit(1);
    }

	fill << "<html>\n<head>\n<title>Index of /</title>\n</head>\n<body bgcolor=\"white\">\n<h1>Index of /</h1><hr><pre>\n";

	while ((dp = readdir(dir)) != NULL)
	{
		fill << "<a href=\"" << dp->d_name << "\">" << dp->d_name << "</a>\n";
	}

	this->path = "/nfs/homes/abouzanb/Desktop/workingnow/indexing.html";

	fill << "</pre><hr>\n</body>\n</html>\n";

	closedir(dir);
	fill.close();
	file_handling();
}

// HANDLE AUTO INDEX : READING DIR AND SETTING IT IN A FILE 
void method_get::handle_auto_index()
{
	DIR *dir;
	if ((dir = opendir(path.c_str())) == NULL)
	{
		this->path = route + "/Desktop/webserver/webserve/forbi.html";
		set_error_404();
	}
	else
		send_indexing(dir);
}


void method_get::get_path(std::string r)
{
	this->path = r;
}


// this function will check is there any auto index or index in the location
void method_get::folder_handling()
{
	struct stat st;
	size_t i = 0;	
	while (i < this->index.size())
	{
		std::string temp = path;
		temp += index[i];
		if (stat(temp.c_str() , &st) == 0)
		{
			this->size = st.st_size;
			infa.size = st.st_size;
			this->path = temp;
			file_handling();
			return ;
		}
		i++;
	}


	if (this->auto_index == true)
	{
		handle_auto_index();
	
	}else
	{
		std::cout << "Iam here in the folder handling 2" << std::endl;
		this->path = "/nfs/homes/abouzanb/Desktop/webserver/webserve/forbi.html";
		set_error_403();
	}
}



void method_get::check_if_method_is_get()
{
	size_t i = 0;
	while (i < keep.getLocationsVec().size())
	{

		if (keep.getLocationsVec()[i].getLocation() == url)
		{
			if (keep.getLocationsVec()[i].getAcceptedMethods()["GET"] == true)
			{
				this->auto_index = keep.getLocationsVec()[i].getAutoIndex();
				this->index = keep.getLocationsVec()[i].getIndex();
				return ;
			}
			else
			{
				this->path = "/nfs/homes/abouzanb/Desktop/webserver/webserve/forbi.html";
				set_error_403();
				throw std::exception();
			}
		}	
		i++;
	}
	if (i == keep.getLocationsVec().size() || i == 0) {
		this ->auto_index = keep.getAutoIndex();
		this->index = keep.getIndex();
	}
}


void method_get::get_check_path()
{

	struct stat st;
	if (stat(path.c_str() , &st) == 0)
	{
		this->size = st.st_size;
		infa.size = st.st_size;
		if (S_ISDIR(st.st_mode))
		{
			try {
				check_if_method_is_get();
				folder_handling();
			}
			catch (const std::exception& e)
			{
			}
		}
		else
			file_handling();
	}
	else 
	{
		this->path = "/nfs/homes/abouzanb/Desktop/workingnow/default_error_pages/401_unauthorized.html";
		set_error_404();
	}
}


method_get::method_get(Directives k , std::string url, info &inf) : infa(inf)
{
	this->keep = k;
	std::string route = k.getRoot();
	this->path = route + url;
	this->url = url;
	std::string moth = "HTTP/1.1 200 OK\n";
	extansion_handling[".html"] = moth + "Content-Type: text/html\nContent-Length: ";
	extansion_handling[".css"] = moth + "Content-Type: text/css\nContent-Length: ";
	extansion_handling[".js"] = moth + "Content-Type: text/javascript\nContent-Length: ";
	extansion_handling[".jpg"] = moth + "Content-Type: image/jpeg\nContent-Length: ";
	extansion_handling[".jpeg"] = moth + "Content-Type: image/jpeg\nContent-Length: ";
	extansion_handling[".png"] = moth + "Content-Type: image/png\nContent-Length: ";
	extansion_handling[".gif"] = moth + "Content-Type: image/gif\nContent-Length: ";
	extansion_handling[".mp4"] = moth + "Content-Type: video/mp4\nContent-Length: ";
	extansion_handling[".mp3"] = moth + "Content-Type: audio/mpeg\nContent-Length: ";
	extansion_handling[".pdf"] = moth + "Content-Type: application/pdf\nContent-Length: ";
	extansion_handling[".txt"] = moth + "Content-Type: text/plain\nContent-Length: ";
	extansion_handling["404"] = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
	extansion_handling["403"] = "HTTP/1.1 403 Forbidden\nContent-Type: text/html\nContent-Length: ";
	std::cout << "Iam here in the constructor" << std::endl;
	get_check_path();
}


void ft_get(Directives data, std::string url,  info &socket)
{
	method_get get(data,url,  socket);
}


int main()
{
	try {
	std::ifstream conf("/nfs/homes/abouzanb/Desktop/workingnow/conf/default.conf");
		Servers 	servers;
		skipEmptyLinesAndCheckServerBlock(conf, true, servers);
		parse_servers(servers);
	int sending = 0;
    Directives a = servers.getServersVec()[0];
	char bu[1024];
	std::map<std::string, std::string> data;
	std::vector<info> clientes;
	struct sockaddr_in addr;
	fd_set readfds,  writefds;
	int socket_cl; 
	int opt = 1;
	signal(SIGPIPE, SIG_IGN);

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	int server = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(P);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(server, (sockaddr *)&addr, sizeof(addr));
	listen(server, 20);

	data["rote"] = a.getRoot();

    if (a.getAutoIndex() == true )
        std::cout << "akojha   aloha "<< std::endl;
    FD_SET(server, &readfds);
	FD_SET(server, &writefds);
	int max_fd = server;
	while (1)
	{
	 fd_set temp_read = readfds; 
	 fd_set temp_write = writefds;
	 int ret = select(max_fd + 1, &temp_read, &temp_write, NULL, NULL);
	 if (ret < 0)
	 {
		 std::cout << "error" << std::endl;
		 exit(1);
	 }
	 if (FD_ISSET(server, &temp_read))
	 {
		socket_cl = accept(server , 0, 0);
		if (socket_cl > max_fd)
			max_fd = socket_cl;
		FD_SET(socket_cl, &temp_read);
		FD_SET(socket_cl, &readfds);
		FD_SET(socket_cl, &writefds);
		FD_SET(socket_cl, &temp_write);
		clientes.push_back(info(socket_cl));
	}
	
	for (size_t i = 0; i < clientes.size(); i++)
	{
		if (FD_ISSET(clientes[i].socket, &temp_read))
		{
            char bu[900000];
		      int b = recv(clientes[i].socket, bu, 1024, 0);
	    	  bu[b] = '\0';
		  std::string s = bu;
          std::ofstream d("./test.txt");
          d << s;

		  try {
		  data["path"] = s.substr(4, s.find("HTTP") - 5);
		  ft_get(a, data["path"], clientes[i]);
		  FD_CLR(clientes[i].socket, &readfds);
		  }
		  catch (const std::exception& e)
		  {
			    FD_CLR(clientes[i].socket, &readfds);
			
			    FD_CLR(clientes[i].socket, &writefds);
			    close(clientes[i].socket);
			    clientes.erase(clientes.begin() + i);
		  	delete clientes[i].file;
		  }
		}
		else if (FD_ISSET(clientes[i].socket, &temp_write))
		{
			bzero(bu,sizeof(bu));
			if (!clientes[i].file->eof())
			{
				if (clientes[i].status == 1)
				{
					std::cout << "sending header" << std::endl;
					send(clientes[i].socket, clientes[i].buffer_to_send.c_str(), clientes[i].buffer_to_send.size(), 0);
					clientes[i].status = 0;
				}
				else 
				{

				std::cout << "sending" << std::endl;
				clientes[i].file->read(bu, 1024);
				int count = clientes[i].file->gcount();
				int sended = send(clientes[i].socket, bu, count, 0);
				if (sended < 1)
				{
					clientes[i].file->close();
					FD_CLR(clientes[i].socket, &writefds);
					close(clientes[i].socket);
					clientes.erase(clientes.begin() + i);
				}
				sending += sended;
				clientes[i].readed += count;
				}
			}
			else 
			{
				std::cout << "closing" << std::endl;
				clientes[i].file->close();
			  	FD_CLR(clientes[i].socket, &writefds);
				close(clientes[i].socket);
				delete clientes[i].file;
			  	clientes.erase(clientes.begin() + i);

			}
		}
	}
	}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}