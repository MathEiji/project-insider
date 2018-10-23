#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "allegroTeste.h"

#define screen_wide 1280
#define screen_length 720

ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_BITMAP *image = NULL; //Being used in game_run
ALLEGRO_BITMAP *background = NULL; //Being used in game_run
ALLEGRO_BITMAP *character = NULL; //Being used in game_run
ALLEGRO_BITMAP *pet_character = NULL;
ALLEGRO_EVENT_QUEUE *event_line = NULL; //Being used in game_menu
ALLEGRO_EVENT_QUEUE *event_line_game = NULL; //For game_run
ALLEGRO_EVENT event; //Being used in game_menu
ALLEGRO_EVENT event_game; //For game_run
ALLEGRO_SAMPLE *loading_exit_sound = NULL;
ALLEGRO_SAMPLE *menu_sound = NULL;
ALLEGRO_AUDIO_STREAM *game_music = NULL; //Not used yet.
bool exit_game = false;
bool start_menu = false;
bool go_to_menu = false;
int level_select = NULL;
int x_character = 0;
int y_character = 0;
int select_character = 1;
int x_pet_character = 0;
int y_pet_character =0;

void destroy(void){ // Destroys all the variables
    al_destroy_display(window);
    al_destroy_bitmap(image);
    al_destroy_bitmap(character);
    al_destroy_bitmap(pet_character);
    al_destroy_bitmap(background);
    al_destroy_event_queue(event_line);
    al_destroy_event_queue(event_line_game);
    al_destroy_audio_stream(game_music);
    al_destroy_sample(loading_exit_sound);
    al_destroy_sample(menu_sound);
    al_uninstall_keyboard();
    al_uninstall_audio();
}

bool audio_initiation(){
    if (!al_install_audio()){
        printf("Problem installing audio.\n");
        return false;
    }

    if (!al_init_acodec_addon()){
        printf("Problem starting audio codex.\n");
        return false;
    }

    if (!al_reserve_samples(100)){
        printf("Problem allocating an audio channel.\n");
        return false;
    }
    return true;
}

bool initializing_commands(void){ //Initializes the commands of Allegro
    if(!al_init()){
        printf("Problem initializing allegro.\n");
        return false;
    }
    if(!al_init_image_addon()){
        printf("Problem initializing imageAddon.\n");
        return false;
    }
    if(!al_install_keyboard()){
        printf("Problem installing keyboard.\n");
        return false;
    }
    if(!audio_initiation()){
        printf("Problem insALLEGRO_PLAYMODE_ONCEtalling audio system.\n");
        return false;
    }
    return true;
}

bool loading_screen(void){ //Inserts the first image on the window and creates a loading screen.
    image = al_load_bitmap("images/initialImage1.jpg");
     if(!image){ //Kinda optional
        printf("Problem finding image.\n");
        destroy();
        return false;
        }
    al_draw_bitmap(image, 0, 0, 0);
    al_flip_display();
    while(!start_menu){ //A basic loop to create the sense of motion in the loading screen.
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
            loading_exit_sound = al_load_sample("songs/OOT_Secret.wav");
            al_play_sample(loading_exit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            printf("\njogo encerrado.\n");
            start_menu = true;
            exit_game = true;
        }
        /*else{
                insert a way to clear the event line!!!!
        }*/
    }
    image = al_load_bitmap("images/menu1.jpg");
    al_draw_bitmap(image, 0, 0, 0);
    al_flip_display();
    return true;
}


bool display_creation(void){ //Creates the display(window)
    window = al_create_display(screen_wide,screen_length);
    if(!window){
        printf("Probem initializing window.\n");
        return false;
        destroy();
    }
    return true;
}

bool line_of_events(void){ //Creates the line of events
    event_line = al_create_event_queue();
    event_line_game = al_create_event_queue();
    if(!event_line || !event_line_game){
        printf("Problem creating event queue.\n");
        destroy();
        return false;
    }
    return true;
}


bool gathering(void){ // Starts initializing commands, display_creation and line of events.
    if(!initializing_commands()){
        printf("Problem initializing commands.\n");
        return false;
        destroy();
    }
    if(!display_creation()){
        printf("Problem creating display.\n");
        return false;
        destroy();
    }
    if(!line_of_events()){
        printf("Problem creating event line.\n");
        return false;
        destroy();
    }
    return true;
}

void game_run_movement(){ //Character movement program.
    go_to_menu=false; //So that it can always execute the While.
    background = al_load_bitmap("images/fase1test.png"); //Assimilates the bitmap to the image.
    character = al_load_bitmap("images/characterTest.png"); //Assimilates the bitmap to the image.
    pet_character = al_load_bitmap("images/petTest.png");
    al_draw_bitmap(image, 0, 0, 0);
    al_draw_bitmap(character,x_character,y_character,0);
    al_draw_bitmap(pet_character,x_pet_character,y_pet_character,0);
    al_flip_display();
    while(!go_to_menu){ //Basically the loop responsible for the character motion.
        al_wait_for_event(event_line_game, &event_game);
        if(event_game.type== ALLEGRO_EVENT_DISPLAY_CLOSE){
            printf("\nMenu.\n");
            go_to_menu=true;
            exit_game=true;
        }
        if(event_game.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(event_game.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    printf("Tecla 'Seta para cima' foi pressionada\n");
                    if(y_character>=10 && select_character==0){
                        y_character=y_character-10;
                    }
                    if(y_pet_character>=10 && select_character==1){
                        y_pet_character=y_pet_character-10;
                    }
                    break;

                case ALLEGRO_KEY_DOWN:
                    printf("Tecla 'Seta para baixo' foi pressionada\n");
                    if(y_character<=690 && select_character==0){
                        y_character=y_character+10;
                    }
                    if(y_pet_character<=690 && select_character==1){
                        y_pet_character=y_pet_character+10;
                    }
                    break;

                case ALLEGRO_KEY_RIGHT:
                    printf("Tecla 'Seta para a direita' foi pressionada\n");
                    if(x_character<=1260 && select_character==0){
                        x_character=x_character+10;
                    }
                    if(x_pet_character<=1260 && select_character==1){
                        x_pet_character=x_pet_character+10;
                    }
                    break;

                case ALLEGRO_KEY_LEFT:
                    printf("Tecla 'Seta para a esquerda' foi pressionada\n");
                    if(x_character>=10 && select_character==0){
                        x_character=x_character-10;
                    }
                    if(x_pet_character>=10 && select_character==1){
                        x_pet_character=x_pet_character-10;
                    }
                    break;

                case ALLEGRO_KEY_SPACE:
                    printf("Tecla 'espaco' foi pressionada\n");
                    if(select_character==0){
                        select_character=1;
                    }
                    else if(select_character==1){
                        select_character=0;
                    }
                    break;


                case ALLEGRO_KEY_ESCAPE:
                    printf("Tecla 'ESC' foi pressionada\n");
                    go_to_menu=true;
                    al_unregister_event_source(event_line_game,al_get_keyboard_event_source());
                    break;

                case ALLEGRO_KEY_ENTER:
                    printf("\nX:%d\nY:%d",x_character,y_character);
                    break;

            }
        }
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap(character,x_character,y_character,0);
        al_draw_bitmap(pet_character,x_pet_character,y_pet_character,0);
        al_flip_display();
    }
}

/*int game_run_level(int count_menu){ //Level selection, work in progress.
    int x_temp_character,y_temp_character,x_temp_pet,y_temp_pet;
    if(count_menu==1){

    }
    else if(count_menu==2){
        level_select=0;
    }
}*/

void game_run(void){ //The game run.
    al_register_event_source(event_line_game, al_get_keyboard_event_source());
    al_register_event_source(event_line_game, al_get_display_event_source(window));
    game_run_movement();
}

void game_menu(void){
    int count_menu=1;
    if(!gathering()){
        printf("problem with gathering.\n");
        destroy();
    }
    menu_sound = al_load_sample("songs/OOT_MainMenu_Cursor.wav");
    al_register_event_source(event_line, al_get_keyboard_event_source());
    al_register_event_source(event_line, al_get_display_event_source(window));
    loading_screen();
    while(!exit_game){
        while(!al_is_event_queue_empty(event_line)){
            al_wait_for_event(event_line, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(event.keyboard.keycode){ //Reacts according to the pressed key
                    case ALLEGRO_KEY_UP:
                        printf("Tecla 'Seta para cima' foi pressionada\n");
                        al_play_sample(menu_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
                        al_play_sample(menu_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
                                printf("'Continue' was selected.\n");
                                image = al_load_bitmap("images/menu1.jpg");
                                al_unregister_event_source(event_line,al_get_keyboard_event_source()); //Basically serves as way to stop event_line from getting the input,while in game_run.
                                game_run();
                                al_register_event_source(event_line, al_get_keyboard_event_source());
                                break;

                            case 2:
                                printf("'Start' was selected.\n");
                                image = al_load_bitmap("images/menu2.jpg");
                                x_character=0,y_character=0,x_pet_character=0,y_pet_character=0;
                                al_unregister_event_source(event_line,al_get_keyboard_event_source()); //Basically serves as way to stop event_line from getting the input,while in game_run.
                                game_run();
                                al_register_event_source(event_line, al_get_keyboard_event_source());
                                break;

                            case 3:
                                printf("'Options' was selected.\n");
                                image = al_load_bitmap("images/menu3.jpg");
                                break;

                        }
                        al_draw_bitmap(image, 0, 0, 0);
                        al_flip_display();
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        printf("Tecla 'ESC' foi pressionada\n");
                        exit_game=true;
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


