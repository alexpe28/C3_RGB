#include <Arduino.h>
#include <config.h>
#include <WiFi.h>
#include <FastBot.h>

FastBot bot(BOT_TOKEN);

void connectWiFi()
{
  delay(2000);
  // Serial.begin(115200);
  Serial.println("start");
  // WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (millis() > 15000)
      ESP.restart();
  }
  Serial.println("Connected");
}

void setColor(int TrueColor)
{
  int R = (TrueColor >> 16) & 0xFF; // Красный
  int G = (TrueColor >> 8) & 0xFF;  // Зеленый
  int B = TrueColor & 0xFF;         // Синий
  analogWrite(PIN_RED, R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE, B);
}

void testColors()
{
  setColor(0x00C9CC);
  delay(1000);
  setColor(0xF7788A);
  delay(1000);
  setColor(0x34A853);
  delay(1000);
  setColor(0x000000); // off
}

void whiteDuty(int D)
{
  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE, 0);
  analogWrite(PIN_WHITE, D);
}

int mesToInt(char symbol)
{
  if ('0' <= symbol && symbol <= '9')
  {
    return symbol - '0';
  }
  else if ('A' <= symbol && symbol <= 'F')
  {
    return symbol - 'A' + 10;
  }
  else if ('a' <= symbol && symbol <= 'f')
  {
    return symbol - 'a' + 10;
  }
  return -1;
}

int msgToColor(const char *mes)
{
  int r = (mesToInt(mes[1]) << 4) + mesToInt(mes[2]);
  int g = (mesToInt(mes[3]) << 4) + mesToInt(mes[4]);
  int b = (mesToInt(mes[5]) << 4) + mesToInt(mes[6]);
  return (r << 16) + (g << 8) + b; // возвращаем в виде числа
}

//========== обработка сообщений ===========

// обработчик сообщений
void newMsg(FB_msg &msg)
{
  // выводим всю информацию о сообщении
  // Serial.println(msg.toString());

  if (msg.text.endsWith("Ping"))
  {
    bot.sendMessage("I'm online!", msg.chatID);
  }

  // if (msg.text.endsWith("restart"))
  // {
  //   bot.sendMessage("OK. restart", msg.chatID);
  //   ESP.restart();
  // }

  else if (msg.text.startsWith("#"))
  {
    bot.sendMessage("OK. Let's change the color", msg.chatID);

    int tempo = msgToColor(msg.text); // сделать приведение типов к const char*
    // setColor(*msg);
  }

  // else if (msg.text.endsWith("led12")) {
  //   bot.sendMessage("Led12 PWM go", msg.chatID);
  //   fadeLed(LED12);
  // }

  // else if (msg.text.endsWith("sleep")) /* для отладки */
  // {
  //   bot.sendMessage("Ok. Going to sleep...", msg.chatID);
  //   Serial.println("Ok. Going to sleep...");
  // }

  else if (msg.text.endsWith("Time"))
  {
    FB_Time t = bot.getTime(3);

    Serial.print(t.timeString());
    Serial.print(' ');
    Serial.println(t.dateString());

    bot.sendMessage(t.timeString(), msg.chatID);
    bot.sendMessage(t.dateString(), msg.chatID);
  }

  // else if (msg.text.startsWith("Alarm"))
  // {
  //   al_hour = msg.text.substring(6, 8).toInt();
  //   delay(200);
  //   Serial.println("al_hour: " + String(al_hour));
  //   bot.sendMessage("al_hour: " + String(al_hour), msg.chatID);
  //   delay(200);
  //   al_minute = msg.text.substring(9, 11).toInt(); /* exe: alarm 09:45 */
  //   Serial.println("al_minute: " + String(al_minute));
  //   bot.sendMessage("al_minute: " + String(al_minute), msg.chatID);

  //   EEPROM.write(0, al_hour);
  //   EEPROM.write(1, al_minute);
  //   EEPROM.commit();
  //   //  ===================== ДЛЯ ОТЛАДКИ =========================
  //   // delay(1000);
  //   // Serial.println("EEPROM.read(0) = " + String(EEPROM.read(0)));
  //   // Serial.println("EEPROM.read(1) = " + String(EEPROM.read(1)));
  //   //  ===========================================================
  // }

  // else if (msg.text.startsWith("Duration"))
  // {
  //   al_duration = msg.text.substring(9, 11).toInt(); /* exe: duration 15 */
  //   delay(200);
  //   Serial.println("al_duration: " + String(al_duration));
  //   bot.sendMessage("al_duration: " + String(al_duration), msg.chatID);

  //   EEPROM.write(2, al_duration);
  //   EEPROM.commit();
  //   //  ===================== ДЛЯ ОТЛАДКИ =========================
  //   delay(1000);
  //   Serial.println("EEPROM.read(2) = " + String(EEPROM.read(2)));
  //   //  ===========================================================
  // }

  // else if (msg.text.startsWith("Duty"))
  // {
  //   al_duty = msg.text.substring(6, 8).toInt();

  //   // delay(200);
  //   // Serial.println("al_hour: " + String(al_hour));
  //   // bot.sendMessage("al_hour: " + String(al_hour), msg.chatID);
  //   // delay(200);
  //   // al_minute = msg.text.substring(9, 11).toInt(); /* exe: Duty 153 */
  //   // Serial.println("al_minute: " + String(al_minute));
  //   // bot.sendMessage("al_minute: " + String(al_minute), msg.chatID);

  //   // EEPROM.write(0, al_hour);
  //   // EEPROM.write(1, al_minute);
  //   // EEPROM.commit();
  //   //  ===================== ДЛЯ ОТЛАДКИ =========================
  //   // delay(1000);
  //   // Serial.println("EEPROM.read(0) = " + String(EEPROM.read(0)));
  //   // Serial.println("EEPROM.read(1) = " + String(EEPROM.read(1)));
  //   //  ===========================================================
  // }

  else if (msg.text.startsWith("Seton"))
  {
    setMode = true;
    delay(200);
    Serial.println("Setting Mode is ON");
    bot.sendMessage("Setting Mode is ON", CHAT_ID);
  }

  else if (msg.text.startsWith("Setoff"))
  {
    setMode = false;
    delay(200);
    Serial.println("Setting Mode is OFF");
    bot.sendMessage("Setting Mode is OFF", CHAT_ID);
  }
}

//==========================================

void setup()
{
  // Serial.begin(115200);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
}

void loop()
{
}
