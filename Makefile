.PHONY: all clean

all:
	g++ -g *.cpp -o run

clean:
	rm -f run
	rm -f *~
