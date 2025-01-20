.PHONY: all clean

all:
	mkdir -p ./bin
	g++ -g ./src/*.cpp -o ./bin/run

clean:
	rm -f ./bin/run
	find -name "*~" -exec rm {} \;
