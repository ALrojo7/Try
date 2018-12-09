void ControlValues(String response232) {
  char checkpoint = '.'; //Point to check were the decimal time start
  char separator = ';'; //response searator by default
  String analyzer = ""; //String to analyze dose and time
  String receivertimeString = "";
  String receiverdoseString = "";
  analyzer = response232.indexOf(checkpoint);
  
  receivertimeString = response232.substring(analyzer.length() + 2, response232.lastIndexOf(separator));
  receiverTime = st2fl(receivertimeString);

  receiverdoseString = response232.substring(analyzer.length() + 3, response232.indexOf(separator));
  receiverDose = st2fl(receiverdoseString);
}

float st2fl(String st)
{
  float fl = 0.0;
  byte e;
  //convert reply string to numeric value
  for (byte i = 0; i < st.indexOf('E'); i++) { //to parse the value without taking into account the '.'
    if ( (st[i] > 47) && (st[i] < 58) ) {
      fl *= 10; fl += (st[i] - 48);
    }
  }
  fl = fl / pow(10, (st.indexOf('E') - st.indexOf('.') - 1));
  //to parse the multiplier
  e = st[st.indexOf('E') + 2] - 48;
  if ( st[st.indexOf('E') + 1] == '-' ) {
    fl = fl / pow(10, e);
  }
  else if ( st[st.indexOf('E') + 1] == '+' ) {
    fl = fl * pow(10, e);
  }
  return fl;
}
void RS232Control() {
  if (Beam_OnFlag) {
    if (IM == 1) {
      if (ATimeEmitted > 0) {
        ATime = ATimeEmitted;
        ATimeEmitted = 0.00;
      }
      ATime = receiverTime;
      strcpy(EState, "Beam Is Emitting");
      digitalWrite(Led1, HIGH);
      if (receiverDose >= Dtoget) {
        digitalWrite(Led1, LOW);
        Serial.print(ATime);
        Serial.println(" finished");
        SessionEnd();
      }
    }
    else if (IM == 2) {
      if (ATimeEmitted > 0) {
        ATime = ATimeEmitted;
        ATimeEmitted = 0.00;
      }
      ATime = receiverTime;
      strcpy(EState, "Beam Is Emitting");
      digitalWrite(Led1, HIGH);
      if (ATime >= Ttoget) {
        digitalWrite(Led1, LOW);
        Serial.print(ATime);
        Serial.println(" finished");
        SessionEnd();
      }
    }
  }
  else if (Beam_StpFlag) {
    ATimeEmitted = ATime;
    strcpy(EState, "Emission Stopped");
    digitalWrite(Led1, LOW);

  }
  else if (Beam_OffFlag) {
    strcpy(EState, "Emission Suspended");
    SessionEnd();
  }
}
