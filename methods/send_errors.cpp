#include "/nfs/homes/abouzanb/Desktop/workingnow/methods/get.hpp"



void method_get::set_error_403()
{
    this ->path  = "/nfs/homes/abouzanb/Desktop/webserver/webserve/forbi.html";
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

void method_get::set_error_404()
{
    this ->path  = "/nfs/homes/abouzanb/Desktop/webserver/webserve/forbi.html";
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
