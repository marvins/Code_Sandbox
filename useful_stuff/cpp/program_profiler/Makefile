

BOOST_INC= -I/opt/local/include 
BOOST_LIB= -L/opt/local/lib -lboost_date_time
BOOST= $(BOOST_INC) $(BOOST_LIB)


demo: demo.cpp Profiler.o Profiler.hpp
	g++ demo.cpp -o demo Profiler.o $(BOOST)


Profiler.o: Profiler.cpp Profiler.hpp
	g++ $< -c -o $@  $(BOOST_INC)


