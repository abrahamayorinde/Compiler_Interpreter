MAKEFILE INSTRUCTIONS:
CC = g++

CFLAGS =-c -Wall

SOURCE = activationrecordcreator.cpp activation_record.cpp
SOURCE += symboltablecreator.cpp parser.cpp miscellaneous.cpp lexer.cpp main.cpp Node.cpp symbol_table.cpp

HEADER = activationrecordcreator.h ActivationRecordType.h typedefarguments.h activation_record.h 
HEADER += symboltablecreator.h parser.h lexer.h miscellaneous.h Node.h symbol_node.h symbol_table.h

OBJS:=$(SOURCE:%.c=%.o)

TARGET = BASIC_COMPILER_X

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: $(SOURCE) $(HEADER)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o