#include <main.h>

coordoneCartesient emplacementRelleRobot,emplacemtnOjectifRobot;
coordonePolair sortieEcodeur;
int impultionDroit=0,impultionGauche=0;
float xRobot=0,yRobot=0,aRobot=0;
//---------------------------variable provisoire---------------------------//
float passe(0),tigtd=0;
float tigtg=0;
boolean bp(false);
coordoneCartesient erreurCar;
coordonePolair erreurPol,consigne;
void setup() {
  //inicialisation de la liaison serial
  Serial1.begin(9600);
  //----------------initialisationdes entrée Sortie----------------//
  //led 13
  pinMode(LED_13,OUTPUT);
  digitalWrite(LED_13,HIGH);
  //inicialisation des entrée sortie morteur
  pinMode(ENCODER_DROIT_A,INPUT_PULLUP);
  pinMode(ENCODER_DROIT_X,INPUT_PULLUP);
  pinMode(ENCODER_DROIT_B,INPUT_PULLUP);
  pinMode(MOTEUR_DROIT_IN1,OUTPUT);
  pinMode(MOTEUR_DROIT_IN2,OUTPUT);

  pinMode(ENCODER_GAUCHE_A,INPUT_PULLUP);
  pinMode(ENCODER_GAUCHE_X,INPUT_PULLUP);
  pinMode(ENCODER_GAUCHE_B,INPUT_PULLUP);
  pinMode(MOTEUR_GAUCHE_IN3,OUTPUT);
  pinMode(MOTEUR_GAUCHE_IN4,OUTPUT);
  //inicialisation du bouton
  pinMode(BOUTON,INPUT_PULLUP);
  //-----------------inicialisation des fonction d'interuption---------------------------------//
  attachInterrupt(digitalPinToInterrupt(ENCODER_DROIT_A),contageEncodeurDroit,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_GAUCHE_A),contageEncodeurGauche,RISING);
  MsTimer2::set(DT,calculAsservisement);
  MsTimer2::start();


  analogWriteFrequency(MOTEUR_DROIT_ENA, 30000);
  analogWriteFrequency(MOTEUR_GAUCHE_ENA, 30000);


  //-----------------inicialisation des structure-------------------//
  emplacemtnOjectifRobot.x=0;
  emplacemtnOjectifRobot.y=0;
  emplacemtnOjectifRobot.a=0;

  emplacementRelleRobot.x=0;
  emplacementRelleRobot.y=0;
  emplacementRelleRobot.a=0;
}

void loop() {
  if(!digitalRead(BOUTON)){
    Serial1.print(consigne.r);
    Serial1.print(" ");
    Serial1.println(consigne.a);
  }
}
void calculAsservisement(){
  coordonePolair parcouru,delta0,delta1;
  int PWMGauche=50,PWMDroit=100;
  parcouru=convertionEncodeur();
  cartesien(parcouru);
  delta1=delta0;
  delta0=erreur();
  consigne=regule(delta0,delta1);
  if(consigne.r>10){
    if(consigne.r>61)consigne.r=61;
    else if(consigne.r<-61)consigne.r=-61;
    if(consigne.a>61)consigne.a=61;
    else if(consigne.a<-61)consigne.a=-61;
    if((consigne.r-consigne.a)>0){
      digitalWrite(MOTEUR_DROIT_IN2,LOW);
      digitalWrite(MOTEUR_DROIT_IN1,HIGH);
    }else{
      digitalWrite(MOTEUR_DROIT_IN1,LOW);
      digitalWrite(MOTEUR_DROIT_IN2,HIGH);
    }
    analogWrite(MOTEUR_DROIT_ENA,130+abs(consigne.r-consigne.a));
  //  analogWrite(MOTEUR_DROIT_ENA,130);
    if((consigne.r+consigne.a)>0){
      digitalWrite(MOTEUR_GAUCHE_IN4,HIGH);
      digitalWrite(MOTEUR_GAUCHE_IN3,LOW);
    }else{
      digitalWrite(MOTEUR_GAUCHE_IN4,LOW);
      digitalWrite(MOTEUR_GAUCHE_IN3,HIGH);
    }
    analogWrite(MOTEUR_GAUCHE_ENA,130+abs(consigne.r+consigne.a));
    //analogWrite(MOTEUR_GAUCHE_ENA,130);
  }else{
    analogWrite(MOTEUR_DROIT_ENA,0);
    analogWrite(MOTEUR_GAUCHE_ENA,0);
  }
}
coordonePolair regule(coordonePolair erreur0,coordonePolair erreur1){
  coordonePolair assair;
  assair.r=erreur0.r*KPG+(erreur0.r+erreur1.r)*KIG+(erreur0.r-erreur1.r)*KDG;
  assair.a=erreur0.a*KPT+(erreur0.a+erreur1.a)*KIT+(erreur0.a-erreur1.a)*KDT;
  return assair;
}
coordonePolair erreur(){
  erreurCar.x=emplacemtnOjectifRobot.x-emplacementRelleRobot.x;
  erreurCar.y=emplacemtnOjectifRobot.y-emplacementRelleRobot.y;
  erreurPol.r=sqrt(erreurCar.x*erreurCar.x+erreurCar.y*erreurCar.y);
  erreurPol.a=0;
  if(erreurCar.y!=0){
    erreurPol.a=atan2(erreurCar.x,erreurCar.y);
  }
  else{
    if(erreurCar.x>0)erreurPol.a=PI/2;
    erreurPol.a=(3*PI)/2;
  }
  erreurPol.a+=(3*PI/2);
  erreurPol.a*=-1;
  erreurPol.a-=emplacementRelleRobot.a;
  return erreurPol;
}
void cartesien(coordonePolair dernierDep){
  if(emplacementRelleRobot.a<0)emplacementRelleRobot.a+=2*PI;
  emplacementRelleRobot.x+=(dernierDep.r*((float)cos(emplacementRelleRobot.a)));
  emplacementRelleRobot.y+=(dernierDep.r*((float)sin(emplacementRelleRobot.a)));
  emplacementRelleRobot.a+=(dernierDep.a*PI);
  while((emplacementRelleRobot.a>2*PI||emplacementRelleRobot.a<-2*PI)){
    if(emplacementRelleRobot.a>0)emplacementRelleRobot.a-=2*PI;
    else emplacementRelleRobot.a+=2*PI;
  }
}
coordonePolair convertionEncodeur(){
  float impD=0,impG=0;
  impD=impultionDroit;
  impultionDroit=0;
  impG=impultionGauche;
  impultionGauche=0;
  impD*=COEF_RD;
  impG*=COEF_RG;
  sortieEcodeur.r=impD+impG;
  sortieEcodeur.a=((impD-impG)*COEF_ROT);
  return sortieEcodeur;
}
void contageEncodeurDroit(){
  if(digitalRead(ENCODER_DROIT_B))impultionDroit++;
  else impultionDroit--;
}

void contageEncodeurGauche(){
  if(digitalRead(ENCODER_GAUCHE_B))impultionGauche--;
  else impultionGauche++;
}
