CC=gcc
APP=user_memory

SOURCE=$(APP).c

$(APP): $(SOURCE)
	$(CC) $(SOURCE) -o $(APP)

.PHONY : run
run: $(APP)
	./$(APP)

.PHONY : clean
clean: 
	rm -rf *.o $(APP)