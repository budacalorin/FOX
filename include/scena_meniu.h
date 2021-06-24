#ifndef SCENA_MENIU
#define SCENA_MENIU
#include <SFML/Graphics.hpp>

class scena_meniu{
public:
    int numar_optiuni=0;
    int optiune_curenta=1;
    bool up=0,down=0,left=0,right=0,enter=0;;
    char optiuni[10][15];
    int numar_levele;
    int level_last_update=-3;
    int level=-3;
    unsigned int numar_stele[20]={0};
    int tip_meniu;//1-main meniu   2-optiuni  3-levels
    int setup=1;
    int numar_setups=0;
    float setups[30][15];
    int fps_setups[20];
    int numar_fps_setups;
    int indice_fps_curent=1;
    bool audio_status;
    bool coins[20]={0};
    sf::Color culoare_text;
    sf::Color culoare_text_selectat;

    void adaugare_optiune(const char *s);

    void adaugare_setup (int x,int y,float forta_frecare,float gravitatia,float viteza_playerul,float viteza_masinei,float propulsie,float coeficient_sarituri,int sprite_offsett,int masina_offset_x,int masina_offset_y);

    void adaugare_fps_setup(int x);

    void init_setups(int ind);

    void update();

    void draw();

    void update_background();

private:
    void afisare_optiuni();

    void actiune();

    bool input_tastatura();

    void deplasare();

    void draw_stele();

    void reset();

}
;

#endif // SCENA_MENIU
