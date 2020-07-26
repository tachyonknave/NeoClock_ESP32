/**
*/
class CommandsBuffer {

  public:
    static const uint8_t NumberOfCommands = 8;
    static const uint8_t CommandSize = 7; 

    CommandsBuffer();

    CommandsBuffer(CommandsBuffer* oldCB);

    bool areThereMoreCommands();

    uint8_t* getNextCommand();

    void reset();

    uint8_t* read(uint8_t offset, uint8_t length);

    void setNumBytes(uint8_t NumBytes);
    uint8_t getNumBytes();

    uint8_t getNumberOfMessages();

    
    //TODO
    uint8_t byteBuffer[NumberOfCommands * CommandSize];

  private:

    uint8_t numBytes;
    uint8_t byteIndex = 0;
};
/**

*/
CommandsBuffer::CommandsBuffer() {
  numBytes = 0;
  byteBuffer[0] = '\0';
}

/**

*/
CommandsBuffer::CommandsBuffer(CommandsBuffer* oldCB) {
  char message[4] = { '\0', '\0', '\0', '\0'};

  reset();

  numBytes = oldCB->numBytes;

  for (int i = 0; i < (NumberOfCommands * CommandSize); i++) {
    byteBuffer[i] = '\0';
  }

  for (int i = 0; i < numBytes; i++) {
    byteBuffer[i] = oldCB->byteBuffer[i];
  }

  Serial.println();
  Serial.print("\t");
  for (int i = 0; i < (NumberOfCommands * CommandSize); i++) {
    sprintf(message, "%.2X ", byteBuffer[i]);
    Serial.print(message);
    if ((i % CommandSize) == 6) {
      Serial.println();
      Serial.print("\t");
    }
  }

  Serial.println();
  Serial.print("CB - Number of Bytes: "); Serial.println(numBytes);
}

/**

*/
void CommandsBuffer::reset() {
  byteIndex = 0;
}

/**
*/
void CommandsBuffer::setNumBytes(uint8_t NumBytes) {
  numBytes = NumBytes;
}
/**

*/
uint8_t CommandsBuffer::getNumBytes() {
  return numBytes;
}


uint8_t CommandsBuffer::getNumberOfMessages(){
  return numBytes / CommandSize;
}

/**
*/
bool CommandsBuffer::areThereMoreCommands() {
  return byteIndex < numBytes;
}

/**

*/
uint8_t* CommandsBuffer::getNextCommand() {
  return read(byteIndex, CommandSize);
}

uint8_t* CommandsBuffer::read(uint8_t offset, uint8_t length) {

  uint8_t* returnBuffer = NULL;

  if ((offset + length) <= numBytes) {

    returnBuffer = new uint8_t[length];

    for (int i = 0; i < length; i++) {
      returnBuffer[i] = byteBuffer[i + offset];
    }

    byteIndex = offset + length;

  } else {
    Serial.println("CB - read : invalid parameters");
  }

  return returnBuffer;
}
