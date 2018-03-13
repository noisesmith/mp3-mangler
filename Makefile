gtest-h = ../googletest/googletest/include
gtest-a = ../gtest-build/libgtest.a

mp3_parse.o: mp3_parse.c++ mp3_parse.hpp
	g++ -c --std=c++11 mp3_parse.c++

test: ex_test test.mp3
	./ex_test

test.mp3: sine.csd
	csound -W sine.csd && lame test.wav

ex_test: test_runner.o mp3_parse_test.o
	g++ -g --std=c++11 -isystem $(gtest-h) -pthread $^ $(gtest-a) -ljack -o $@

test_runner.o: test/test_runner.c++
	g++ -g -c --std=c++11 -isystem $(gtest-h) -pthread $<

mp3_parse_test.o: test/mp3_parse_test.c++ mp3_parse.c++ mp3_parse.hpp
	g++ -g -c --std=c++11 -isystem $(gtest-h) -pthread $<

clean:
	rm -f *.o ex_test test.wav test.mp3 dup.mp3
