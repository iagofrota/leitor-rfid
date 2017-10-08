#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

#define LED_VERMELHO 2
#define LED_VERDE 3

MFRC522 mfrc522(SS_PIN, RST_PIN);

char st[20];
String uid;

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  iniciarPortas();

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

  uid = lerUID();
  imprimirUID(uid);
  //  Serial.println();
  //  Serial.println("UID lido: ");
  //  Serial.println(uid);

  if (isCartaoBranco(uid)) //UID 1 - Meu Cartão
  {
    liberarAcesso();
    Serial.println("Olá cidadão, pode entrar!");
    Serial.println();
    delay(3000);
    return;
  }

  if (isTagAzul(uid)) //UID 2 - Minha tag
  {
    negarAcesso();
    Serial.println("Você foi bloqueado da minha lista. Vá embora!");
    Serial.println();
    delay(1000);
    return;
  }

  return;
}

// Inicia as portas
void iniciarPortas()
{
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
}

// Imprimi o UID lido
void imprimirUID(String uid)
{
  Serial.print("UID da tag :");
  Serial.println();
  Serial.print("Mensagem : ");
  uid.toUpperCase();
}

// Retorna o que o UID que foi lido
String lerUID()
{
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  return conteudo;
}

// Libera o acesso
void liberarAcesso()
{
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  delay(1000);
  digitalWrite(LED_VERDE, HIGH);
}

// Nega o acesso
void negarAcesso()
{
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);
  delay(1000);
  digitalWrite(LED_VERMELHO, HIGH);
}

// Verifica se é o cartão branco
bool isCartaoBranco(String uid)
{
  String cartaoBranco = "c2 66 86 ab";
  
  if (cartaoBranco == uid.substring(1))
  {
     return true;
  }
  return false;
}

// verifica se é a tag azul
bool isTagAzul(String uid)
{
  String tagAzul = "95 b3 16 ab";
 
  if (tagAzul == uid.substring(1))
  {
    return true;
  }
  return false;
}
