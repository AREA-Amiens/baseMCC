#include <Arduino.h>
#include <MsTimer2.h>
#include <Math.h>
//------------definition des structure--------------------//
typedef struct{
  float x;
  float y;
  float a;
}coordoneCartesient;
typedef struct{
  float r;
  float a;
}coordonePolair;
//------------definition des etrée sortie-----------------//
//definition de la pine 13
#define LED_13 13

//definition des pin partie motrice et encodeur
#define ENCODER_DROIT_A     2
#define ENCODER_DROIT_X     3
#define ENCODER_DROIT_B     4
#define MOTEUR_DROIT_ENA    5
#define MOTEUR_DROIT_IN1    6
#define MOTEUR_DROIT_IN2    7

#define ENCODER_GAUCHE_A   8
#define ENCODER_GAUCHE_X   9
#define ENCODER_GAUCHE_B   10
#define MOTEUR_GAUCHE_ENA  11
#define MOTEUR_GAUCHE_IN3  12
#define MOTEUR_GAUCHE_IN4  14

//definition du bouton
#define BOUTON 15
//-------------defintion des constante--------------------//
#define DT 50//en ms

#define COEF_RD 0.0525994656//coeficient de la roue droit * son coretctifre / 2
#define COEF_RG 0.0524736058//coeficient de la roue gauche * sont corectiffe / 2

#define COEF_ROT 0.0027440507//coeficient pour le calcuele de langle 2 / (pi * l'entraxe)
//-------------coeficient assairvissemnt-----------------//
#define KPG 0.3
#define KIG 0
#define KDG 1

#define KPT 10
#define KIT 0
#define KDT 1
//-------------definition des fonction--------------------//
void calculAsservisement();
//calcule de tous se qui vas sevire a l'asservisement
//stocage dans un tableau es diférente valeur
coordonePolair regule(coordonePolair erreur0,coordonePolair erreur1);
//calcule le PID
coordonePolair erreur();
//calcule de lerreur de du robot renvoie lerreur sou forme coordonée polaire
//les coordone seron directement exploitable pour lasservisement
void cartesien(coordonePolair);
//mait a jour les coordonée catesien du robot
coordonePolair convertionEncodeur();
//convetion des des encodeur en coordonée polaire
void contageEncodeurDroit();
//ajout ou retrée en fonction de lencodeur droit
void contageEncodeurGauche();
//ajout ou retrée en fonction de lencodeur guauche
