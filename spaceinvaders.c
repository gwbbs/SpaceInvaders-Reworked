#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>

#define LARGHEZZA 41 // X dell'area di gioco
#define ALTEZZA 23 // Y dell'area di gioco
#define MAX_NEMICI 6 // definisce quanti nemici ci sono in una riga
#define VELOCITA_NEMICI_DESTRA 0.5
#define VELOCITA_NEMICI_SINISTRA 0.5

int vite = 1;
int monete = 10000;

float posizioneNemicoX[MAX_NEMICI][MAX_NEMICI]; // matrice bidimensionale che tiene traccia della X dei nemici
float posizioneNemicoY[MAX_NEMICI][MAX_NEMICI]; // matrice bidimensionale che tiene traccia della Y dei nemici
bool nemicoAttivo[MAX_NEMICI][MAX_NEMICI]; 
static int direzioneNemici = 1;

bool proiettileAttivo; // booleana che indica se il proiettile è attivo o meno
int posizioneGiocatoreX, posizioneGiocatoreY;
int posizioneProiettileX, posizioneProiettileY;
int punteggio;

void inizializza() {
    posizioneGiocatoreX = LARGHEZZA / 2;
    posizioneGiocatoreY = ALTEZZA - 1;
    for (int i = 0; i < MAX_NEMICI; i++) {
        for (int j = 0; j < MAX_NEMICI; j++) {
            posizioneNemicoX[i][j] = j * 4; // modifica spazi per l'asse X
            posizioneNemicoY[i][j] = i * 1; // modifica spazi per l'asse Y
            nemicoAttivo[i][j] = true;
        }
    }

    proiettileAttivo = false;
}

void sistemaPunteggioMonete() {
    monete += 5;
    punteggio += 10;
}

void muoviNemici() {
    static int contatore = 0;
    static int faseMovimento = 0; // 0: destra, 1: scendere, 2: sinistra
    static int drop = 0;

    if (contatore % 2 == 0) {
        drop = 1;
    } else {
        drop = 0;
    }

    for (int i = 0; i < MAX_NEMICI; i++) {
        for (int j = 0; j < MAX_NEMICI; j++) {
            if (nemicoAttivo[i][j]) {
                // Movimento a destra
                if (faseMovimento == 0) {
                    if (drop) {
                        posizioneNemicoY[i][j] += 1;
                    }

                    posizioneNemicoX[i][j] += 1; // Ridotta la velocità
                    if (posizioneNemicoX[i][j] >= LARGHEZZA - 4) {
                        posizioneNemicoX[i][j] = LARGHEZZA - 5;
                        faseMovimento = 1; // Passa alla fase di scendere
                    }
                }
                // Scendere
                else if (faseMovimento == 1) {
                    posizioneNemicoY[i][j] += 1; // Ridotta la velocità
                    if (posizioneNemicoY[i][j] >= ALTEZZA - 1) {
                        posizioneNemicoY[i][j] = ALTEZZA - 2;
                        faseMovimento = 2; // Passa alla fase di sinistra
                    }
                }
                // Movimento a sinistra
                else if (faseMovimento == 2) {
                    if (drop) {
                        posizioneNemicoY[i][j] += 1;
                    }

                    posizioneNemicoX[i][j] -= 1; // Ridotta la velocità
                    if (posizioneNemicoX[i][j] <= 0) {
                        posizioneNemicoX[i][j] = 0;
                        faseMovimento = 0; // Passa alla fase di destra
                    }
                }
            }
        }
    }

    contatore++;
}

void partita() {
    system("cls"); // cancella il terminale
    printf("Punteggio: %d\tVite: %d", punteggio, vite);
    printf("\n");
    // sistema spawn dei nemici 
    for (int i = 0; i < ALTEZZA; i++) {
        for (int j = 0; j < LARGHEZZA; j++) {
            if (i == posizioneGiocatoreY && j == posizioneGiocatoreX) {
                printf("-A-"); // stampa il giocatore 
            } else {
                bool nemicoStampato = false;
                for (int k = 0; k < MAX_NEMICI; k++) {
                    for (int l = 0; l < MAX_NEMICI; l++) {
                        if (nemicoAttivo[k][l] && i == posizioneNemicoY[k][l] && j == posizioneNemicoX[k][l]) {
                            printf("N"); // stampa i nemici 
                            nemicoStampato = true;
                            break;
                        }
                    }
                    if (nemicoStampato) {
                        break;
                    }
                }
                if (!nemicoStampato && proiettileAttivo && i == posizioneProiettileY && j == posizioneProiettileX) {
                    printf("|"); // stampa il proiettile 
                } else if (!nemicoStampato) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void reset() {
    punteggio = 0;
}


//--------- PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER PLAYER
void aggiornaProiettile() {
    if (proiettileAttivo) {
        posizioneProiettileY--;
        // 
        for (int i = 0; i < MAX_NEMICI; i++) {
            for (int j = 0; j < MAX_NEMICI; j++) {
                if (nemicoAttivo[i][j] && posizioneProiettileY == posizioneNemicoY[i][j] && posizioneProiettileX == posizioneNemicoX[i][j]) { 
                    nemicoAttivo[i][j] = false;
                    proiettileAttivo = false;
                    sistemaPunteggioMonete();
                    return;
                }
            }
        }

        // Controlla se il proiettile ha raggiunto la cima dello schermo
        if (posizioneProiettileY <= 0) {
            proiettileAttivo = false;
        }
    }
}
void sparaProiettile() {
    if (!proiettileAttivo) {
        posizioneProiettileX = posizioneGiocatoreX;
        posizioneProiettileY = posizioneGiocatoreY - 4; // Determina velocità proiettile
        proiettileAttivo = true;
    }
}

void movimentoGiocatore(char tasto) {
    if (tasto == 'a' && posizioneGiocatoreX > 0) {
        posizioneGiocatoreX -= 2;
    } else if (tasto == 'd' && posizioneGiocatoreX < LARGHEZZA-1) {
        posizioneGiocatoreX += 2;
    }
}

void Giocatore(char tasto) {
    if (_kbhit()) {
        tasto = _getch();
        if (tasto == 'a' && posizioneGiocatoreX > 0) {
            movimentoGiocatore(tasto);
        } else if (tasto == 'd' && posizioneGiocatoreX < LARGHEZZA - 1) {
            movimentoGiocatore(tasto);
        } else if (tasto == ' ') {
            sparaProiettile();
        }
    }
}

void gioca(){
    char tasto =' ';
    bool fineGioco = false;

    while (!fineGioco) {
        partita();
        aggiornaProiettile();
        Giocatore(tasto);
        muoviNemici();
        if (GetAsyncKeyState('C') & 0x8001) {
            fineGioco = true;
            reset();
        }

        Sleep(30); // frequenza d'aggiornamento
    }
}

//--------- MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU MENU


void apriNegozio() {
    char negoziochoose = '\0';

    // loop che permette di aggiornare il negozio e il counter delle monete 
    while (true) {
        system("cls");
        printf("Monete: %d\n\n", monete);
        printf("Potenziamenti:\n\n");

        printf("\t1 ~Velocita' Proiettili (100)\n");
        printf("\tAumenta la velocita' del proiettile di +1.\n\n");

        printf("\t2 ~Vita Extra (400)\n");
        printf("\tAumenta le vite del giocatore.\n\n");

        printf("\t3 ~Boost Velocita' (400)\n");
        printf("\tAumenta la velocita' del player.\n\n");

        printf("Premi 'ESC' per tornare al menu principale.");

        negoziochoose = _getch();
        
        if (negoziochoose == '1' && monete >= 100) {
            monete -= 100;
            // Aggiorna velocità proiettile
        } else if (negoziochoose == '2' && monete >= 400) {
            monete -= 400;
            vite += 1;
            // Aggiorna vite giocatore
        } else if (negoziochoose == '3' && monete >= 500) {
            monete -= 500;
            // Aggiorna velocità giocatore
        } else if (negoziochoose == 27) {  // 'ESC'
            break;  
        }
    }
}

void menu() {
    system("cls");
    printf("Benvenuto nel menu:\n");
    printf("1. Avvia la partita\n");
    printf("2. Apri il negozio\n");
    printf("3. Esci\n");

    char scelta = _getch();

    switch (scelta) {
        case '1':
            inizializza();
            gioca();
            break;
        case '2':
            apriNegozio();
            _getch();
            break;
        case '3':
            exit(0);
            break;
        default:
            break;
    
    }
}

//--------- GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO GIOCO 

int main(){
    srand((unsigned int)time(NULL));

    while (true) {
        menu();
    }

    return 0;
}