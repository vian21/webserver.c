FLAGS:=-Wall -Werror -Wpedantic

all: webserver run

webserver: main.c library.o
	gcc ${FLAGS} $^ -o $@
	
library.o: library.c
	gcc  -c ${FLAGS} $^ -o $@

run: webserver
	@./$^

clean:
	@echo "Deleting executables"
	@find . -maxdepth 1 -type f -executable -delete
	@rm *.o
