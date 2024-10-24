#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

unsigned long id;
unsigned long data;

SPIClass vspi = SPIClass(VSPI);
MCP_CAN CAN0(&vspi, 5);

void setup()
{
  Serial.begin(115200);
  vspi.begin(18, 19, 23, 5); // Pins for VSPI
  //  Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  CAN0.setMode(MCP_NORMAL); // Change to normal mode to allow messages to be transmitted
}

void loop()
{
  if (Serial.available() >= 14)
  { // Ожидаем 14 байт
    unsigned long id = 0;
    byte data[8];
    unsigned int delayMs = 0;

    // Читаем unsigned long (4 байта)
    for (int i = 0; i < 4; i++)
    {
      id |= (unsigned long)Serial.read() << (i * 8);
    }

    // Читаем byte[8] (8 байт)
    for (int i = 0; i < 8; i++)
    {
      data[i] = Serial.read();
    }

    // Читаем unsigned int (2 байта)
    for (int i = 0; i < 2; i++)
    {
      delayMs |= (unsigned int)Serial.read() << (i * 8);
    }

    byte sndStat = CAN0.sendMsgBuf(id, 1, 8, data); // Задние противотуманные фонари
    if (sndStat == CAN_OK)
    {
      // При корректной отправке пакета, выводим данные в Serial Monitor
      Serial.print(id);
      Serial.print(" ");
      for (int i = 0; i < 8; i++)
      {
        Serial.print(data[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
    else
    {
      Serial.println("Error Sending Message...");
    }
    delay(delayMs); // send data per 100ms
  }
}
