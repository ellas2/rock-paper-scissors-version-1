COMP = g++-5.3.0
#COMP = g++
#put all your object files here
OBJS =  main.o RPSGame.o GamePiece.o Player.o RPSMessage.o
#The executabel filename DON'T CHANGE
EXEC = ex1
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG



$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies

RPSMessage.o: RPSMessage.cpp RPSMessage.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Player.o: Player.cpp Player.h RPSMessage.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
GamePiece.o: GamePiece.cpp GamePiece.h Player.h RPSMessage.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSGame.o: RPSGame.cpp RPSGame.h GamePiece.h Player.h RPSMessage.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
main.o: main.cpp RPSGame.h GamePiece.h Player.h RPSMessage.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)
