.PHONY: all clean

all:
	g++ *.cpp -o run

clean:
	rm -f run
	rm -f *~
