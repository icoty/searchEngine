INC_DIR:= ./include/
SRC_DIR:= ./src/
OBJS:= $(wildcard $(SRC_DIR)*.cc) $(wildcard $(SRC_DIR)*.cpp)

CXX:= g++
EXE:= ./bin/main.exe

LIBS:= -llog4cpp -lpthread
#-lregex
RM:= rm -rf

CXXFLAGS:= -Wall -g -std=c++11 $(addprefix -I ,$(INC_DIR)) $(LIBS)

$(EXE):$(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	$(RM) $(EXE) $(CLIENTEXE)

