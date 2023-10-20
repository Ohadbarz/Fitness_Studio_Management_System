bin/studio: src/Trainer.cpp src/Customer.cpp src/Workout.cpp src/Action.cpp src/Studio.cpp include/Trainer.h include/Customer.h include/Workout.h include/Action.h include/Studio.h
	g++ -g -Wall -std=c++11 -o bin/studio src/main.cpp src/Trainer.cpp src/Customer.cpp src/Workout.cpp src/Action.cpp src/Studio.cpp
#
run: bin/studio
	./bin/studio ./ExampleInput.txt
# clean build files
clean:
	@rm -f ./bin/*
#
check: bin/studio
	valgrind --leak-check=full --track-origins=yes ./bin/studio ./ExampleInput.txt
all: bin/studio
