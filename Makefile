all: serialGaussian parallelGaussian

serialGaussian:
	g++ -std=c++11 -o serialGaussian Source.cpp -L/usr/X11R6/lib -lm -lpthread -lX11
parallelGaussian:
	g++ -std=c++11 -o parallelGaussian ParallelGaussian.cpp -L/usr/X11R6/lib -lm -lpthread -lX11

