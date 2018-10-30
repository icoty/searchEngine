INC_DIR:= ./include  ./include/threadpool ./include/net ./include/log ./include/cppjieba
SRC_DIR:= ./src/ ./src/threadpool ./src/net ./src/log

OBJS:= $(wildcard ./src/*.cpp) $(wildcard ./src/net/*.cpp) $(wildcard ./src/threadpool/*.cpp) $(wildcard ./src/log/*.cpp)
INC_FILE:= $(addprefix -I ,$(INC_DIR))

CXX:= g++
CXXFLAGS:= -std=c++11 -g -Wno-deprecated
LIBS:= -llog4cpp -lpthread -ljsoncpp
RM:= rm -rf

TARGET:= ./bin/main.exe
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INC_FILE)  $(LIBS)

clean:
	$(RM) $(TARGET)
