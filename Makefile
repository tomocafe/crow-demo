CXX := clang++
CC := g++
BOOST := boost_1_78_0
CPPFLAGS := -I$(BOOST)
LDFLAGS := -lpthread

SRCS := main.cpp
OBJS := $(subst .cpp,.o,$(SRCS))

server: $(OBJS) crow_all.h boost_1_78_0
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

boost_1_78_0:
	wget https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.bz2 && tar xjvf $(BOOST).tar.bz2 && rm $(BOOST).tar.bz2

crow_all.h:
	wget https://github.com/CrowCpp/Crow/releases/download/v0.3%2B4/crow_all.h

clean:
	rm -f server $(OBJS)

.PHONY: clean
