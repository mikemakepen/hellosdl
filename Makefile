CC :=gcc
CFLAG := -g
FILETYPE := c
DIRS := ./
SRC := $(foreach dir, $(DIRS), $(wildcard $(dir)*.$(FILETYPE)))
OBJ := $(patsubst %.$(FILETYPE), %.o, $(SRC))
TARGET := hellosdl
THISLIB :=  -lSDL2

$(TARGET) : $(OBJ) 
	$(CC) $(CFLAG)  -o $@ $^  $(THISLIB) 
%.o:%.$(FILETYPE)
	$(CC) $(CFLAG)  -c $^ -o $@

.PHONY:clean
clean:
	-$(RM) $(OBJ) $(TARGET)
