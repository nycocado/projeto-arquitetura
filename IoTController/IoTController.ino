#define LED_VERMELHO 10
#define LED_VERDE 11
#define BOTAO_VERMELHO 9
#define BOTAO_VERDE 8
#define BUZZER A0

char *texto[] = {"Identidade Válida0", "Identidade Inválida1", "LED VERMELHO ON", "LED VERMELHO OFF", "LED VERDE ON", "LED VERDE OFF", "ALARME ON", "ALARME OFF"}; // Textos exibidos no terminal do PC
String charsIn = ""; // Esse é o caractere lido pelo Arduino
char printout[20]; // Ainda não entendi para que serve

const int buzzerFrequency = 1000; // Frequência de 1000 Hz

void setup()
{
  // Faço o setup das portas usadas
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BOTAO_VERMELHO, INPUT);
  pinMode(BOTAO_VERDE, INPUT);
  pinMode(BUZZER, OUTPUT); // Configura a porta do buzzer como saída

  // Inicia a porta serial em 9600
  Serial.begin(9600);

  // Mostra o comando de conexão estabelecida
  delay(250);
  Serial.println("Conexão Estabelecida");
}

void acender_botao(int conector, char *texto, bool ativar_buzzer)
{
  digitalWrite(conector, HIGH); // Ele ativa o led
  if (ativar_buzzer)
  {
    tone(BUZZER, buzzerFrequency); // Ativa o buzzer com a frequência especificada
  }
  Serial.println(texto);        // Ele envia a mensagem de confirmação junto do ID para salvar na tabela de eventos
  delay(2000);                  // Tem um delay de 2s
  digitalWrite(conector, LOW);  // Desliga o led
  if (ativar_buzzer)
  {
    noTone(BUZZER); // Desativa o buzzer
  }
}

void interagir_led(int conector, char *texto, int estado)
{
  digitalWrite(conector, estado); // Ele vai acender o led
  Serial.println(texto);          // E vai mandar a mensagem de confirmação
}

void interagir_buzzer(char *texto, int estado)
{
  if (estado == HIGH)
  {
    tone(BUZZER, buzzerFrequency); // Ativa o buzzer com a frequência especificada
    delay(2000);                   // Mantém o buzzer ativado por 2s
    noTone(BUZZER);                // Desativa o buzzer após o delay
  }
  else
  {
    noTone(BUZZER); // Desativa o buzzer
  }
  Serial.println(texto); // E vai mandar a mensagem de confirmação
}

void interagir_buzzer_comando(char *texto, int estado)
{
  if (estado == HIGH)
  {
    tone(BUZZER, buzzerFrequency); // Ativa o buzzer com a frequência especificada
  }
  else
  {
    noTone(BUZZER); // Desativa o buzzer
  }
  Serial.println(texto); // E vai mandar a mensagem de confirmação
}


void loop()
{
  while (Serial.available()) // Faz a leitura dos comandos dados ao PC
  {
    char charRead = Serial.read();
    charsIn.concat(charRead);
  }

  // Abaixo são os comandos de interação do Arduino (Veja o README para entender os comandos do Arduino)

  if (digitalRead(BOTAO_VERDE)) // Se o botão do led verde for pressionado
  {
    acender_botao(LED_VERDE, texto[0], false); // Vai acender o led verde por 2s e enviar a mensagem de confirmação junto com o ID do evento
  }

  if (digitalRead(BOTAO_VERMELHO)) // Se o botão do led vermelho for pressionado
  {
    acender_botao(LED_VERMELHO, texto[1], true); // Vai acender o led vermelho por 2s, ativar o buzzer e enviar a mensagem de confirmação junto com o ID do evento
  }

  // Abaixo são os comandos controlados pelo PC (Veja o README para entender os comandos do PC)

  if (charsIn != "") // Caso tenha alguma entrada de dados vinda do PC
  {
    if (charsIn == "i") // Caso essa entrada seja o caractere i
    {
      interagir_led(LED_VERMELHO, texto[2], HIGH); // Vai acender o led vermelho e enviar a mensagem de confirmação
    }

    if (charsIn == "o") // Caso essa entrada seja o caractere o
    {
      interagir_led(LED_VERMELHO, texto[3], LOW); // Vai apagar o led vermelho e enviar a mensagem de confirmação
    }

    if (charsIn == "k") // Caso essa entrada seja o caractere k
    {
      interagir_led(LED_VERDE, texto[4], HIGH); // Vai acender o led verde e enviar a mensagem de confirmação
    }

    if (charsIn == "l") // Caso essa entrada seja o caractere l
    {
      interagir_led(LED_VERDE, texto[5], LOW); // Vai apagar o led verde e enviar a mensagem de confirmação
    }

    if (charsIn == "n") // Caso essa entrada seja o caractere b
    {
      interagir_buzzer_comando(texto[6], HIGH); // Vai ativar o buzzer por 2s e enviar a mensagem de confirmação
    }

    if (charsIn == "m") // Caso essa entrada seja o caractere n
    {
      interagir_buzzer_comando(texto[7], LOW); // Vai desativar o buzzer e enviar a mensagem de confirmação
    }

    if (charsIn == "z") // Caso essa entrada seja o caractere z
    {
      Serial.println("2"); // Ele vai mandar o ID 2 para o computador, onde será executada a lista de eventos
    }

    if (charsIn == "c") // Caso essa entrada seja o caractere c
    {
      Serial.println("3"); // Ele vai mandar o ID 3 para o computador, onde será exibido os controles de comando
    }

    charsIn.toCharArray(printout, 21); // Ainda não sei para que isso serve
    charsIn = "";                      // Define o caractere como nulo
  }
}
