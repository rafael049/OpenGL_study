# -*- MakeFile -*-
main : main.o glad.o
	g++ main.o glad.o -o main.exec -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lassimp
main.o : main.cpp
	g++ -c main.cpp
glad.o : glad.c
	gcc -c glad.c

clean:
	rm -f *.o
