/*
Criado por Gustavo Silveira, a.k.a. Músico Nerd, para o projeto
"Escrevendo Palavras com o Piscar dos Olhos", de Paulo Vitor Loureiro
na residência da Red Bull Basement.
http://www.musiconerd.com
gustavosilveira@musiconerd.com

Baixe todos os arquivos em: https://github.com/silveirago/teclado-capacitivo-diy

*/

// Adicione as seguintes bibliotecas
#include <CapacitiveSensor.h> // https://github.com/PaulStoffregen/CapacitiveSensor
#include "Keyboard.h" // https://github.com/arduino-libraries/Keyboard

/////////////////////////////////////////////
// botoes
const int Nbotoes = 1; //* Numero de sensores
int buttonCState[Nbotoes] = {0};         // armazena o valor atual do botão
int buttonPState[Nbotoes] = {0};        // armazena o valor anterior do botão

/////////////////////////////////////////////
// debounce
unsigned long lastDebounceTime[Nbotoes] = {0};  // a última vez que o pino de saída foi alternado
unsigned long debounceDelay = 10;    //* o tempo de debounce; aumentar se a saída piscar

/////////////////////////////////////////////
// SENSOR CAPACITIVO
// Use um resistor de alto valor entre os pinos 4 & 2, pino 2 é o pino sensor, adicione um fio e material condutivo se desejado
// Use 100k para ativar com toque absoluto, e aprtir de 1M para ativar a distancia
CapacitiveSensor   sensorCapacitivo[Nbotoes] = {CapacitiveSensor(4, 2)};

//Se for usar mais sensores os adicione no array acima sempre usando o pino 4, como...
//CapacitiveSensor   sensorCapacitivo[3] = {CapacitiveSensor(4, 2), CapacitiveSensor(4,6), CapacitiveSensor(4,8)};

int gatilho = 10; // valor minimo para ativar o sensor
int valorAtual = 0;
long leituraDoSensor[Nbotoes] = {0};

/////////////////////////////////////////////
// Caractere
int caractere[Nbotoes] = {32}; // coloque o(s) valor(es) ASCI desejados :: 32 para barra de espaço

int ledPin = 9; // pino do led

void setup() {

  Keyboard.begin();
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

}

void loop() {

  botoes();
  //Serial.println(leituraDoSensor1); // imprime o valor do sensor

}

/////////////////////////////////////////////
// botoes
void botoes() {

long start = millis();

  for (int i = 0; i < Nbotoes; i++) {
    
    leituraDoSensor[i] =  sensorCapacitivo[i].capacitiveSensor(30); // armazena a leitura do sensor

    if (leituraDoSensor[i] > gatilho) { // tranforma a leitura em 1 ou zero, dependendo do valor do gatilho
      valorAtual = HIGH;
    }
    else {
      valorAtual = LOW;
    }

    buttonCState[i] = valorAtual;

    if ((millis() - lastDebounceTime[i]) > debounceDelay) { // faz o "debounce"

      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();

        if (buttonCState[i] == HIGH) {

          Keyboard.press(caractere[i]); // "aperta" a tecla
          digitalWrite(ledPin, HIGH); // acende o led

          Serial.println("PRESSIONADO");
        }
        else {

          Keyboard.release(caractere[i]); // "solta" a tecla
          digitalWrite(ledPin, LOW); // apaga o LED

          Serial.println("LIBERADO");
        }
        buttonPState[i] = buttonCState[i]; // armazena o estado atual no estado previo
      }
    }
  }
}

