CC=gcc
APP=user_time

SOURCE=$(APP).c

$(APP): $(SOURCE)
	$(CC) $(SOURCE) -o $(APP)

.PHONY : run
run: $(APP)
	./$(APP)

.PHONY : clean
clean: 
	rm -rf *.o $(APP)
