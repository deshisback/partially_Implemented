#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#define PI 3.14159265

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
int win_width = 800;
int win_heigh = 800;
SDL_Event ev;

//функция деиницилизации подключенных зависимостей
void DeInit(int error) {
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();
	exit(error);
}

//функция подключения зависимостей
void Init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Couldn't init SDL! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	win = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (win == NULL) {
		std::cout << "Couldn't create window! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL) {
		std::cout << "Couldn't create renderer! Error:" << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	if (TTF_Init()) {
		std::cout << "Couldn't init SDL_ttf! Error:" << SDL_GetError();
		system("pause");
		DeInit(1);
	}
}

//функция, рисующая окружность
double circle(double x, double x0, double y0, double r) {
	 return sqrt(r * r - (x - x0) * (x - x0)) + y0;
}

double straight(double x, double x1, double x2, double y1, double y2) {
	return (x * (y2 - y1) / (x2 - x1) - x1 * (y2 - y1) / (x2 - x1) + y1);
}

//функция, переводящая вычисленные координаты в координаты для правильного отображения в окне
void mathCoordsToScreen(double x, double y, double scale, int centerx, int centery, int& sx, int& sy) {
	sx = round(centerx + x * scale);
	sy = round(centery - y * scale);
}

//функция, рисующая разметку в окне
void drawLines() {
	SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
	SDL_RenderDrawLine(ren, 0, win_heigh/2, win_width, win_heigh / 2);
	SDL_RenderDrawLine(ren, win_width / 2, 0, win_width/2, win_heigh);
}

int main(int argc, char* argv[]) {

	Init();
	setlocale(LC_ALL, "ru");
	srand(time(NULL));

	TTF_Font* font = TTF_OpenFont("impact.ttf", 25);

	char xchar[2] = "x";
	char ychar[2] = "y";

	SDL_Surface* xsurf = TTF_RenderText_Blended(font, xchar, { 0, 0, 0, 255 });
	SDL_Surface* ysurf = TTF_RenderText_Blended(font, ychar, { 0,0,0,255 });

	SDL_Rect xrect = { 410, 5, xsurf->w, xsurf->h };
	SDL_Rect yrect = { 780, 410, ysurf->w, ysurf->h };

	SDL_Texture* xtext = SDL_CreateTextureFromSurface(ren, xsurf);
	SDL_Texture* ytext = SDL_CreateTextureFromSurface(ren, ysurf);

	SDL_SetRenderDrawColor(ren, 187, 187, 187, 255);
	SDL_RenderClear(ren);
	drawLines();
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

	double m, v, q, b, alpha;
	std::cout << "Масса записывается в формате m * 10 ^ -31, скорость в формате v * 10^7, заряд в формате q * 10 ^ -19, значение вектора в формате b * 10^-3" << std::endl;
	std::cout << "Введите данные частицы: " << std::endl;
	std::cout << "m = ";
	std::cin >> m;
	std::cout << "v = ";
	std::cin >> v;
	std::cout << "q = ";
	std::cin >> q;
	std::cout << "b = ";
	std::cin >> b;
	std::cout << "angle = ";
	std::cin >> alpha;
	
	std::cout <<std::endl << "m = " << m << " * 10 ^ -31" << std::endl;
	std::cout << "v = " << v << " * 10 ^ 7" << std::endl;
	std::cout << "q = " << q << " * 10 ^ -19" << std::endl;
	std::cout << "B = " << b << " * 10 ^ -3" << std::endl;
	double x, y;
	int sx1, sy1, sx2, sy2;
	double rad = m * v / (q * b);
	bool isRunning = true;
	
		if ((int)alpha == 90) {
	
			rad *= 100;
	
			SDL_Rect r = { 0,0,3,3 };
	
			for (double angel = 0; angel <= 360; angel += 0.5) {
				SDL_PollEvent(&ev);
				x = rad * cos(angel * M_PI / 180);
				y = rad * sin(angel * M_PI / 180);
				mathCoordsToScreen(x, y, 1, win_width / 2, win_heigh / 2, sx1, sy1);
				r.x = sx1 - 1;
				r.y = sy1 - 1;
				SDL_RenderFillRect(ren, &r);
			}
		}

		else if ((int)alpha == 0 || (int)alpha == 180) {

			double s1, s2, t1, t2;

			std::cout << "Введите расстояние и время первой и второй точки " << std::endl;
			std::cout << "S1 = ";
			std::cin >> s1;
			std::cout << "T1 = ";
			std::cin >> t1;
			std::cout << "S2 = ";
			std::cin >> s2;
			std::cout << "T2 = ";
			std::cin >> t2;

			double v1 = s1 / t1;
			double v2 = s2 / t2;
			std::cout << "V1 = " << v1 << ", v2 = " << v2;

			double y1, y2;

			for (int i = 0; i <= win_width / 2; i++) {
				SDL_PollEvent(&ev);
				y1 = straight(i, t1, t2, s1, s2) / 10;
				y2 = straight(i + 1, t1, t2, s1, s2) / 10;
				mathCoordsToScreen(i, y1, 1, win_width / 2, win_heigh / 2, sx1, sy1);
				mathCoordsToScreen(i + 1, y2, 1, win_width / 2, win_heigh / 2, sx2, sy2);

				SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);
			}
		}

		else {

			double r = (m * v * sin(alpha * PI / 180)) / (abs(q) * b); // считает в радинаха аргумент
			double h = (2 * PI * m * v * cos(alpha * PI / 180)) / (q * b);
			r *= 10;
			h *= 10;

			double temph = 0;
			int changer = 1;
			int radius = 0;
			double dif = r / h;

			for (int i = -win_width / 2; i < win_width / 2; i++) {
				SDL_PollEvent(&ev);
				
				if (temph <= h / 2) {

					mathCoordsToScreen(i, radius * changer * dif, 1, win_width / 2, win_heigh / 2, sx1, sy1);
					mathCoordsToScreen(i + 1, (radius - 1) * changer * dif, 1, win_width / 2, win_heigh / 2, sx2, sy2);
					SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);

					radius--;
					temph++;
				}

				else if (temph > h / 2 && temph < h) {

					mathCoordsToScreen(i, radius * changer * dif, 1, win_width / 2, win_heigh / 2, sx1, sy1);
					mathCoordsToScreen(i + 1, (radius - 1) * changer * dif, 1, win_width / 2, win_heigh / 2, sx2, sy2);
					SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);

					radius++;
					temph++;
				}

				else {
					temph = 0;
					changer *= -1;
					radius = 0;
				}

			}
			
		}
	
	SDL_RenderCopy(ren, xtext, NULL, &xrect);
	SDL_RenderCopy(ren, ytext, NULL, &yrect);
	SDL_RenderPresent(ren);
	SDL_Delay(5000);

	SDL_DestroyTexture(ytext);
	SDL_DestroyTexture(xtext);
	TTF_CloseFont(font);
	DeInit(0);
	return 0;
}