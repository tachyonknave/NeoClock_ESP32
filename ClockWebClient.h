
#ifndef __WIFI_H__
#include <WiFi.h>
#define __WIFI_H__
#endif

#include "CommandsBuffer.h"

/**
*/
class ClockWebClient {

  private:
    WiFiClient m_client;

    void sendOK();
    String readLine();
    int readRequestBody(int requestBodySize);

    CommandsBuffer* commandsBuffer;

  public:
    ClockWebClient(WiFiClient client);
    ~ClockWebClient();

    void processRequest();

    void sendResponse();

    CommandsBuffer* getCommandBuffer();
};


/**

*/
ClockWebClient::ClockWebClient(WiFiClient client)
{
  commandsBuffer = NULL;
  m_client = client;
}

/**

*/
ClockWebClient::~ClockWebClient()
{
  if (commandsBuffer != NULL) {
    delete commandsBuffer;
  }
}

/**

*/
CommandsBuffer* ClockWebClient::getCommandBuffer() {

  //Can the copy and the delete be replaced by 'cbCopy = commandsBuffer'?
  CommandsBuffer* cbCopy = new CommandsBuffer(commandsBuffer);

  delete commandsBuffer;
  commandsBuffer = NULL;

  return cbCopy;
}

/**

*/
void ClockWebClient::sendResponse() {

  char messageString[32];
  int numOfMessages = commandsBuffer->getNumberOfMessages();
  int offset = 0;

  //Output Response Header
  m_client.print("HTTP/1.1 202 Accepted\r\n");
  m_client.print("Content-Type: text/html\r\n");
  m_client.print("Connection: close\r\n");
  m_client.print("\r\n");

  //Output Response Body

  m_client.println("<!DOCTYPE html><html>\n<head><title>Command Reciept Confirmation</title></head>\n<body>");

  for (int i = 0; i < numOfMessages; i++) {

    offset = i * 7;

    sprintf(messageString, "%.2X  %.2X  %.2X %.2X   %.2X %.2X %.2X\n\0",
            commandsBuffer->byteBuffer[offset + 0],
            commandsBuffer->byteBuffer[offset + 1],
            commandsBuffer->byteBuffer[offset + 2],
            commandsBuffer->byteBuffer[offset + 3],
            commandsBuffer->byteBuffer[offset + 4],
            commandsBuffer->byteBuffer[offset + 5],
            commandsBuffer->byteBuffer[offset + 6]);

    m_client.print("\t<h1>");
    m_client.print(messageString);
    m_client.println("</h1>");

  }

  m_client.println("</body>\n</html>");
  m_client.print("\r\n");

  m_client.stop();
}

/**

*/
String ClockWebClient::readLine() {

  char c = '\0';
  String line = "";
  int charNum = 0;

  do {
    c = m_client.read();
    line += c;
    charNum = (int) c;
  } while (c != '\n');

  return line;

}
/**

*/
int ClockWebClient::readRequestBody(int requestBodySize) {

  commandsBuffer = new CommandsBuffer();

  uint8_t bytesRead = m_client.read(commandsBuffer->byteBuffer, requestBodySize);
  commandsBuffer->byteBuffer[bytesRead] = '\0';
  commandsBuffer->setNumBytes(bytesRead);

  return bytesRead;
}
/**

*/
void ClockWebClient::processRequest()
{
  Serial.println("Client Connected");

  String currentLine = "";                // make a String to hold incoming data from the client
  String contentLength = "";
  String contentLengthField = "Content-Length:";
  int    contentLengthFieldLength = contentLengthField.length() + 1;
  int    stringIndex = -1;
  int    contentLengthInt = 0;

  while (m_client.available()) {             // if there's bytes to read from the client,

    do {
      currentLine = readLine();

      int stringIndex = currentLine.indexOf(contentLengthField);

      if (stringIndex != -1) {
        String content_length = currentLine.substring(stringIndex + contentLengthFieldLength);
        contentLengthInt = content_length.toInt();
      }

    } while (currentLine != "\r\n");

    if (contentLengthInt > 0)
      readRequestBody(contentLengthInt);

    sendResponse();

  }//End while client.available
}
