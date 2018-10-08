#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "allegroTeste.h"

#define screen_wide 1280
#define screen_length 720

ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_EVENT_QUEUE *event_line = NULL;
ALLEGRO_EVENT event;
bool exit_game = false;
bool start_menu = false;

void destroy(void){ // Destroys all the variables
    al_destroy_display(window);
    al_destroy_bitmap(image);
    al_destroy_event_queue(event_line);
    al_uninstall_keyboard();
}

bool initializing_commands(void){ //Initializes the commands of Allegro
    if(!al_init()){
        printf("Problem initializing allegro.");
        return false;
    }
    if(!al_init_image_addon()){
        printf("Problem initializing imageAddon.");
        return false;
    }
    if(!al_install_keyboard()){
        printf("Problem installing keyboard.");
        return false;
    }
    return true;
}

bool insert_first_image(void){ //Inserts the first image on the window.
    al_register_event_source(event_line, al_get_keyboard_event_source());
    al_register_event_source(event_line, al_get_display_event_source(window));
    image = al_load_bitmap("images/initialImage1.jpg");
    al_draw_bitmap(image, 0, 0, 0);
    al_flip_display();
    while(!start_menu){
        al_get_next_event(event_line, &event);
        image = al_load_bitmap("images/initialImage1.jpg");
        al_draw_bitmap(image, 0, 0, 0);
        al_flip_display();
        al_rest(0.25);
        image = al_load_bitmap("images/initialImage2.jpg");
        al_draw_bitmap(image, 0, 0, 0);
        al_flip_display();
        al_rest(0.25);
        image = al_load_bitmap("images/initialImage3.jpg");
        al_draw_bitmap(image, 0, 0, 0);
        al_flip_display();
        al_rest(0.25);
        if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
            start_menu = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            printf("\njogo encerrado.\n");
            start_menu = true;
            exit_game = true;
        }
    }
    image = al_load_bitmap("images/menu1.jpg");
    if(!image){
        printf("Problem finding image.");
        destroy();
        return false;
        }
    al_draw_bitmap(image, 0, 0, 0);
    al_flip_display();
    return true;
}


bool display_creation(void){ //Creates the display(window)
    window = al_create_display(screen_wide,screen_length);
    if(!window){
        printf("Probem initializing window");
        return false;
        destroy();
    }
    return true;
}

bool line_of_events(void){ //Creates the event line
    event_line = al_create_event_queue();
    if(!event_line){
        printf("Problem creating event queue.");
        destroy();
        return false;
    }
    al_register_event_source(event_line, al_get_display_event_source(window));
    return true;
}


bool gathering(void){ // Starts initializing commands, display_creation and line of events.
    if(!initializing_commands()){
        printf("Problem initializing commands.");
        return false;
        destroy();
    }
    if(!display_creation()){
        printf("Problem creating display.");
        return false;
        destroy();
    }
    if(!line_of_events()){
        printf("Problem creating event line.");
        return false;
        destroy();
    }
    return true;
}

void game_menu(void){
    int count_menu=1;
    if(!gathering()){
        printf("problem with gathering.");
        destroy();
    }
    al_register_event_source(event_line, al_get_keyboard_event_source());
    al_register_event_source(event_line, al_get_display_event_source(window));
    insert_first_image();
    while(!exit_game){
        while(!al_is_event_queue_empty(event_line)){
            al_wait_for_event(event_line, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(event.keyboard.keycode){ //Reacts according to the pressed key
                    case ALLEGRO_KEY_UP:
                        printf("Tecla 'Seta para cima' foi pressionada\n");
                        count_menu--;
                        if(count_menu==1){
                            image = al_load_bitmap("images/menu1.jpg");
                        }
                        if(count_menu==2){
                            image = al_load_bitmap("images/menu2.jpg");
                        }
                        else if(count_menu<1){
                            count_menu=3;
                            image = al_load_bitmap("images/menu3.jpg");
                        }
                        al_draw_bitmap(image, 0, 0, 0);
                        al_flip_display();
                        break;

                    case ALLEGRO_KEY_DOWN:
                        printf("Tecla 'Seta para baixo' foi pressionada\n");
                        count_menu++;
                        if(count_menu==2){
                            image = al_load_bitmap("images/menu2.jpg");
                        }
                        if(count_menu==3){
                            image = al_load_bitmap("images/menu3.jpg");
                        }
                        else if(count_menu>3){
                            count_menu=1;
                            image = al_load_bitmap("images/menu1.jpg");
                        }
                        al_draw_bitmap(image, 0, 0, 0);
                        al_flip_display();
                        break;

                    case ALLEGRO_KEY_ENTER:
                        printf("Tecla 'Seta para baixo' foi pressionada\n");
                        switch(count_menu){
                            case 1:
                                printf("'Continue' was selected.");
                                image = al_load_bitmap("images/teste1.jpg");
                                break;

                            case 2:
                                printf("'Start' was selected.");
                                image = al_load_bitmap("images/teste2.jpg");
                                break;

                            case 3:
                                printf("'Options' was selected.");
                                image = al_load_bitmap("images/teste3.jpg");
                                break;

                        }
                        al_draw_bitmap(image, 0, 0, 0);
                        al_flip_display();
                        break;
                }
            }
            else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                printf("\njogo encerrado.\n");
                exit_game = true;
            }
        }
    }
}


