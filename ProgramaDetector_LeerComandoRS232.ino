void RS232_Communication() {
  if ((MD == 20) && (Beam_OnFlag)) {
  if (CheckCommunication_Flag) {
      RS232cmd("MV");
      ControlValues(RS232read());
    }
    else {
      RS232_CheckCommunication();
    }
  }
}

///Function to send String from RS232
void RS232cmd (String command232)
{
  reply232 = "";
  RS232.listen(); //sets listen of Softwareserial on RS232 port.
  RS232.println(command232);
  delay (delaycom);

  if (debug) {
    Serial.print(F("cmd232 : "));
    Serial.println(command232);
  }
}

///Function to read RS232 response
String RS232read() {
  String cadena = "";
  while (RS232.available()) {
    cadena.concat(char(RS232.read()));
  }
  if (debug) {
    Serial.print(F("RS232 read: "));
    Serial.println(cadena);
  }
  return (cadena[0]!='M'?"E":cadena.substring(3));
}

void RS232_CheckCommunication() {
  int i;
  String response;
  for (i = 0; i <= 2; i++) {
    RS232cmd("PTW");
    response = RS232read();
    if (response != "") {
      CheckCommunication_Flag = true;
    }
  }
  if (!CheckCommunication_Flag) {
    Serial.println(F("Connection Failed..."));
    Beam_OnFlag = false;
  } else {
    RS232cmd("STA");
  }
}
