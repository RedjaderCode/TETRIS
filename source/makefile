
SDL_Tetris: SDL_Tetris.cpp
	g++ SDL_Tetris.cpp -IC:\mingw_SDL_lib\include\SDL2 -LC:\mingw_SDL_lib\lib -w -lmingw32 -lSDL2main -lSDL2 -lSDL2_Image -static-libgcc -static-libstdc++ -o SDL_Tetris

test: test.cpp
	g++ test.cpp -IC:\mingw_SDL_lib\include\SDL2 -LC:\mingw_SDL_lib\lib -w -lmingw32 -lSDL2main -lSDL2 -lSDL2_Image -static-libgcc -static-libstdc++ -o test

INSTALLER: INSTALLER.cpp
	g++ INSTALLER.cpp -static-libstdc++ -static-libgcc -o INSTALLER

clean:
	rm *.o *.exe *.s
