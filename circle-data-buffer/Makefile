CXX = g++

EXEC = run

SRC_BUFF = CircleDataBuffer.cpp
SRC_TEST = test.cpp

OBJ_BUFF = CircleDataBuffer.o

COMMON_LD_FLAG = -std=c++11 -O3 -pthread


test:
	$(CXX) $(SRC_BUFF) $(SRC_TEST) $(COMMON_LD_FLAG) -o $(EXEC)

buffer:
	$(CXX) $(SRC_BUFF) $(COMMON_LD_FLAG) -o $(OBJ_BUFF)

clean:
	rm -f $(EXEC) $(OBJ_BUFF)


