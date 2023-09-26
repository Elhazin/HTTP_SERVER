NAME = webserv

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

SRCS = parse_conf/check_directives.cpp \
	parse_conf/check_location.cpp \
	parse_conf/check_servers.cpp \
	parse_conf/conf_parser.cpp \
	parse_conf/utils.cpp \
	parse_conf/Directives.cpp \
	parse_conf/Locations.cpp \
	parse_conf/Servers.cpp \
	methods/get.cpp methods/handle_file.cpp methods/send_errors.cpp methods/handle_folder.cpp  methods/delete.cpp 

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

CONF = /nfs/homes/abouzanb/Desktop/webservee/conf/default.conf

$(NAME): $(OBJS) $(CONF)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY : all clean fclean re run
