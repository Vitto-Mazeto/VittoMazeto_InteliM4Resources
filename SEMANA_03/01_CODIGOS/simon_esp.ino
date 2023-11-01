const uint8_t BOTOES[] = {16, 17, 18, 19};
const uint8_t LEDS[] = {27, 26, 25, 33};
const uint8_t BOTAO_RESET = 32;
const uint8_t BUZZER_PIN = 22;
#define TAMANHO_JOGO 4

uint8_t sequenciaJogo[TAMANHO_JOGO] = {0};
uint8_t indexJogo = 0;

void setup()
{
  Serial.begin(9600);
  initializePins();
  randomSeed(analogRead(4));
}

void initializePins()
{
  for (byte i = 0; i < 4; i++)
  {
    pinMode(LEDS[i], OUTPUT);
    pinMode(BOTOES[i], INPUT_PULLUP);
  }
  pinMode(BOTAO_RESET, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void lightLED(byte ledIndex)
{
  digitalWrite(LEDS[ledIndex], HIGH);
  delay(300);
  digitalWrite(LEDS[ledIndex], LOW);
}

void iniciarSequencia()
{
  for (int i = 0; i < indexJogo; i++)
  {
    byte ledAtual = sequenciaJogo[i];
    lightLED(ledAtual);
    tone(BUZZER_PIN, 500);
    delay(50);
    noTone(BUZZER_PIN);
  }
}

byte apertarBotao()
{
  while (true)
  {
    if (digitalRead(BOTAO_RESET) == LOW)
    {
      resetarJogo();
      return 0;
    }
    for (byte i = 0; i < 4; i++)
    {
      if (digitalRead(BOTOES[i]) == LOW)
      {
        return i;
      }
    }
    delay(1);
  }
}

void resetarJogo()
{
  lightLED(1);
  tone(BUZZER_PIN, 500);
  delay(80);
  tone(BUZZER_PIN, 250);
  lightLED(1);
  delay(80);
  tone(BUZZER_PIN, 300);
  lightLED(1);
  delay(80);
  tone(BUZZER_PIN, 750);
  lightLED(1);
  Serial.print("Perdeu");
  delay(80);
  indexJogo = 0;
  delay(200);
}

bool checarSequencia()
{
  for (int i = 0; i < indexJogo; i++)
  {
    byte expectedButton = sequenciaJogo[i];
    byte actualButton = apertarBotao();
    lightLED(actualButton);
    if (expectedButton != actualButton)
    {
      return false;
    }
  }
  return true;
}

void loop()
{
  if (indexJogo == TAMANHO_JOGO)
  {
    while (digitalRead(BOTAO_RESET) != LOW)
    {
      lightLED(2);
      delay(80);
    }
    resetarJogo();
    return;
  }

  sequenciaJogo[indexJogo] = random(0, 4);
  indexJogo++;
  Serial.print("Game index: ");
  Serial.println(indexJogo);

  iniciarSequencia();

  if (!checarSequencia())
  {
    resetarJogo();
  }

  delay(300);

  if (indexJogo > 0)
  {
    delay(300);
  }
}
