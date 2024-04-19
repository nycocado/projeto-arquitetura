// Defino as portas dos dispositivos ligados ao Arduino
#define LED_VERMELHO 10
#define LED_VERDE 11
#define BOTAO_VERMELHO 9
#define BOTAO_VERDE 8

String charsIn = ""; // Esse é o caractere lido pelo Arduino
char printout[20]; // Ainda não entendi para que serve
 
void setup() {
  // Faço o setup das portas usadas
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BOTAO_VERMELHO, INPUT);
  pinMode(BOTAO_VERDE, INPUT); 

  // Inicia a porta serial em 9600
  Serial.begin(9600);
  
  //Mostra o comando de conexão estabelecida
  delay(250);
  Serial.println("Conexão Estabelecida");
}
 
void loop()
{
  while (Serial.available()) // Faz a leitura dos comandos dados ao PC
  {
    char charRead = Serial.read();
    charsIn.concat(charRead);
  }

  // Abaixo são os comandos de interação do Arduino (Veja o README para entender os comandos do Arduino)

  if(digitalRead(BOTAO_VERDE)) // Se o botão do Led Verde for pressionado
  {
    digitalWrite(LED_VERDE, HIGH); // Ele ativa o led verde
    Serial.println("Identidade Válida0"); // Ele envia a mensagem de confirmação junto do ID para salvar na tabela de eventos
    delay(1100); // Tem um delay de 1,1s
    digitalWrite(LED_VERDE, LOW); // Desliga o led verde
  }

  if(digitalRead(BOTAO_VERMELHO)) // Se o botão do led vermelho for pressionado
  {
    digitalWrite(LED_VERMELHO, HIGH); // Ele ativa o led vermelho
    Serial.println("Identidade Inválida1"); // Ele envia a mensagem de confirmação junto do ID para salvar na tabela de eventos
    delay(1100); // Tem um delay de 1,1s
    digitalWrite(LED_VERMELHO, LOW); // Desliga o led vermelho
  }

  // Abaixo são os comandos controlados pelo PC (Veja o README para entender os comandos do PC)

  if(charsIn != "") // Caso tenha alguma entrada de dados vinda do PC
  {
    if (charsIn == "i") // Caso essa entrada seja o caractere i 
    {
      digitalWrite(LED_VERMELHO, HIGH); // Ele vai acender o led vermelho
      Serial.println("LED VERMELHO ON"); // E vai mandar a mensagem de confirmação
    }
    
    if (charsIn == "o") // Caso essa entrada seja o caractere i 
    {
      digitalWrite(LED_VERMELHO, LOW); // Ele vai apagar o led vermelho
      Serial.println("LED VERMELHO OFF"); // E vai mandar a mensagem de confirmação
    }
    
    if (charsIn == "k") // Caso essa entrada seja o caractere k
    {
      digitalWrite(LED_VERDE, HIGH); // Ele vai acender o led verde
      Serial.println("LED VERDE ON"); // E vai mandar a mensagem de confirmação
    }
    
    if (charsIn == "l") // Caso essa entrada seja o caractere l
    {
      digitalWrite(LED_VERDE, LOW); // Ele vai apagar o led verde
      Serial.println("LED VERDE OFF"); // E vai mandar a mensagem de confirmação
    }
    
    if (charsIn == "z") // Caso essa entrada seja o caractere z
    {
      Serial.println("2"); // Ele vai mandar o ID 2 para o computador, onde será executada a lista de eventos
    }

    charsIn.toCharArray(printout, 21); // Ainda não sei para que isso serve
    charsIn = ""; // Define o caractere como nulo
  }
}
