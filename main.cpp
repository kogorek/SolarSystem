#include <SDL2/SDL.h> // Библиотека SDL 2

#include <GL/gl.h> // Библиотека OpenGL
#include <GL/glu.h> // Библиотека GLU

#include <iostream> // стандартная библиотека ввода-вывода в C++
using namespace std;

SDL_Window *window; // задаем окно для SDL

const int width = 640; // ширина окна
const int height = 480; // высота окна

void drawCircle(float radius, int x, int y);
void display();
void physics(void);

struct planet
{
    float n_x, n_y;
    float f_vx, f_vy;
    float f_mass;
    float f_radius;
    float f_color[3];
};
const int N = 3;
planet planets[N];

struct star
{
    float f_x;
    float f_y;
    float f_color[3];
};

const int starQuantity = 200;
star stars[starQuantity];

void init(){

    // Инициализация SDL

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        cout << "Unable to init SDL, error: " << SDL_GetError() << endl;
        exit(1);
    }

    // Включаем двойной буфер, настраиваем цвета

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    // Создаем окно с заголовком "Cube", размером 640х480 и расположенным по центру экрана.

    window = SDL_CreateWindow("Sol system", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext glcontext = SDL_GL_CreateContext(window); // создаем контекст OpenGL

    if(window == NULL){ // если не получилось создать окно, то выходим
        exit(1);
    }

    // Инициализация OpenGL

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // устанавливаем фоновый цвет на черный
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST); // включаем тест глубины
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, (float) width / (float) height, 0.1f, 300.0f); // настраиваем трехмерную перспективу
    glMatrixMode(GL_MODELVIEW); // переходим в трехмерный режим
    //glOrtho(-240, 240, -160, 160, -1.0, 1.0);
}

int main(int argc, char *argv[]){


    for (int i = 0; i < starQuantity; i++)
    {
        stars[i].f_x = rand()%width - width/2;
        stars[i].f_y = rand()%height - height/2;
        stars[i].f_color[0] = 1;//float(rand())/1000000000;
        stars[i].f_color[1] = float(rand())/1000000000;
        stars[i].f_color[2] = float(rand())/1000000000;
    }

    //Sun
    planets[0].f_mass = 10000.0;
    planets[0].f_radius = 10.0;
    planets[0].f_vx = 0.0;
    planets[0].f_vy = 0.0;
    planets[0].n_x = .0;
    planets[0].n_y = 0.0;
    planets[0].f_color[0] = 1.0;
    planets[0].f_color[1] = 0.5;
    planets[0].f_color[2] = 0.5;

    //Earth
    planets[1].f_mass = 10.0;
    planets[1].f_radius = 4.0;
    planets[1].f_vx = 0.0;
    planets[1].f_vy = 0.8;
    planets[1].n_x = 100.0;
    planets[1].n_y = 0.0;
    planets[1].f_color[0] = 0.5;
    planets[1].f_color[1] = 0.5;
    planets[1].f_color[2] = 1.0;

    //Mercury
    planets[2].f_mass = 10;
    planets[2].f_radius = 3.0;
    planets[2].f_vx = 0.0;
    planets[2].f_vy = 1.2;
    planets[2].n_x = 50.0;
    planets[2].n_y = 0.0;
    planets[2].f_color[0] = 0.8;
    planets[2].f_color[1] = 0.3;
    planets[2].f_color[2] = 0.3;

    init(); // инициализация

    bool running = true;

    while(running){

        SDL_Event event; // события SDL

        while ( SDL_PollEvent(&event) ){ // начинаем обработку событий
            switch(event.type){ // смотрим:
            case SDL_QUIT: // если произошло событие закрытия окна, то завершаем работу программы
                running = false;
                break;

            case SDL_KEYDOWN: // если нажата клавиша
                switch(event.key.keysym.sym){ // смотрим какая
                case SDLK_ESCAPE: // клавиша ESC
                    running = false; // завершаем работу программы
                    break;
                }
                break;
            }
        }

        // пока программа запущена изменяем углы поворота, тем самым вращая куб



        // обновляем экран
        physics();
        display();
        glFlush();
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit(); // завершаем работу SDL и выходим
    return 0;
}

void physics(void)
{
    for (int i = 0; i < N; i++)
    {
        planet &p0 = planets[i];
        for (int j = 0; j < N; j++)
        {
            if (j == i)
                continue;
            const planet &p = planets[j];
            float d = sqrt( (p0.n_x - p.n_x)*(p0.n_x - p.n_x)+(p0.n_y - p.n_y)*(p0.n_y - p.n_y) );
            if (d > 3)
            {
                p0.f_vx += 0.007 * p.f_mass / d / d * (p.n_x - p0.n_x) / d;
                p0.f_vy += 0.007 * p.f_mass / d / d * (p.n_y - p0.n_y) / d;
            }
        }
        p0.n_x += p0.f_vx;
        p0.n_y += p0.f_vy;
    }
}

void drawCircle(float radius, int x, int y)
{
    int pointsInTotal = 24;
    float xArray[pointsInTotal];
    float yArray[pointsInTotal];
    double test;



    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -200.0f);

    for (float rad = radius; rad != 0; rad -= 0.5)
    {
        test = 0;
        for (int i=0;i<pointsInTotal;i++)
        {
            xArray[i] = x+cos(test*180/M_PI)*rad;
            yArray[i] = y+sin(test*180/M_PI)*rad;
            test += 0.005;
        }
        glBegin(GL_LINE_STRIP);
        for (int i=0;i<pointsInTotal;i++)
        {
            glVertex2f(xArray[i], yArray[i]);
        }
        glVertex2f(xArray[0], yArray[0]);
        glEnd();
    }
}

void display()
{
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glTranslatef(0.0f, 0.0f, -200.0f);
    glBegin(GL_POINTS);
    //glTranslatef(0.0f, 0.0f, -200.0f);
    for (int i = 0; i < starQuantity; i++)
    {
        glTranslatef(0.0f, 0.0f, -201.0f);
        glColor3f(stars[i].f_color[0], stars[i].f_color[1], stars[i].f_color[2]);
        glVertex2f(stars[i].f_x, stars[i].f_y);
    }
    glEnd();
    for (int i = 0; i < N; i++)
    {
        glColor3f(planets[i].f_color[0], planets[i].f_color[1], planets[i].f_color[2]);
        drawCircle(planets[i].f_radius, planets[i].n_x, planets[i].n_y);
    }
}
