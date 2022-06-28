//test.cpp

#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_timer.h>

#include <iostream>
#include <stdio.h>
#include "windows.h"
#include <vector>
#include <memory>
#include <conio.h>
#include <cstdlib>

char usertry[9];
char passtry[9];

const char * username = "username";//placeholder
const char * password = "password";//placeholder

#define log(x) std::cout << x
#define END std::cout << std::endl
#define MAX 100000
#define Twidth 5
#define Theight 5

class logincrypt{

	public:
	// Create a custom username and password
	virtual std::string Make_User(){

		char nUser[10];

			printf("Set Username: ");
			for (int i = 0; i < 10; i++){
				nUser[i] = getch();
				printf("*");
			}
			return nUser;
	}
	virtual std::string Make_Pass(){

		char nPass[10];

			printf("Set Password: ");
			for (int i = 0; i < 10; i++){
				nPass[i] = getch();
				printf("*");
			}
			return nPass;


	}
};

class SDLsubsystems{

	public:
		SDL_Window* cWindow;
		SDL_Renderer* cRender;
		SDL_Texture* cTexture;

		int speed;
        	SDL_Surface* PNG;
		SDL_Rect Entity;

		bool EventRunning = true;
		const char* PATH;

	private:

		const int nScreenWidth;
		const int nScreenHeight;
	public:

	SDLsubsystems()
		: cWindow(nullptr),
		cRender(nullptr), cTexture(nullptr),
		speed(500),
		PATH("Image/GREEN_BLOCK.png"),
		nScreenWidth(800), nScreenHeight(600)
	{
		
		//////////////////////////////////
		int32_t INIT = SDL_Init(SDL_INIT_EVERYTHING);
			log("->> SDL subsystems initializing...\n");

		if (INIT < 0)
			printf("INIT ERROR[SDL_Init(SDL_INIT_EVERYTHING): %s]\n", SDL_GetError());
		//////////////////////////////////
		cWindow = SDL_CreateWindow(
				"Tetris for my love",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				nScreenWidth,
				nScreenHeight,
				SDL_WINDOW_RESIZABLE);
		log("->> cWindow initializing...\n");
		if (cWindow == NULL)
			printf("cWindow ERROR[SDL_CreateWindow: %s]\n", SDL_GetError());
		///////////////////////////////////
	}

	struct imageloader{
	imageloader()
	{}
		void IMGLOAD(const char* file, SDL_Surface* Image){
			Image = IMG_Load(file);
			log("->> imageloader initialized\n");

			if (!Image)
				printf("IMAGE ERROR[IMG_Load: %s]\n", IMG_GetError());
		}
	};

	struct ApplyImage{
	ApplyImage()
	{}
		void APPIMG(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface* image){
			texture = SDL_CreateTextureFromSurface(renderer, image);
			log("->> ApplyImage initialized\n");

			if (texture == nullptr)
				printf("TEXTURE ERROR[CreateTextureFromSurface: %s]\n", SDL_GetError());
		}

	};

	// Create array to create Tetreminos
	
	void APPIMG(){

		// CreateRenderer //
		
		cRender = SDL_CreateRenderer(
				cWindow, -1,
				SDL_RENDERER_ACCELERATED);
		log("->> Renderer initialized\n->> Essential systems loaded\n\n");

		if (cRender == NULL) printf("Renderer ERROR[%s]\n", SDL_GetError());
		// Apply the IMG
		// AND call QueryTexture for IMG manipulation//


		PNG = IMG_Load(PATH);
		cTexture = SDL_CreateTextureFromSurface(cRender, PNG);
		SDL_QueryTexture(cTexture, nullptr, nullptr, &Entity.w, &Entity.h);

		//Initial position of texture
		Entity.w /= 35;
		Entity.h /= 35;

		Entity.x = (nScreenWidth - Entity.w) / 2;
		Entity.y = (nScreenHeight - Entity.h) / 2;

		int EX = Entity.x;
		int EY = Entity.y;


		int Tzpiece[Twidth * Theight] =
	{
	 0, 0, 0, 0, 0,
	 0, 0, 3, 0, 0,
	 0, 1, 2, 4, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0
	};	

	for(int i=0; i<Twidth*Theight; i++){
		log(Tzpiece[i]);
		END;
	}
	// place holder bracket

	/*

		cRender = SDL_CreateRenderer(
				cWindow, -1,
				SDL_RENDERER_ACCELERATED);

		PNG = IMG_Load(PATH);
		cTexture = SDL_CreateTextureFromSurface(cRender, PNG);
		SDL_QueryTexture(cTexture, nullptr, nullptr, &Entity.w, &Entity.h);
		

		// CreateRenderer //

		cRender = SDL_CreateRenderer(
				cWindow, -1,
				SDL_RENDERER_ACCELERATED);
		log("->> Renderer initialized\n->> Essential systems loaded\n\n");

		if (cRender == NULL) printf("Renderer ERROR[%s]\n", SDL_GetError());
		// Apply the IMG
		// AND call QueryTexture for IMG manipulation//


		PNG = IMG_Load(PATH);
		cTexture = SDL_CreateTextureFromSurface(cRender, PNG);
		SDL_QueryTexture(cTexture, nullptr, nullptr, &Entity.w, &Entity.h);

		//Initial position of texture
		Entity.w /= 35;
		Entity.h /= 35;

		Entity.x = (nScreenWidth - Entity.w) / 2;
		Entity.y = (nScreenHeight - Entity.h) / 2;

		int EX = Entity.x;
		int EY = Entity.y;

		switch(Tzpiece[i]){
		
			case 0:
				log("->>>NULL ");
				END;
				break;
			case 1:
				EX - Entity.x;
				break;
			case 2:
				log("original");
				break;
			case 3:
				EY - Entity.y;
				break;
			case 4:
				EX + Entity.x;
				break;

		}
	}
	*/

	}
	
	void InitImage(){

		APPIMG();

		//Entity.w /= 35;
		//Entity.h /= 35;

		//Entity.x = (nScreenWidth - Entity.w) / 2;
		//Entity.y = (nScreenHeight - Entity.h) / 2;

		// ATTEMPT AT POLYMORPHISM //
	}


	void Init(){

	InitImage();

	
	
	SDL_Event cEvent;

	while (EventRunning){


		while (SDL_PollEvent(&cEvent)){
	// Animation loops and event handling within this game loop

	switch(cEvent.type){

		case SDL_QUIT: EventRunning = false;
			      //SDL.~SDLsubsystems();
			break;
		case SDL_KEYDOWN: // if a key is pressed, read which key
			switch (cEvent.key.keysym.scancode){
			
				case SDL_SCANCODE_UP:
					Entity.y -= speed / 30;
					break;
				case SDL_SCANCODE_LEFT:
					Entity.x -= speed / 30;
					break;
				case SDL_SCANCODE_DOWN:
					Entity.y += speed / 30;
					break;
				case SDL_SCANCODE_RIGHT:
					Entity.x += speed / 30;
					break;
				default:
					break;
			}

	}
		}

	//Entity.y += speed / 200;

	if (Entity.x + Entity.w > nScreenWidth)
		Entity.x = nScreenWidth - Entity.w;
	if (Entity.x < 0)
		Entity.x = 0;
	if (Entity.y + Entity.h > nScreenHeight){
		Entity.y = nScreenHeight - Entity.h;
	}	
	if (Entity.y < 0){
		log("GAME OVER");
		EventRunning = false;
	}

	SDL_RenderClear(cRender);
	SDL_RenderCopy(cRender,
			cTexture,
			NULL,
			&Entity);
	SDL_RenderPresent(cRender);
	SDL_Delay(1000 / 120);

	}

	}
	

	~SDLsubsystems(){
		log("->> deconstructor called...\n");

		SDL_DestroyWindow(cWindow);
			cWindow = nullptr;
			PNG = nullptr;
			cTexture = nullptr;
		SDL_DestroyTexture(cTexture);
		SDL_DestroyRenderer(cRender);
        	SDL_FreeSurface(PNG);
		SDL_Quit();
		
		log("->> Memory destroyed\n");
		log("->> SDL subsystems uninitialized\n\n");

		log("Press ENTER to close...");
		std::cin.get();

	}
	

};

bool Main_BoolUser_Loop_Check = false;

void TryUser_nPass(){

while(1){

	log("Username: ");
	for(int i = 0; i < 8; i++){
		usertry[i] = getch();
		printf("*");
	}
	std::cout << std::endl;

	log("Password: ");
	for(int i = 0; i < 8; i++){
		passtry[i] = getch();
		printf("*");
	}
	std::cout << std::endl;

	if (strcmp(usertry, username) == 0 && strcmp(passtry, password) == 0){
		log("username and password correct...\n\n");
		Main_BoolUser_Loop_Check = true;
		std::cin.get();
		break;
	}
	else{
		log("username and/or password are not correct...\n\n");
		std::cin.get();
		system("cls");
	}
}
}

void Sortalg(){

	int num;
	int arr[MAX];

	log("Insert Max < 100000: ");
	std::cin >> num;

	for(int i=0; i<num; i++){
		arr[i] = rand();
	}

	for(int i = 0; i<num; i++){
		for(int j = i+1; j<num; j++){
  	 		if(arr[j]<arr[i]){
  	 			int T = arr[i];
  	 			arr[i] = arr[j];
  	 			arr[j] = T;
			}
		}
	}

	for(int i=0; i<num; i++){
		log(arr[i] << "_");
	}

	std::cin.get();
	std::cin.get();
}


void cCatDrawing(){


std::cout << "                  ;,_            ,                                        \n";
std::cout << "                 _uP~*b          d*u,                                     \n";
std::cout << "                dP'   *b       ,d*  *o					\n";
std::cout << "               d*    , `b     d*'    *b					\n";
std::cout << "              l] [    * `l,  d*       lb                                  \n";
std::cout << "              Ol ?     *  *b`*=uoqo,_  *l					\n";
std::cout << "            ,dBb *b        *b,    `*~~TObup,_				\n";
std::cout << "          ,d* (db.`*         **     *tbc,_ `~*Yuu,_			\n";
std::cout << "        .d* l`T'  '=                      ~     `**Yu,			\n";
std::cout << "      ,dO` gP,                           `u,   b,_  *b7			\n";
std::cout << "     d?' ,d* l,                           `*b,_ `~b  *1			\n";
std::cout << "   ,8i' dl   `l                 ,ggQOV*,dbgq,._*  `l  lb			\n";
std::cout << "  .df' (O,    *             ,ggQY*~  , @@@@@d*bd~  `b *1			\n";
std::cout << " .df'   `*           -=@QgpOY**     (b  @@@@P db    `Lp*b,		\n";
std::cout << ".d(                  _               *ko *=d_,Q`  ,_  *  *b,		\n";
std::cout << "Ql         .         `*qo,._          *tQo,_`**bo ;tb,    `*b,		\n";
std::cout << "qQ         |L           ~*QQQgggc,_.,dObc,opooO  `*~~*;.   __,7,		\n";
std::cout << "qp         t|io,_           `~*TOOggQV****        _,dg,_ =PIQHib.		\n";
std::cout << "`qp        `Q[*tQQQo,_                          ,pl{QOP*'   7AFR`		\n";
std::cout << "  `         `tb  '**tQQQg,_             p* *b   `       .;-.`Vl'		\n";
std::cout << "             *Yb      `*tQOOo,__    _,edb    ` .__   /`/'|  |b;=;.__	\n";
std::cout << "                           `*tQQQOOOOP**`*|QV;qQObob*`-._`|_~~-._		\n";
std::cout << "                                ###    ._        /   | |oP*|_   ~| ~|_~|	\n";
std::cout << "                                        `~*|ic,qggddOOP*|  |  ~|   `|~-._	\n";
std::cout << "                                          ,qP`***|*   | `| `;   `|   `|	\n";
std::cout << "                               _        _,p*     |    |   `|`;    |    |	\n";
std::cout << "                               *boo,._dP*       `|_  `|    `||   `|   ;	\n";
std::cout << "                                `*7tY~'            `|  `|    `|_   |	\n";
std::cout << "                                                      `~|  |		\n\n";
std::cout << " I love you to the moon and beyond!! Here's a cat!! \n";

Sleep(3000);
system("cls");

}


void getXY(int x, int y){
HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

CursorPosition.X = x;
CursorPosition.Y = y;
SetConsoleCursorPosition(Console, CursorPosition);
}

void S_Menu(){

	int menu_item=0, run, x=7;
	bool running = true;

	getXY(18,5); std::cout << "JUPOIO";
	getXY(18,7); std::cout << "->";

	while(running)
	{
                getXY(20,7);  std::cout << "1) Login";
		getXY(20,8);  std::cout << "2) Register";
		getXY(20,9);  std::cout << "3) Tetris";
		getXY(20,10); std::cout << "4) test tet";
		getXY(20,11); std::cout << "Quit Program";

		system("pause>nul");

		if(GetAsyncKeyState(VK_DOWN) && x != 11) //down button pressed
			{
				getXY(18,x); std::cout << "  ";
				x++;
				getXY(18,x); std::cout << "->";
				menu_item++;
				continue;

			}

		if(GetAsyncKeyState(VK_UP) && x != 7) //up button pressed
			{
				getXY(18,x); std::cout << "  ";
				x--;
				getXY(18,x); std::cout << "->";
				menu_item--;
				continue;
			}

		if(GetAsyncKeyState(VK_RETURN)){ // Enter key pressed

			switch(menu_item){

				case 0: {

					getXY(20,16);
					std::cout << "Login Here to access program  \n\n";
					TryUser_nPass();//////////////////////////////////////
					system("cls");

					getXY(20,7); std::cout << "Sorting test algorithm...'ENTER'...";
					std::cin.get();

					Sortalg();//////////////////////////////////////////
					running = false;
					break;
				}

				case 1: {
					getXY(20,16);
					std::cout << "Register Functions here...     ";
					break;
				}

				case 2: {
					getXY(20,16);
					std::cout << "Launching Tetris...     \n\n";
					SDLsubsystems SDL;
					SDL.Init();
					break;
				}

				case 3: {
					getXY(20,16);
					std::cout << "You chose Option 4...     ";
					break;
				}

				case 4: {
					getXY(20,16);
					std::cout << "Boolean false, and SDL_Quit()";
					running = false;
				}

			}

		}

	}

	getXY(20,21);
}

int main(int argc, char* args[]){

	cCatDrawing();
	S_Menu();


	//SDLsubsystems SDL;

	//SDL.Init();
	
/*
	int main(int argc, char * argv[])
{
    std::vector<std::unique_ptr<a>> vec;
    vec.push_back(std::unique_ptr<a>(new a()));
    vec.push_back(std::unique_ptr<b>(new b()));
    for (int i = 0; i < vec.size(); i++)
        vec[i]->func();

	std::vector<base> vec;
vec.push_back(std::unique_ptr<derived>(new derived()));

std::vector<base *> vec;
std::unique_ptr<derived> ptr(new derived());
vec.push_back(ptr.get());
ptr.release();
}
*/

	return 0;

}
