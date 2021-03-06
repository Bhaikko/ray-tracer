CXX = g++
CPPFLAGS = -std=c++11 -Wall -g

LIB_CPPS = \
				./lib/sphere.cpp \
				./lib/moving_sphere.cpp \
				./lib/hittable_list.cpp \
				./lib/hittable.cpp \
				./lib/camera.cpp \
				./lib/material.cpp \
				./lib/texture.cpp \
				./lib/perlin.cpp \
				./lib/aabb.cpp \
				./lib/aarect.cpp \
				./lib/bvh.cpp \
				./lib/box.cpp \
				./lib/constant_medium.cpp \

SRCS_CPPS = \
				./src/main.cpp \

run:
	$(CXX) $(SRCS_CPPS) $(LIB_CPPS) -o ./build/tracer $(CPPFLAGS)