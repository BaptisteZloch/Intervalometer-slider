#include <Arduino.h>
#include "LittleFS.h"          //nouveau système de gestion de fichier
#include <ESP8266WiFi.h>       //wifi
#include "ESPAsyncWebServer.h" //webserver asynchrone
#include "myWifi.h"

const char *ssid = "ZTA_ssid";
const char *ssid_maison = ssid_devolo;
const char *mdp_maison = password_devolo;

String nbposes;
String tpsposes;
String longueur;
String angle;
boolean SensDefaut;  //gauche vers la droite
boolean SensHoraire; //sens horaire par défaut
unsigned long Start; //millis du début de la prise de vue
int increment = 0;

boolean dispo = false;

AsyncWebServer server(80); //déclaration du serveur

void setup()
{
  Serial.begin(115200);
  //-------------------WIFI_AP--------------------------
  Serial.println("\nCréation du point d'accès ... ");
  WiFi.disconnect(true);
  if (WiFi.softAP(ssid)) //create Access point on esp8266
  {
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP()); //print the IP that we have to connect
  }
  else
  {
    Serial.println("Failed!");
  }
  WiFi.begin(ssid_maison, mdp_maison);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nDevolo-Zwifi IP address = ");
  Serial.println(WiFi.localIP());

  if (LittleFS.begin())
  {
    Serial.println(F("LittleFS system mounted with success"));
  }
  else
  {
    Serial.println(F("An Error has occurred while mounting LittleFS"));
  }
  Serial.println("List files :");
  Dir dir = LittleFS.openDir("/");
  while (dir.next())
  {
    Serial.println(dir.fileName());
  }

  //-------------------SERVER--------------------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.on("/video.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/video.html", "text/html");
  });
  server.on("/photo.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/photo.html", "text/html");
  });
  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/w3.css", "text/css");
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/script.js", "text/javascript");
  });
  server.on("/jquery-3.5.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/jquery-3.5.1.min.js", "text/javascript");
  });

  server.on("/envoi_video", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP_POST");
    if (request->hasParam("angle", true))
    {
      angle = request->getParam("angle", true)->value();
      angle = angle.toInt();
      Serial.print("Angle :");
      Serial.println(angle);
    }
    if (request->hasParam("longueur", true))
    {
      longueur = request->getParam("longueur", true)->value();
      longueur = longueur.toInt();
      Serial.print("Longueur :");
      Serial.println(longueur);
    }
    if (request->hasParam("tpsposes", true))
    {
      tpsposes = request->getParam("tpsposes", true)->value();
      tpsposes = tpsposes.toInt();
      Serial.print("Temps de pose :");
      Serial.println(tpsposes);
    }
    dispo = true;
    request->send(204);
  });

  server.on("/envoi_photo", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP_POST");
    if (request->hasParam("angle", true))
    {
      angle = request->getParam("angle", true)->value();
      angle = angle.toInt();
      Serial.print("Angle :");
      Serial.println(angle);
    }
    if (request->hasParam("longueur", true))
    {
      longueur = request->getParam("longueur", true)->value();
      longueur = longueur.toInt();
      Serial.print("Longueur :");
      Serial.println(longueur);
    }
    if (request->hasParam("tpsposes", true))
    {
      tpsposes = request->getParam("tpsposes", true)->value();
      tpsposes = tpsposes.toInt();
      Serial.print("Temps de pose :");
      Serial.println(tpsposes);
    }
    if (request->hasParam("nbposes", true))
    {
      nbposes = request->getParam("nbposes", true)->value();
      nbposes = nbposes.toInt();
      Serial.print("Nombre de poses :");
      Serial.println(nbposes);
    }
    if (request->hasParam("DefautSens", true))
    {
      SensDefaut = request->getParam("DefautSens", true)->value();
      Serial.print("Sens de gauche à droite : ");
      Serial.println(SensDefaut);
    }
    if (request->hasParam("SensHoraire", true))
    {
      SensHoraire = request->getParam("SensHoraire", true)->value();
      Serial.print("Sens horaire : ");
      Serial.println(SensHoraire);
    }
    dispo = true;
    Start = millis();
    request->send(204);
  });

  server.begin();
  Serial.println("\nServer actif !");
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
}

void loop()
{
  if (dispo)
  {
    if (increment < nbposes.toInt())
    {
      Serial.println("En cours de poses...");
      digitalWrite(D4, LOW);
      delay(tpsposes.toInt() * 1000);
      digitalWrite(D4, HIGH);
      increment += 1;
      dispo = true;
    }
    else
    {
      increment = 0;
      Serial.println("Fini les poses !");
      dispo = false;
    }
  }
  delay(500);
  digitalWrite(D4, HIGH);
}