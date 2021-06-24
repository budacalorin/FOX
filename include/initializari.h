#include "init.h"

int tutorial=-3;
bool game_on=1;
int hight=768;
int width=1366;
int scena_curenta=0;
float coeficient_frecare=0.90;
float gravitatie=0.7;
int cooldown_puteri=500000;
int viata_proiectile=500000;
float viteza_proiectile=7;
int damage_paricula_la_busire_perete=3;
float viteza_player;
float viteza_masina;
float coeficient_propulsie;
float coeficient_saritura;
int putere_saritura=15;
int inaltime_healthbar=30;
float coeficient_healthbar=2.5;
int damage_proiectil_player=10;
int inaltime_ground=498;
int hp_player=200;
int dimensiune_proiectile_x=100;
int dimensiune_proiectile_y=100;
int constructor_x=32;
int constructor_y=32;
int sprite_offset;
int FPS;
int timp_per_frame;
int indice_rezolutie;
bool audio;
int masina_offset_X;
int masina_offset_Y;
std::string string_font;


sf::Texture background_texture;
sf::Texture *p_background_texture;
sf::RectangleShape background;
sf::Texture textura_masina;
sf::Texture *p_textura_masina;
numar numar_blocuri_ramase;
sf::Texture *p_textura_blocuri_ramase;
sf::Texture textura_bloc;
sf::Texture textura_bloc_propulsie;
sf::Texture masina_animatie;
sf::Texture fox;
sf::Texture textura_palete;
sf::Texture textura_cer;
sf::RectangleShape cer;
sf::Texture textura_front;
sf::Texture textura_mid;
sf::Texture textura_back;
sf::RectangleShape Front;
sf::RectangleShape Mid;
sf::RectangleShape Back;
sf::Texture textura_stea;
sf::Texture textura_stea_goala;
sf::Texture bloc_blocat;
sf::Texture bloc_propulsie_blocat;
sf::Texture textura_coin;
sf::Texture textura_single_coin;
sf::Texture textura_chenar_coin;
sf::Texture slash;


sf::Music music;
sf::Music music_cave;

sf::SoundBuffer saritura_buffer;
sf::Sound saritura;

sf::SoundBuffer adaugare_bloc_buffer;
sf::Sound adaugare_bloc_sound;

sf::SoundBuffer aterizare_buffer;
sf::Sound atezirare;

sf::SoundBuffer buffer_coin;
sf::Sound sunet_coin;

