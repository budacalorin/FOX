#ifndef INIT
#define INIT


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct numar{
sf::Texture textura;
sf::Texture zece;
int valoare=-1;
};

extern std::string string_font;
extern int tutorial;
extern int hight,width;
extern bool game_on;
extern int scena_curenta;
extern float coeficient_frecare;
extern float gravitatie;
extern int cooldown_puteri;
extern int viata_proiectile;
extern float viteza_proiectile;
extern float coeficient_saritura;
extern int damage_paricula_la_busire_perete;
extern float viteza_player;
extern float viteza_masina;
extern float coeficient_propulsie;
extern int putere_saritura;
extern int inaltime_healthbar;
extern float coeficient_healthbar;
extern int damage_proiectil_player;
extern int hp_player;
extern int inaltime_ground;
extern int dimensiune_proiectile_y;
extern int dimensiune_proiectile_x;
extern int constructor_x;
extern int constructor_y;
extern int sprite_offset;
extern int FPS;
extern int timp_per_frame;
extern int indice_rezolutie;
extern bool audio;
extern int masina_offset_X;
extern int masina_offset_Y;

extern sf::Clock ceas_general;
extern sf::RenderWindow window;
extern sf::RectangleShape border;
extern sf::Texture background_texture;
extern sf::Texture *p_background_texture;
extern sf::RectangleShape background;
extern sf::Texture textura_1;
extern sf::Texture *p_textura_1;
extern sf::Texture textura_2;
extern sf::Texture *p_textura_2;
extern sf::Texture textura_masina;
extern sf::Texture *p_textura_masina;
extern numar numar_blocuri_ramase;
extern sf::Texture *p_textura_blocuri_ramase;
extern sf::Texture textura_bloc;
extern sf::Texture textura_bloc_propulsie;
extern sf::Texture masina_animatie;
extern sf::Texture fox;
extern sf::Texture textura_palete;
extern sf::Texture textura_cer;
extern sf::RectangleShape cer;
extern sf::Texture textura_stea;
extern sf::Texture textura_stea_goala;
extern sf::Texture bloc_blocat;
extern sf::Texture bloc_propulsie_blocat;
extern sf::Texture textura_coin;
extern sf::Texture textura_single_coin;
extern sf::Texture textura_chenar_coin;
extern sf::Texture slash;

extern sf::Music music;
extern sf::Music music_cave;

extern sf::SoundBuffer saritura_buffer;
extern sf::Sound saritura;

extern sf::SoundBuffer adaugare_bloc_buffer;
extern sf::Sound adaugare_bloc_sound;

extern sf::SoundBuffer aterizare_buffer;
extern sf::Sound atezirare;

extern sf::SoundBuffer buffer_coin;
extern sf::Sound sunet_coin;


/*
extern sf::Texture textura_front;
extern sf::Texture textura_mid;
extern sf::Texture textura_back;
extern sf::RectangleShape Front;
extern sf::RectangleShape Mid;
extern sf::RectangleShape Back;
*/




#endif // INIT
