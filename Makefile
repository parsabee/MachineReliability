TARGET:=reliability
CXX:=c++
CXXFLAGS:=-std=c++14 -g
OBJECT:=main.o

all: $(TARGET)

reliability: $(OBJECT)
	$(CXX) $^ -o $@

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -O2 $^

clean:
	rm $(TARGET) $(OBJECT)
