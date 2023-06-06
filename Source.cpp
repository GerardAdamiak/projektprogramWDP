//Gerard Adamiak, Aleksandra Dudzic

#include <allegro5\allegro5.h>

#include <iostream>

#include <allegro5\allegro_primitives.h>

#include <allegro5\allegro_image.h>

#include <cmath>    //wszystkie biblioteki

#include <time.h>

#include <allegro5\allegro_font.h>

#include <allegro5\allegro_ttf.h>

#include <allegro5\allegro_audio.h>

#include <allegro5\allegro_acodec.h>

#include <stdio.h>

#include <cstdlib>

#include <fstream>

#include <string>

#include <ctime>

#define BACKGROUND_FILE "snake.png"
using namespace std;

const int height = 600,width = 800; //wymiary okna

bool generateCoins(int snakeX[], int snakeY[], int coinX, int coinY, int score) {
    bool overlap = false;

    // Check for overlap with snake's body
    for (int i = 0; i < score; i++) {
        if (coinX == snakeX[i] && coinY == snakeY[i]) {
            overlap = true;
            break;
        }
    }

    return overlap;
}
struct Coin {
    int x;
    int y;
};

int main() {
    Coin coin;

    srand(time(NULL)); //zeby bylo randomowo
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_TIMER* blinkTimer = al_create_timer(0.5); // New timer for blinking
 
    ALLEGRO_SAMPLE* MUZ = NULL; //inicjacja muzyczki zbierania pieniazkow

    const float FPS = 60.0;
    const float frameFPS = 15.0;
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_BITMAP* background = NULL; //zainicjowanie tla (wezyk)
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon(); //inicjacja wszystkich potrzebnych dodatkow

    ALLEGRO_DISPLAY* display = al_create_display(800, 600); //tworzenie okna
    ALLEGRO_FONT* font1 = al_load_ttf_font("test3.ttf", 200, 0); //czcionka glownego napisu
    ALLEGRO_FONT* font2 = al_load_ttf_font("samson.ttf", 40, 0); //czcionki do mniejszych napisow
    ALLEGRO_FONT* font4 = al_load_ttf_font("samson.ttf", 80, 0);
    bool done = false, active = false; //zmienne sprawdzajace stany
    bool draw = true;
    int x = 0, y = 0;

    if (!font1 || !font2 || !font4)
        return -2;
    if (!display)
        return -1;
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(blinkTimer)); // Register the blinkTimer
    bool redraw = true;
    bool blink = false; // Variable to control the blinking state
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    al_start_timer(blinkTimer); // Start the blinkTimer
   

    while (1) { //petla do wyswietlania ekranu startowego
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (event.timer.source == blinkTimer) {
                blink = !blink; // Toggle the blink state
                redraw = true;
            }
            else {
                redraw = true;
            }
        }
        else if ((event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) {
            break;
        } //skonczenie petli przy kliknieciu spacji lub zamknieciu okna
        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); //ustawianie kolor tla ekranu glownego na czarny
            background = al_load_bitmap(BACKGROUND_FILE); //ustawianie weza na ekranie glowwnym
            al_draw_bitmap(background, 100, 30, 0); //rysowanie weza

            // Get the text dimensions
            int textWidth1 = al_get_text_width(font1, "SNAKE");
            int textWidth2 = al_get_text_width(font2, "press space to start");
            int textHeight2 = al_get_font_ascent(font2);

            // Calculate the position to center the text
            int textX1 = (800 - textWidth1) / 2;
            int textX2 = (800 - textWidth2) / 2;
            al_draw_text(font1, al_map_rgb(255, 255, 255), textX1, 0, 0, "SNAKE"); //rysowanie glownego napisu
            if (!blink) {
                al_draw_text(font2, al_map_rgb(255, 255, 255), textX2, 0.9 * (600 - textHeight2), 0, "press space to start"); //wyswietlanie tekstu na dole migajacego
            }
            al_flip_display(); //odswiezenie ekranu

            redraw = false;
        }
    }
    if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) { //jezeli zostala nacisnieta spacja, przechodzimy do gry
        al_clear_to_color(al_map_rgb(0, 0, 0)); //wyczyszczenie ekranu do koloru czarnego

        ifstream scoreFile("best_score.txt"); //otwieramy plik zapisujacy najlepszy wynik w historii
        int bestScore; //zmienna zapisujaca najlepszy wynik
        scoreFile >> bestScore; //wpisywanie do zmiennej

        // Close the file
        scoreFile.close(); //zamykamy plik

        

        if (!al_init()) return -1;

        if (!display) return -1;

        if (!al_install_audio()) {
            fprintf(stderr, "failed to initialize audio!\n");
            return -2;
        }

        if (!al_init_acodec_addon()) {
            fprintf(stderr, "failed to initialize audio codecs!\n");
            return -3;
        }

        if (!al_reserve_samples(1)) {
            fprintf(stderr, "failed to reserve samples!\n");
            return -4;
        }

        bool done = false, active = false;
        bool draw = true;
        int x = 0, y = 0;

        al_init_primitives_addon();
        al_install_keyboard();
        al_init_image_addon();
        al_init_font_addon();
        al_init_ttf_addon();

        MUZ = al_load_sample("coinSound.wav");
        if (!MUZ) {
            printf("Audio clip sample not loaded!\n");
            return -5;
        }
        ALLEGRO_BITMAP* headRight = al_load_bitmap("head.png");
        ALLEGRO_BITMAP* headLeft = al_load_bitmap("headLeft.png");
        ALLEGRO_BITMAP* headUp = al_load_bitmap("headUp.png");
        ALLEGRO_BITMAP* headDown = al_load_bitmap("headDown.png");
        ALLEGRO_BITMAP* body = al_load_bitmap("body.png");
        ALLEGRO_BITMAP* coin1 = al_load_bitmap("coinPic.png");

        ALLEGRO_TIMER* timer = al_create_timer(1.0/10.0);//szybkosc gry
        ALLEGRO_TIMER* TimeCounter = al_create_timer(1);//licznik czasu w grze
        ALLEGRO_KEYBOARD_STATE keyState;

        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        al_register_event_source(event_queue, al_get_timer_event_source(timer));
        al_register_event_source(event_queue, al_get_timer_event_source(TimeCounter));
        al_register_event_source(event_queue, al_get_display_event_source(display));

        al_start_timer(timer);
        al_start_timer(TimeCounter);

   
        int timeS = 0;//czas poczatkowy
        int timeF; //czas do wyswietlenia na ekranie koncowym

        enum Direction {
            DOWN,
            LEFT,
            RIGHT,
            UP
        };

        int dir = DOWN;
        int score = 1;
        int lastX;
        int lastY;

        coin.x = 40 * (rand() % 20);
        coin.y = 40 * (rand() % 14) + 40;//poczatkowe miejsce pieniazka

        int snakeT[300], snakeX[300], snakeY[300];
        for (int i = 0; i < 300; i++) {
            snakeT[i] = 0;
            snakeX[i] = 0;
            snakeY[i] = 0;
        }//czyszczenie tablic

        bool menu = false;//czy menu jest wyswietlane
        bool dead = false;//czy jest sie martwym
        while (!done) {

            lastX = x;
            lastY = y;

            ALLEGRO_EVENT events;
            al_wait_for_event(event_queue, &events);

            if (events.type == ALLEGRO_EVENT_KEY_UP) {
                switch (events.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (menu) menu = false, score = 1, timeS = 0;
                    break;
                }
            }
            else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;

            if (events.type == ALLEGRO_EVENT_TIMER) {
                if (events.timer.source == TimeCounter) timeS++;//liczenie czasu
                if (events.timer.source == timer) {

                    al_get_keyboard_state(&keyState);
                    if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && dir != LEFT)
                        dir = RIGHT;
                    else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && dir != RIGHT)
                        dir = LEFT;
                    else if (al_key_down(&keyState, ALLEGRO_KEY_UP) && dir != DOWN)
                        dir = UP;
                    else if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) && dir != UP)
                        dir = DOWN; 
                    else if (al_key_down(&keyState, ALLEGRO_KEY_ENTER) && menu == true)
                        menu = false, score = 1, timeS = 0, x = 400, y = 320;
                    if (menu == false) {
                        if (score != 0) {
                            for (int i = score; i > 0; i--) {
                                snakeX[i] = snakeX[i - 1];
                                snakeY[i] = snakeY[i - 1];
                            }
                            snakeX[0] = lastX;
                            snakeY[0] = lastY;
                        }
                    }

                    switch (dir) {
                    case RIGHT:
                        x = x + 40;
                        break;
                    case LEFT:
                        x = x - 40;
                        break;
                    case UP:
                        y = y - 40;
                        break;
                    case DOWN:
                        y = y + 40;
                        break;
                    }

                    if (x == coin.x && y == coin.y) {

                        if (score > bestScore) {
                            bestScore = score;
                            ofstream newScoreFile("best_score.txt");

                            // Write the new best score to the file
                            newScoreFile << score;

                            // Close the file
                            newScoreFile.close();

                        }
                        score++;
                        al_play_sample(MUZ, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        coin.x = 40 * (rand() % 20);
                        coin.y = 40 * (rand() % 14) + 40;
                        while (generateCoins(snakeX, snakeY, coin.x, coin.y, score) == 1) {
                            coin.x = 40 * (rand() % 20);
                            coin.y = 40 * (rand() % 14) + 40;
                        }//generowanie nowego pieniazka tam gdzie nie ma weza

                        snakeT[score] = 1;
                    }

                    if (menu == false) {
                        for (int i = 0; i < score; i++) {
                            if (x == snakeX[i] && y == snakeY[i] && menu == false) dead = true;
                        }
                        if (x < 0 || x >= 800 || y < 0 || y >= 600 && menu == false) dead = true;
                    }

                    draw = true;

                }
            }

            if (dead && menu == false) {
                menu = true;
                timeF = timeS;
                x = 0, y = 0;
                for (int i = 0; i < 300; i++) {
                    snakeT[i] = 0;
                }
                dead = false;
                dir = DOWN;
            }

            if (draw == true) {
                draw = false;
                if (menu) {
                    x = 0, y = 0;
                    for (int i = 0; i < 300; i++) {
                        snakeT[i] = 0;
                    }
                    al_draw_text(font4, al_map_rgb(255, 255, 255), 20, 100, 0,
                        "Press Enter to Start");
                    al_draw_text(font4, al_map_rgb(255, 255, 255), 100, 200, 0,
                        "Press Esc to Exit");
                    al_draw_textf(font2, al_map_rgb(255, 255, 255), 120, 350, 0,
                        "Coins %i", score - 1);
                    al_draw_textf(font2, al_map_rgb(255, 255, 255), 290, 500, 0,
                        "Best Score %i", bestScore);
                    al_draw_textf(font2, al_map_rgb(255, 255, 255), 460, 350, 0,
                        "Time %i sec", timeF);
                }
                else {
                    al_draw_bitmap(coin1, coin.x, coin.y, 0);
                    for (int i = 0; i < score; i++) {
                        al_draw_bitmap(body, snakeX[i], snakeY[i], NULL);
                    }

                    switch (dir) {
                    case RIGHT:
                        al_draw_bitmap(headRight, x - 10, y, NULL);
                        break;
                    case LEFT:
                        al_draw_bitmap(headLeft, x - 20, y, NULL);
                        break;
                    case UP:
                        al_draw_bitmap(headUp, x, y - 20, NULL);
                        break;
                    case DOWN:
                        al_draw_bitmap(headDown, x, y - 10, NULL);
                        break;
                    }

                    al_draw_textf(font2, al_map_rgb(255, 255, 255), 5, 5, 0,
                        "Coins: %i", score - 1);
                    al_draw_textf(font2, al_map_rgb(250, 255, 255), 625, 5, 0,
                        "Time: %i", timeS);
                }

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }

        }

        al_destroy_sample(MUZ);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return 0;

    }
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_display(display);

    return 0;
}