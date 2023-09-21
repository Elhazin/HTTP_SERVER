#include "get.hpp"



void method_get::set_error_403()
{
    this ->path  = erros_page[403];
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
	this ->path  =erros_page[404];
	this->file = new std::ifstream(path.c_str(),  std::ios::binary);
	if (file->fail())
	{
		std::cout << this ->path << std::endl;
		std::cout << "error" << std::endl;
	}
	struct stat st;
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
