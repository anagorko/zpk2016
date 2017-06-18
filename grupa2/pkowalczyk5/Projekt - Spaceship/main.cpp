#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <math.h>
#include <iostream>
using namespace std;

/*
English naming convention and comments for potential non-Polish readers.

Author: Paweł Kowalczyk
Contact: p.kowalczyk5@student.uw.edu.pl
Project: Graded project for Advanced Computer Programming course, 2016/2017 spring semester,
         Faculty of Economic Sciences, University of Warsaw

Disclaimers:
The libraries used in this project are parts of the standard C++ language resources and the basic Allegro 5.0 package.
Allegro libraries are used for handling all interface-related aspects of the program.
All non-dynamically drawn graphics were created by Paweł Kowalczyk - source graphic files available upon request.
The included fonts are either default Windows .ttf fonts or ones freely available for non-commercial use:
 -"font-courbi" is standard Windows Courier New with bold and italic
 -"font-OptimusPrinceps" is a custom font available at: http://www.dafont.com/optimusprinceps.font ,
  with the authors' permission at: http://manfred-klein.ina-mar.com/
*/

//configuration
//game variables
bool key[ALLEGRO_KEY_MAX];

const float FPS = 60;

const int screen_w = 600;   // screen width
const int screen_h = 600;   // screen height
int difficulty_level = 0;   // game difficulty

//ship parameters
const int ship_x0 = screen_w/2 - 20;        // starting X position of ship top left corner - screen center
const int ship_y0 = screen_h - 40 - 10;     // starting Y position of ship top left corner - screen bottom
const int ship_speed_list[3] = {10, 8, 6};  // ship movement speed (pixels per click)
const string ship_graphic = "spaceship-v3-4flames.png"; //source graphic file for the ship (with flame variants)

//obstacle parameters
const int obstacle_speed_list[3] = {4, 6, 8};       // default falling speed of obstacles (changes with game difficulty level)
const int obstacle_min_time_list[3] = {12, 6, 3};   // minimal interval before next obstacle
const int obstacle_time_list[3] = {240, 180, 90};   // maximum delay before next obstacle
const int obstacle_delay = 120;                     // off-screen delay - so the obstacle doesn't appear/disappear immediately
const string propeller_graphic = "propeller-bitmap.png";

//flags for game control
bool menu_main = true;          // displaying main menu (default screen)
bool menu_settings = false;     // displaying game settings
bool game_active = false;
bool game_paused = false;
bool program_active = true;
bool user_exit = false;         // was the game exited manually?
bool ship_wrecked = false;      // was the ship destroyed? - these two control the exit message for the user

//ship collision markers
//front, left cabin, right cabin, left first wing, right first wing, left second wing, right second wing, back
int ship_marker_x[8] = {20, 14, 26, 0, 40, 5, 35, 20};
int ship_marker_y[8] = {0, 13, 13, 27, 27, 36, 36, 40};



enum obstacle_type
{
    ball = 0,       //normal round obstacle
    paddle,         //horizontally moving rectangle
    volatile_ball,  //round obstacle with changing radius
    gate,           //rectangles on both sides with horizontal lines expanding to the center of the screen
    propeller       //rotating propeller-like obstacle
};

enum obstacle_threshold //thresholds in a 0-100 range, from which the game randomly draws a number to choose the type
{
    t_ball = 20,
    t_paddle = 40,
    t_volatile_ball = 60,
    t_gate = 80,
    t_propeller = 100
};

class obstacle_class
{
    private:
        //parameters that are always used
        double center_x;
        double center_y;
        int speed;
        enum obstacle_type type;
        double obst_size;

        //not always used - only for certain obstacle types
        int min_size; //size limit for obstacle transformations and movement
        int max_size; //size limit for obstacle transformations and movement
        double angle; //only for propellers, the current rotation angle (in radians)
        double direction; //only for moving obstacles, takes values of {-1, 1}
        //paddle: -1 = left, 1 = right
        //volatile_ball: -1 = shrink, 1 = expand
        //gate: -1 = open, 1 = close
        //propeller: -1 = counter-clockwise, 1 = clockwise

    public:
        obstacle_class(){};
        ~obstacle_class(){};

        double getX(){ return center_x; };
        double getY(){ return center_y; };
        int getSpeed(){ return speed; };
        obstacle_type getType(){ return type; };
        double getSize(){ return obst_size; };
        int getMinSize(){ return min_size; };
        int getMaxSize(){ return max_size; };
        double getAngle(){ return angle; };
        double getDirection(){ return direction; };

        void setX(double xn){ center_x = xn; };
        void setY(double yn){ center_y = yn; };
        void setSpeed(int sn){ speed = sn; };
        void setType(obstacle_type tn){ type = tn; };
        void setSize(double sn){ obst_size = sn; };
        void setMinSize(int minsn){ min_size = minsn; };
        void setMaxSize(int maxsn){ max_size = maxsn; };
        void setAngle(double an){ angle = an; };
        void setDirection(double dn){ direction = dn; };

        void Generation(int new_speed)
        {
            //indicator used to define the type of the upcoming obstacle
            int obst_type_ind = 1 + rand()%100;

            if (obst_type_ind <= t_ball)
            { setType(ball); }
            else if (obst_type_ind < t_paddle)
            { setType(paddle); }
            else if (obst_type_ind < t_volatile_ball)
            { setType(volatile_ball); }
            else if (obst_type_ind < t_gate)
            { setType(gate); }
            else if (obst_type_ind < t_propeller)
            { setType(propeller); }

            setY(-obstacle_delay); //delay added so they don't appear out of nowhere
            setSpeed(new_speed);

            switch(getType())
            {
                case ball:
                    setX(rand()%screen_w);
                    setMinSize(30);
                    setMaxSize(80);
                    setSize( getMinSize() + rand()%(getMaxSize() - getMinSize()) );
                    break;
                case paddle:
                    setMinSize(40);
                    setMaxSize(80);
                    setSize( getMinSize() + rand()%(getMaxSize() - getMinSize()) );

                    setX(getSize() + rand()%(screen_w - (int) getSize() ) );

                    if(rand()%2 == 0){ setDirection(-1); }
                    else { setDirection(1); }
                    break;
                case volatile_ball:
                    setX(rand()%screen_w);
                    setMinSize(20);
                    setMaxSize(80);
                    setSize( getMinSize() + rand()%(getMaxSize() - getMinSize()) );

                    if(rand()%2 == 0){ setDirection(-1); }
                    else { setDirection(1); }
                    break;
                case gate:
                    setMinSize(0);
                    setMaxSize(screen_w/2 - 50);
                    setSize(1 + rand()%150);

                    setDirection(1);
                    break;
                case propeller:
                    setX( 120 + rand()%(screen_w - 240) );

                    if(rand()%2 == 0){ setDirection(-1); }
                    else { setDirection(1); }

                    setAngle( (1 + rand()%314) ); //radian hundredth parts
                    setAngle( getAngle()/100 ); //for some reason it didn't want to work in one line
                    break;
            }
        };

        void Movement()
        {
            //due to the different structure of each obstacle type they have to be moved separately
            switch(getType())
            {
                case ball:
                    setY(getY() + getSpeed());
                    break;
                case paddle:
                    if( (getDirection() == -1 && getX() <= getSize())
                        || (getDirection() == 1 && getX() >= screen_w - getSize()) )
                    { setDirection( (-1)*getDirection() ); }
                    setY(getY() + getSpeed());
                    setX( getX() + getDirection()*getSpeed() );
                    break;
                case volatile_ball:
                    setY(getY() + getSpeed());
                    if( getSize() >= getMaxSize() || getSize() <= getMinSize() )
                    { setDirection( (-1)*getDirection() ); }
                    setSize( getSize() + getDirection()*getSpeed()/4 );
                    break;
                case gate:
                    setY(getY() + getSpeed());
                    if( getSize() > getMaxSize() || getSize() <= getMinSize() )
                    { setDirection( (-1)*getDirection() ); }
                    setSize( getSize() + getDirection()*getSpeed() );
                    break;
                case propeller:
                    setY(getY() + getSpeed());
                    setAngle( getAngle() + getDirection()*getSpeed()/100 );
                    if(abs(getAngle()) >= 3.14) { setAngle( getAngle() - getDirection()*3.14 ); }
                    break;
            }
        };
};



class ship_class
{
    private:
        int x, y;
        int speed;

    public:
        ship_class(){};
        ship_class(int xn, int yn, int sn)
        {
            x = xn;
            y = yn;
            speed = sn;
        };
        ~ship_class(){};

        int getX(){ return x; };
        int getY(){ return y; };
        int getSpeed(){ return speed; };

        void setX(int xn){ x = xn; };
        void setY(int yn){ y = yn; };
        void setSpeed(int sn){ speed = sn; };
};



void check_collision(obstacle_class obs, ship_class s)
{
    int x[8], y[8];

    for(int i = 0; i < 8; i++)
    {
        x[i] = s.getX() + ship_marker_x[i];
        y[i] = s.getY() + ship_marker_y[i];
    }

    for(int i = 0; i < 8; i++)
    {
        //due to the different structure of each obstacle type they have to be checked separately
        switch(obs.getType())
        {
            case ball:
                if( sqrt(pow(y[i]-obs.getY(), 2) + pow(x[i]-obs.getX(), 2)) < obs.getSize() )
                {
                    ship_wrecked = true;
                    game_active = false;
                }
                break;
            case paddle:
                if( (abs(y[i] - obs.getY()) < 20 ) && (abs(x[i] - obs.getX()) < obs.getSize()) )
                {
                    ship_wrecked = true;
                    game_active = false;
                }
                break;
            case volatile_ball:
                if( sqrt(pow(y[i]-obs.getY(), 2) + pow(x[i]-obs.getX(), 2)) < obs.getSize() )
                {
                    ship_wrecked = true;
                    game_active = false;
                }
                break;
            case gate:
                if( (abs(y[i] - obs.getY()) < 15 && x[i]  < 50) //left base
                    || (abs(y[i] - obs.getY()) < 15 && x[i]  > screen_w - 50) //right base
                    || (abs(y[i] - obs.getY()) < 5 && x[i] < 50 + obs.getSize()) //left wing
                    || (abs(y[i] - obs.getY()) < 5 && x[i] > screen_w - 50 - obs.getSize()) //right wing
                   )
                {
                    ship_wrecked = true;
                    game_active = false;
                }
                break;
            case propeller:
                double p1_x, p1_y;

                //rotate the collision marker to default (horizontal) propeller position
                if( obs.getDirection() == -1 )
                {
                    p1_x = cos(-obs.getAngle()) * (x[i] - obs.getX())
                            - sin(-obs.getAngle()) * (y[i] - obs.getY()) + obs.getX();
                    p1_y = sin(-obs.getAngle()) * (x[i] - obs.getX())
                            + cos(-obs.getAngle()) * (y[i] - obs.getY()) + obs.getY();
                }
                else if ( obs.getDirection() == 1 )
                {
                    p1_x = cos(3.14 - obs.getAngle()) * (x[i] - obs.getX())
                            - sin(3.14 - obs.getAngle()) * (y[i] - obs.getY()) + obs.getX();
                    p1_y = sin(3.14 - obs.getAngle()) * (x[i] - obs.getX())
                            + cos(3.14 - obs.getAngle()) * (y[i] - obs.getY()) + obs.getY();
                }

                //now just compare with standard circle and rectangle conditions
                if( sqrt(pow(p1_y-obs.getY(), 2) + pow(p1_x-obs.getX(), 2)) < 25
                   ||  (abs(p1_y - obs.getY()) < 5 && abs(p1_x - obs.getX()) < 145) )
                {
                    ship_wrecked = true;
                    game_active = false;

                    //cout << "Ship: " << i << " " << x[i] << " " << y[i] << ", transform: " << p1_x << " " << p1_y << endl;
                    //cout << "Propeller: " << obs.getX() << " " << obs.getY() << " " << obs.getAngle() << endl;
                }
                break;
        }
    }
}

void draw_obstacle(obstacle_class x, ALLEGRO_BITMAP *obstacle_bitmaps[])
{
    //due to the different structure of each obstacle type they have to be drawn separately
    switch(x.getType())
    {
        case ball:
            al_draw_filled_circle(x.getX(), x.getY(), x.getSize(), al_map_rgb(255, 255, 255));
            break;
        case paddle:
            al_draw_filled_rounded_rectangle(x.getX()-x.getSize(),x.getY()-20,
                                            x.getX()+x.getSize(),x.getY()+20,
                                            10,10,al_map_rgb(255, 255, 255) );
            break;
        case volatile_ball:
            al_draw_filled_circle(x.getX(), x.getY(), x.getSize(), al_map_rgb(255, 255, 255));
            break;
        case gate:
            al_draw_filled_rounded_rectangle(-20, x.getY()-15, 50, x.getY()+15, 15, 5, al_map_rgb(255, 255, 255));
            al_draw_filled_rounded_rectangle(screen_w-50, x.getY()-15, screen_w+20,
                                             x.getY()+15, 15, 5, al_map_rgb(255, 255, 255));
            al_draw_filled_rectangle(50, x.getY()-5, 50+x.getSize(), x.getY()+5, al_map_rgb(255, 255, 255));
            al_draw_filled_rectangle(screen_w-50-x.getSize(), x.getY()-5, screen_w-50, x.getY()+5,
                                     al_map_rgb(255, 255, 255));
            break;
        case propeller:
            al_draw_rotated_bitmap(obstacle_bitmaps[0], 145, 25, x.getX(), x.getY(), x.getAngle(), 0);
            break;
    }

}

void draw_ship(ship_class s, ALLEGRO_BITMAP *bitmap)
{
    //draw ship with flame animation
    int ship_draw_ind = rand()%4;
    al_draw_bitmap_region(bitmap, 0+ship_draw_ind*40, 0, 40, 48, s.getX(), s.getY(), 0);

    /*
    //draw collision markers
    for(int i = 0; i < 8; i++)
    {
        al_draw_filled_circle(s.getX()+ship_marker_x[i], s.getY()+ship_marker_y[i], 2, al_map_rgb(255, 0, 0));
    }
    */

}



int main()
{
    //basic allegro initialization
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_set_new_window_position(600, 60);



    //load graphics
    al_init_image_addon();
    ALLEGRO_BITMAP* ship_bitmap;
    ship_bitmap = al_load_bitmap(ship_graphic.c_str());

    ALLEGRO_BITMAP* obstacle_bitmaps[1];
    obstacle_bitmaps[0] = al_load_bitmap(propeller_graphic.c_str());



    //allegro event sources
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;



    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        cerr << "Error during timer initialization." << endl;
        return -1;
    }

    display = al_create_display(screen_w, screen_h);
    if(!display) {
        cerr << "Error during screen initialization." << endl;
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        cerr << "Error during event queue initialization." << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());



    //allegro fonts definition
    ALLEGRO_FONT *f_title_1 = al_load_ttf_font("font-courbi.ttf", 60, 0);
    ALLEGRO_FONT *f_title_2 = al_load_ttf_font("font-courbi.ttf", 30, 0);
    ALLEGRO_FONT *f_title_3 = al_load_ttf_font("font-courbi.ttf", 15, 0);
    ALLEGRO_FONT *f_menu_option = al_load_ttf_font("font-courbi.ttf", 20, 0);
    ALLEGRO_FONT *f_death = al_load_ttf_font("font-OptimusPrinceps.ttf", 100, 0);



    //loading screen :)
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_text(f_title_1, al_map_rgb(255,255,255), screen_w/2, screen_h/2-50, ALLEGRO_ALIGN_CENTRE, "SPACESHIP");
    al_draw_text(f_title_2, al_map_rgb(255,255,255), screen_w/2, screen_h/2, ALLEGRO_ALIGN_CENTRE, "by Paweł Kowalczyk");

    al_flip_display();
    al_rest(2);


    //almost ready
    al_start_timer(timer);

    bool redraw = true;

    srand( time(NULL) );



    while(program_active)
    {
        //cout << "Before declarations." << endl;

        //initialization
        //game control variables
        int obstacle_timer = 0;     //time since last obstacle
        int next_obstacle = FPS;    //time until next obstacle - first obstacle spawns after 1s
        int no_of_obstacles = 0;    //current number of obstacles

        //ship declaration
        ship_class ship(ship_x0, ship_y0, ship_speed_list[difficulty_level]);

        //default difficulty level
        int obstacle_speed = obstacle_speed_list[difficulty_level];
        int obstacle_min_time = obstacle_min_time_list[difficulty_level];
        int obstacle_time_bracket = obstacle_time_list[difficulty_level];

        //obstacle list declaration and cleaning
        int min_list_size = (screen_h+2*obstacle_delay)/obstacle_speed/obstacle_min_time;
        obstacle_class obst_list[min_list_size];
        //the minimum size to handle a worst-case scenario - an obstacle spawning every minimal interval
        //array with the size of the max possible number of active obstacles under the most pessimistic circumstances

        for(int i = 0; i < min_list_size; i++)
        {
            obst_list[i].setX(0);
            obst_list[i].setY(0);
            obst_list[i].setSpeed(0);
            obst_list[i].setType(ball);
            obst_list[i].setSize(0);
            obst_list[i].setMinSize(0);
            obst_list[i].setMaxSize(0);
            obst_list[i].setAngle(0);
            obst_list[i].setDirection(0);
        }



        ALLEGRO_EVENT ev;

        //cout << "After declarations, before main menu." << endl;



        while(menu_main)
        {
            //what to do when we are in the main menu screen

            //1. New game
            //2. Settings
            //3. Exit

            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                if(key[ALLEGRO_KEY_1])
                {
                    //set flags to start a new game
                    game_active = true;
                    menu_main = false;
                    menu_settings = false;
                }
                else if (key[ALLEGRO_KEY_2])
                {
                    //set flags to enter settings menu
                    game_active = false;
                    menu_main = false;
                    menu_settings = true;
                }
                else if (key[ALLEGRO_KEY_3] || key[ALLEGRO_KEY_ESCAPE])
                {
                    //set flags to exit the program
                    program_active = false;
                    game_active = false;
                    menu_main = false;
                    menu_settings = false;
                }

                redraw = true;
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                key[ev.keyboard.keycode] = true;

                redraw = true;

            }
            else if (ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                key[ev.keyboard.keycode] = false;
            }

            if(redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;

                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_text(f_title_2, al_map_rgb(255,255,255), 95, 10, ALLEGRO_ALIGN_CENTRE, "SPACESHIP");
                al_draw_text(f_title_3, al_map_rgb(255,255,255), 95, 35, ALLEGRO_ALIGN_CENTRE, "by Paweł Kowalczyk");

                al_draw_rectangle(100, 100, screen_w-100, screen_h-100, al_map_rgb(0, 0, 255), 3);
                al_draw_line(100, 200, screen_w-100, 200, al_map_rgb(0, 0, 255), 3);

                al_draw_text(f_title_2, al_map_rgb(0,0,255), screen_w/2, 150-15, ALLEGRO_ALIGN_CENTRE, "MAIN MENU");

                al_draw_rounded_rectangle(screen_w/2-100, screen_h/2-20, screen_w/2+100, screen_h/2+20,
                                          10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), screen_w/2, screen_h/2-10,
                             ALLEGRO_ALIGN_CENTRE, "1. New game");

                al_draw_rounded_rectangle(screen_w/2-100, screen_h/2+30, screen_w/2+100, screen_h/2+70,
                                          10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), screen_w/2, screen_h/2+40,
                             ALLEGRO_ALIGN_CENTRE, "2. Settings");

                al_draw_rounded_rectangle(screen_w/2-100, screen_h/2+80, screen_w/2+100, screen_h/2+120,
                                          10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), screen_w/2, screen_h/2+90,
                             ALLEGRO_ALIGN_CENTRE, "3. Exit");

                al_flip_display();
            }
        }
        //cout << "Between main menu and settings." << endl;



        while(menu_settings)
        {
            //what to do when we are in the settings screen

            //1. Set difficulty level "Normal"
            //2. Set difficulty level "Hard"
            //3. Set difficulty level "Very hard"
            //4. Back to main menu

            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                if(key[ALLEGRO_KEY_1])
                {
                    difficulty_level = 0;
                }
                else if (key[ALLEGRO_KEY_2])
                {
                    difficulty_level = 1;
                }
                else if (key[ALLEGRO_KEY_3])
                {
                    difficulty_level = 2;
                }

                redraw = true;
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                key[ev.keyboard.keycode] = true;

                redraw = true;

            }
            else if (ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                key[ev.keyboard.keycode] = false;

                if (ev.keyboard.keycode == ALLEGRO_KEY_4 || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    //change flags to go back to main menu
                    menu_main = true;
                    menu_settings = false;

                    //before leaving settings, update game parameters with difficulty level
                    ship.setSpeed(ship_speed_list[difficulty_level]);
                    obstacle_speed = obstacle_speed_list[difficulty_level];
                    obstacle_min_time = obstacle_min_time_list[difficulty_level];
                    obstacle_time_bracket = obstacle_time_list[difficulty_level];

                    cout << "Leaving settings, difficulty level: " << difficulty_level+1 << endl;
                }
            }

            if(redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;

                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_text(f_title_2, al_map_rgb(255,255,255), 95, 10, ALLEGRO_ALIGN_CENTRE, "SPACESHIP");
                al_draw_text(f_title_3, al_map_rgb(255,255,255), 95, 35, ALLEGRO_ALIGN_CENTRE, "by Paweł Kowalczyk");

                al_draw_rectangle(100, 100, screen_w-100, screen_h-100, al_map_rgb(0, 0, 255), 3);
                al_draw_line(100, 200, screen_w-100, 200, al_map_rgb(0, 0, 255), 3);

                al_draw_text(f_title_2, al_map_rgb(0,0,255), screen_w/2, 150-15, ALLEGRO_ALIGN_CENTRE, "SETTINGS");

                al_draw_rounded_rectangle(120, 270, 280, 310, 10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), 200, 280, ALLEGRO_ALIGN_CENTRE, "Difficulty:");

                al_draw_rounded_rectangle(300, 220, screen_w-120, 260, 10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), 390, 230, ALLEGRO_ALIGN_CENTRE, "1. Normal");

                al_draw_rounded_rectangle(300, 270, screen_w-120, 310, 10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), 390, 280, ALLEGRO_ALIGN_CENTRE, "2. Hard");

                al_draw_rounded_rectangle(300, 320, screen_w-120, 360, 10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), 390, 330, ALLEGRO_ALIGN_CENTRE, "3. Very hard");

                al_draw_rounded_rectangle(screen_w/2-120, screen_h/2+140, screen_w/2+120, screen_h/2+180,
                                          10, 10, al_map_rgb(255, 255, 255), 3);
                al_draw_text(f_menu_option, al_map_rgb(255,255,255), screen_w/2, screen_h/2+150,
                             ALLEGRO_ALIGN_CENTRE, "4. Main menu");

                al_flip_display();
            }
        }

        //cout << "Between settings and game." << endl;



        while(game_active)
        {
            //what to do while a game is running

            //ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_TIMER && !game_paused)
            {
                //ship movement
                if(key[ALLEGRO_KEY_LEFT] && ship.getX() >= 15)
                {
                    ship.setX(ship.getX() - ship.getSpeed());
                }
                else if (key[ALLEGRO_KEY_LEFT] && ship.getX() < 15)
                {
                    ship.setX(5);
                }

                if(key[ALLEGRO_KEY_RIGHT] && ship.getX() <= screen_w - 55)
                {
                    ship.setX(ship.getX() + ship.getSpeed());
                }
                else if (key[ALLEGRO_KEY_RIGHT] && ship.getX() > screen_w - 55)
                {
                    ship.setX(screen_w - 40 - 5);
                }

                if(key[ALLEGRO_KEY_UP] && ship.getY() >= 15)
                {
                    ship.setY(ship.getY() - ship.getSpeed());
                }
                else if (key[ALLEGRO_KEY_UP] && ship.getY() < 15)
                {
                    ship.setY(5);
                }

                if(key[ALLEGRO_KEY_DOWN] && ship.getY() <= screen_h - 55)
                {
                    ship.setY(ship.getY() + ship.getSpeed());
                }
                else if (key[ALLEGRO_KEY_DOWN] && ship.getY() > screen_h - 60)
                {
                    ship.setY(screen_h - 40 - 10);
                }

                redraw = true;

            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && !game_paused)
            {
                key[ev.keyboard.keycode] = true;

                redraw = true;

            }
            else if (ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                key[ev.keyboard.keycode] = false;

                if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && !game_paused)
                {
                    game_paused = true;
                    redraw = false;
                    cout << "Game paused." << endl;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && game_paused)
                {
                    game_paused = false;
                    redraw = true;
                    cout << "Game unpaused." << endl;
                }

                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    user_exit = true;
                    game_active = false;
                    menu_main = true;
                }

            }

            if(!game_paused)
            {
                //obstacle movement
                for(int i = 0; i < no_of_obstacles; i++)
                {
                    obst_list[i].Movement();
                }

                //obstacle destruction
                //obstacles are ordered, so the first one in the array is always the first to destroy
                if(no_of_obstacles > 0)
                {
                    if(obst_list[0].getY() >= screen_h + obstacle_delay) //delay added so they don't disappear immediately
                    {
                        for(int i = 0; i < no_of_obstacles - 1; i++)
                        {
                            obst_list[i].setType( obst_list[i+1].getType() );
                            obst_list[i].setX( obst_list[i+1].getX() );
                            obst_list[i].setY( obst_list[i+1].getY() );
                            obst_list[i].setSpeed( obst_list[i+1].getSpeed() );
                            obst_list[i].setSize( obst_list[i+1].getSize() );
                            obst_list[i].setMinSize( obst_list[i+1].getMinSize() );
                            obst_list[i].setMaxSize( obst_list[i+1].getMaxSize() );
                            obst_list[i].setDirection( obst_list[i+1].getDirection() );
                            obst_list[i].setAngle( obst_list[i+1].getAngle() );
                        }

                    no_of_obstacles--;

                    //cout << "Obstacle destroyed, count: " << no_of_obstacles << endl;

                    }
                }

                //obstacle generation
                if(obstacle_timer == next_obstacle)
                {
                    obst_list[no_of_obstacles].Generation(obstacle_speed);

                    obstacle_timer = 0;
                    next_obstacle = obstacle_min_time + rand()%obstacle_time_bracket;

                    no_of_obstacles++;

                    //cout << "Obstacle created, count: " << no_of_obstacles << ", next in " << next_obstacle << endl;
                }

                //obstacle timer update
                obstacle_timer++;
            }

            if(redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;

                for(int i = 0; i < no_of_obstacles; i++)
                {
                    check_collision(obst_list[i], ship);
                    if(!game_active)
                    {
                        menu_main = true;
                    }
                }

                al_clear_to_color(al_map_rgb(0,0,0));

                for(int i = 0; i < no_of_obstacles; i++)
                {
                    draw_obstacle(obst_list[i], obstacle_bitmaps);
                }

                draw_ship(ship, ship_bitmap);

                al_flip_display();

             }

            if (user_exit)
            {
                cout << "You pressed 'Escape'. Exiting the game." << endl;
                user_exit = false;
            }
            else if (ship_wrecked)
            {
                al_draw_text(f_death, al_map_rgb(255,0,0), screen_w/2, screen_h/2-50, ALLEGRO_ALIGN_CENTRE, "YOU DIED");
                al_flip_display();
                al_rest(2);

                cout << "Your ship has crashed. Game over." << endl;
                ship_wrecked = false;
            }

        }

        //cout << "After game." << endl;
    }

    al_rest(1);
    return 0;
}

