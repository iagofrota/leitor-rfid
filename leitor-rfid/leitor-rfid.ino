#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

#define LED_VERMELHO 2
#define LED_VERDE 3

MFRC522 mfrc522(SS_PIN, RST_PIN);

char st[20];

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  Serial.println("Aproxime o cartão");
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "C2 66 86 AB") //UID 1 - Meu Cartão
  {
    digitalWrite(LED_VERMELHO, HIGH);
    Serial.println("Olá cidadão, pode entrar!");
    Serial.println();
    digitalWrite(LED_VERDE, LOW);
    delay(3000);
    return;
  }

  if (conteudo.substring(1) == "95 B3 16 AB") //UID 2 - Minha tag
  {
    digitalWrite(LED_VERDE, HIGH);
    Serial.println("Você foi bloqueado da minha lista. Vá embora!");
    Serial.println();
    digitalWrite(LED_VERMELHO, LOW);
    delay(3000);
    return;
  }
}
