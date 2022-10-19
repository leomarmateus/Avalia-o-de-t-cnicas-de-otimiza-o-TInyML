/*******************************************************************************
* Serial Library
*******************************************************************************/

// Functions

/*******************************************************************************
Serial read bytes
Inputs:
- Data array
Return:
- Count
*******************************************************************************/
int serialRead(byte *data_read) {
  int count = 0;
  int incomingByte;

  // Aguardar resposta!!!
  // TODO: Adicionar timeout e remover delay
  delay(1000);

  while (Serial.available() > 0) {
    incomingByte = Serial.read();
    data_read[count] = incomingByte;
    count++;
  }
  return count;
}