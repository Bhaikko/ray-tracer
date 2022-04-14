CXX = g++
CPPFLAGS = -std=c++11 -Wall -g

LIB_CPPS = \
				./lib/sphere.cpp \
				./lib/hittable_list.cpp \

SRCS_CPPS = \
				./src/main.cpp \

run:
	$(CXX) $(SRCS_CPPS) $(LIB_CPPS) -o ./build/tracer $(CPPFLAGS)