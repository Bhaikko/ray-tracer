CXX = g++
CPPFLAGS = -std=c++11 -Wall -g

LIB_CPPS = \
				./lib/sphere.cpp \
				./lib/moving_sphere.cpp \
				./lib/hittable_list.cpp \
				./lib/camera.cpp \
				./lib/material.cpp \
				./lib/texture.cpp \
				./lib/aabb.cpp \
				./lib/bvh.cpp \

SRCS_CPPS = \
				./src/main.cpp \

run:
	$(CXX) $(SRCS_CPPS) $(LIB_CPPS) -o ./build/tracer $(CPPFLAGS)