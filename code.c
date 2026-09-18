#include <Adafruit_NeoPixel.h>
#include <Servo.h>

// =====================================================
// GRANDE LED RONDE / ANNEAU
// =====================================================

#define ANNEAU_PIN 6
#define NOMBRE_LEDS 12

Adafruit_NeoPixel anneau(
  NOMBRE_LEDS,
  ANNEAU_PIN,
  NEO_GRB + NEO_KHZ800
);


// =====================================================
// INTERRUPTEUR + PETITE LED
// =====================================================

#define INTERRUPTEUR_PIN 2
#define PETITE_LED_PIN 7

bool petiteLedEtat = false;

unsigned long dernierClignotement = 0;
const unsigned long periodeClignotement = 500;


// =====================================================
// SERVOMOTEUR SG90
// =====================================================

#define SERVO_PIN 9

Servo moteur;

int positionServo = 0;
int directionServo = 1;

unsigned long dernierMouvementServo = 0;

// Vitesse du servo
const unsigned long vitesseServo = 15;


// =====================================================
// ANNEAU : CHANGEMENT DE COULEUR
// =====================================================

unsigned long dernierChangementCouleur = 0;
const unsigned long periodeCouleur = 1000;

int couleur = 0;


// =====================================================
// SETUP
// =====================================================

void setup() {

  // -----------------------------
  // Anneau LED
  // -----------------------------

  anneau.begin();
  anneau.clear();
  anneau.show();


  // -----------------------------
  // Interrupteur
  // -----------------------------

  pinMode(INTERRUPTEUR_PIN, INPUT_PULLUP);


  // -----------------------------
  // Petite LED
  // -----------------------------

  pinMode(PETITE_LED_PIN, OUTPUT);
  digitalWrite(PETITE_LED_PIN, LOW);


  // -----------------------------
  // Servo
  // -----------------------------

  moteur.attach(SERVO_PIN);

  // Dès que le circuit est alimenté,
  // le servo commence à 0°
  moteur.write(0);
}


// =====================================================
// LOOP
// =====================================================

void loop() {


  // ===================================================
  // PETITE LED
  // CONTRÔLÉE PAR L'INTERRUPTEUR
  // ===================================================

  if (digitalRead(INTERRUPTEUR_PIN) == LOW) {

    // Petite LED ON → clignotement
    if (millis() - dernierClignotement >= periodeClignotement) {

      dernierClignotement = millis();

      petiteLedEtat = !petiteLedEtat;

      digitalWrite(PETITE_LED_PIN, petiteLedEtat);
    }

  } else {

    // Interrupteur OFF → LED éteinte
    digitalWrite(PETITE_LED_PIN, LOW);

    petiteLedEtat = false;
  }


  // ===================================================
  // SERVO
  // INDÉPENDANT DE L'INTERRUPTEUR
  // ===================================================

  if (millis() - dernierMouvementServo >= vitesseServo) {

    dernierMouvementServo = millis();

    // Changer progressivement la position
    positionServo += directionServo;


    // Arrivé à 180°
    if (positionServo >= 180) {

      positionServo = 180;

      directionServo = -1;
    }


    // Arrivé à 0°
    if (positionServo <= 0) {

      positionServo = 0;

      directionServo = 1;
    }

    moteur.write(positionServo);
  }


  // ===================================================
  // GRANDE LED RONDE
  // ===================================================

  if (millis() - dernierChangementCouleur >= periodeCouleur) {

    dernierChangementCouleur = millis();

    couleur++;

    if (couleur > 3) {
      couleur = 0;
    }


    // ROUGE
    if (couleur == 0) {

      anneau.fill(
        anneau.Color(255, 0, 0)
      );
    }


    // VERT
    else if (couleur == 1) {

      anneau.fill(
        anneau.Color(0, 255, 0)
      );
    }


    // BLEU
    else if (couleur == 2) {

      anneau.fill(
        anneau.Color(0, 0, 255)
      );
    }


    // JAUNE
    else if (couleur == 3) {

      anneau.fill(
        anneau.Color(255, 255, 0)
      );
    }

    anneau.show();
  }
}