#include <HCSR04.h>                   // inclui a biblioteca do módulo HC-SR04
#include <LiquidCrystal_I2C.h>       // inclui a biblioteca LCD com I2C

// define pinos de entrada e saída do Arduino
const byte pinTrig = 8; // pino usado para disparar os pulsos do sensor
const byte pinEcho = 9; // pino usado para ler a saida do sensor
const byte pinBut = 2;  // pino usado para o push button

#define col  16 //Define o número de colunas do display utilizado
#define lin   2 //Define o número de linhas do display utilizado
#define ende  0x27 //Define o endereço do display
// instancia objetos das bibliotecas
LiquidCrystal_I2C lcd(ende, col, lin); //Cria o objeto lcd passando como parâmetros o endereço, o nº de colunas e o nº de linhas

// define variáveis globais
byte state = 1;          // estado do menu
byte decimal = 0;        // número de casas decimais
String unity = "cm";     // descrição das unidades
double *result;          // uso do ponteiro para um double (obrigatório)

void setup(){
  // pin mode - definie entradas e saídas digitais
  pinMode(pinBut, INPUT_PULLUP);   // pullup interno do Arduino

  lcd.init(); //Inicializa a comunicação com o display já conectado
  lcd.clear(); //Limpa a tela do display
  lcd.backlight(); //Aciona a luz de fundo do display
  lcd.setCursor(0, 0); //Coloca o cursor do display na coluna 1 e linha 1
  lcd.print("Bora medir?"); //Exibe a mensagem na primeira linha do display
  lcd.setCursor(0, 1); //Coloca o cursor do display na coluna 1 e linha 2
  lcd.print("Ass: Tomate");  //Exibe a mensagem na segunda linha do display

  // inicializa o senor HC-SR04
  HCSR04.begin(pinTrig,pinEcho);
  
  // Inicializa a porta serial
  Serial.begin(9600);

  // inicializa o lcd 16X2 com I2C
  lcd.begin(16,2);          
}

void loop(){ 
  readPushButton(); // verifica se o pushbutton foi acionado
  sowDisplay();     // exibe a distância no display lcd
  delay(150); // delay de 150us
}

void readPushButton() { // verifica se o botão foi acionado - altera para cm, mm ou pol
  if (!digitalRead(pinBut)) { // verifica se o botão foi acionado = LOW
    state >= 3 ? state = 0 : state = state;  
    state++;  
    while (!digitalRead(pinBut)) {} // aguarda soltar o botão
    lcd.clear();
    delay(150); // delay para reduzir o efeito bouncing
  }
}

void sowDisplay() { // exibe resultados no display LCD
  // faz leitura de acordo com o valor da variável state
  switch (state) {
    case 1:
    result = HCSR04.measureDistanceCm();
    decimal = 0;
    unity = " cm";
    break;
    
    case 2:
    result = HCSR04.measureDistanceMm();
    unity = " mm";
    decimal = 0;
    break;

    case 3:
    result = HCSR04.measureDistanceIn();
    unity = " pol";
    decimal = 1;
    break;
  }
  
  //Exibe no display as distâncias em cm, mm ou em polegadas (inchies)
  lcd.setCursor(0,0);
  lcd.print("      "); 
  lcd.print(result[0],decimal);    //Exibe no display as medidas de distância em cm, mm e polegadas
  lcd.print(unity);
  lcd.print("       ");
  lcd.setCursor(1,1);
  lcd.print("Squids Arduino");
}
