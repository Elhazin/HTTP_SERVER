#include "get.hpp"
#define P 8080
 
void method_get::get_allowed()
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
				set_error_403();
				throw std::exception();
			}
		}	
		i++;
	}
}

void method_get::check_if_method_is_get()
{
	this->get_allowed();
	this ->auto_index = keep.getAutoIndex();
	std::cout << this->auto_index << std::endl;
	this->index = keep.getIndex();
}

void check_location()
{
	size_t i = 0;
	int max_lenght = 0;
	int size = 0;
	while (i < keep.getLocationsVec().size())
	{
		size = keep.getLocationsVec()[i].getLocation().size();
		if (keep.getLocationsVec()[i].getLocation().size() >= url.size() && keep.getLocationsVec()[i].getLocation() == url.substr(0, size) && size > max_lenght)
		{
			max_lenght = size;
			this->auto_index = keep.getLocationsVec()[i].getAutoIndex();
			this->index = keep.getLocationsVec()[i].getIndex();
			this->route = keep.getLocationsVec()[i].getRoot();
			if (keep.getLocationsVec()[i].getAcceptedMethods()["GET"] == true)
			{
				this->auto_index = keep.getLocationsVec()[i].getAutoIndex();
				this->index = keep.getLocationsVec()[i].getIndex();
				return ;
			}
			else
			{
				set_error_403();
				throw std::exception();
			}
		}
		i++;
	}
	set_error_404();
	throw std::exception();
}

void method_get::get_check_path()
{
	struct stat st;
	if (stat(path.c_str() , &st) == 0)
	{
		try{
			check_location();
			this->size = st.st_size;
			infa.size = st.st_size;
			if (S_ISDIR(st.st_mode))
			{
				
				check_if_method_is_get();
				folder_handling();
				
			}
			else
				file_handling();
		}
		catch (const std::exception& e)
		{
		}
	}
	else 
	{
		set_error_404();
	}
}


method_get::method_get(Directives k , std::string url, info &inf) : infa(inf)
{
	this->keep = k;
	std::string route = k.getRoot();
	this->path = route + url;
	this->url = url;
	infa.path = path;
	this->erros_page = k.getErrorPages();
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
	get_check_path();
}


void ft_get(Directives data, std::string url,  info &socket)
{
	method_get get(data,url,  socket);
}

void close_socket(std::vector<int> &clients, int& i , fd_set &writefds)
{
		clientes[i].file->close();
		FD_CLR(clientes[i].socket, &writefds);
		close(clientes[i].socket);
		delete clientes[i].file;
		clientes.erase(clientes.begin() + i);
}

void get_response(std::vector<int> &clients, int& i , fd_set &writefds)
{
	char bu[1025];
	if (!clientes[i].file->eof())
	{
		if (clientes[i].status == 1)
		{
			if (send(clientes[i].socket, clientes[i].buffer_to_send.c_str(), clientes[i].buffer_to_send.size(), 0) <= 0)
			{
				close_socket(clients, i, writefds);
				return ;
			}
			clientes[i].status = 0;
		}
		clientes[i].file->read(bu, 1024);
		int count = clientes[i].file->gcount();
		if (count <= 0)
		{
			close_socket(clients, i, writefds);
			return ;
		}
		if (send(clientes[i].socket, bu, count, 0) <= 0)
		{
			close_socket(clients, i, writefds);
			return ;
		}		
	}
	else
		close_socket(clients, i, writefds);
}

int main()
{ 
        std::cout << "akojha   aloha "<< std::endl;
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
			if (clientes[i].anas == 1)
			{
			std::cout << "reading" << std::endl;
				FD_CLR(clientes[i].socket, &readfds);
				FD_CLR(clientes[i].socket, &temp_read);
				continue ;
			}
            char bu[1025];
		      int b = recv(clientes[i].socket, bu, 1024, 0);
	    	  bu[b] = '\0';
		  std::string s;
		  s = bu;
		  try {
		  data["path"] = s.substr(4, s.find("HTTP") - 5);
		  std::cout << i << std::endl;
		  ft_get(a, data["path"], clientes[i]);
		  clientes[i].anas = 1;
		  FD_CLR(clientes[i].socket, &readfds);
		  FD_CLR(clientes[i].socket, &temp_read);
		  }
		  catch (std::exception &r){
			FD_CLR(clientes[i].socket, &readfds);
				clientes[i].file->close();
			  	FD_CLR(clientes[i].socket, &writefds);
				close(clientes[i].socket);
				delete clientes[i].file;
			  	clientes.erase(clientes.begin() + i);
		  }

		}
		else if (FD_ISSET(clientes[i].socket, &temp_write))
		{
			bzero(bu,sizeof(bu));
			if (!clientes[i].file->is_open())
			{
				clientes[i].file = new std::ifstream(clientes[i].path.c_str(), std::ios::binary);
			}
			get_response(clientes, i, writefds, sending, bu)
			if (!clientes[i].file->eof())
			{
				if (clientes[i].status == 1)
				{
					send(clientes[i].socket, clientes[i].buffer_to_send.c_str(), clientes[i].buffer_to_send.size(), 0);
					clientes[i].status = 0;
				}

				std::cout << "sending" << std::endl;
				clientes[i].file->read(bu, 1024);
				int count = clientes[i].file->gcount();
				int sended = send(clientes[i].socket, bu, count, 0);
				if (sended <= 0)
				{
					clientes[i].file->close();
					FD_CLR(clientes[i].socket, &writefds);
					close(clientes[i].socket);
					clientes.erase(clientes.begin() + i);
				}
				sending += sended;
				clientes[i].readed += count;
				
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