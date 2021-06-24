#ifndef SCENA_JOC
#define SCENA_JOC

#include "player.h"
#include "proiectil.h"
#include "particula.h"
#include "constructor.h"
#include "morisca.h"
#include "coin.h"
#include "plasma_gun.h"
class scena_joc{
public:
    player playerul;
    constructor masina;
    sf::Vector2f pozitie_player;
    sf::Vector2f pozitie_masina;
    bloc blocuri[10000];
    bloc blocuri_masina[100];
    particula particule[5000];
    morisca morisci[20];
    plasma_gun plasma_guns[30];
    coin coinul;
    coin coin_scor;
    sf::RectangleShape camera_coin;
    bool e_in_camera_coin;
    bool e_coin;
    bool coin_colectat;
    int trigers[20];
    int numar_morisci;
    int numar_particule;
    int numar_playeri;
    int numar_blocuri;
    int numar_blocuri_masina;
    int numar_plasma_guns;
    long long timp_precedent;
    bool space_apasat;
    int turn;
    bool is_turn_pressed;
    bool is_r_pressed;
    int numar_level;
    int numar_blocuri_disponibile_masinei[10];
    int level=1;
    unsigned int numar_stele[20]={3};
    bool coins[20]={0};
    unsigned int numar_maxim_stele[20]={0};
    bool aterizat;
    float scale_x;
    float scale_y;
    float particule_timer;
    sf::Image imagine;



    void actiune_masina();

    void coliziuni(bool &moare);

    void update_playeri(float dt);

    void draw();

    void draw_blocuri();

    void verificare_final();

    void update();

    void reset(int x,unsigned int numar_stele[20],bool coinuri[20]);

    void update_events();

    void adaugare_bloc(int x,int y,int size_x,int size_y,sf::Color culoare,bool killer);

    void adaugare_bloc_masina(int x,int y,int size_x,int size_y,sf::Color culoare,int tip);

    void adaugare_morisca(sf::Vector2f pozitie,int numar_palete,sf::Vector2f dimensiune,int switch_time,sf::Texture *textura);

    void draw_particule();

    void update_particule(float x);

    int update_cooldown();

    void update_moristi(int x);

    void f ();

    void actualizare_animatii(int timp_trecut);

    void revive();

    void reset_level();

    void update_turn();

    void init_levels(int x);

    void draw_numar_blocuri_ramase();

    void update_animatii(int x);

    void adaugare_chenar();

    void draw_morisci();

    void coliziune_palete(bool &moare);

    void update_stele();

    void draw_stele();

    void copiere_date(unsigned int x[20],bool coinuri[20]);

    bool verificare_pozitie_bloc(sf::Vector2f pozitie);

    void pop_up_stele();

    void init_hitboxuri(int x);

    void pop_up_text(char txt[350]);

    void adaugare_coin(sf::Vector2f camera_position,sf::Vector2f camera_size,sf::Vector2f coin_position);

    void update_coin(int dt);

    void draw_coin();

    void adaugare_plasma_gun(sf::Vector2f pozitie,sf::Vector2f dimenisune,int unghi,int cooldown_tragere,int cooldown_plasma);

    void update_plasma_guns(int dt);

    void draw_plasma_guns();

    void update_blocuri();

    void playerul_moare();
};


#endif
