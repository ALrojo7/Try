void NewSessionCreation() {
  int val;
  EEPROM.get(direcEEPROM, val);
  if (NewSessionNeeded) {
    SC = val + 1;
    if (SC >= 4294967296) {  //4 byte
      EEPROM.put(direcEEPROM, 0);
    }
    else EEPROM.put(direcEEPROM, SC);
  }
  else {
    SC = val;
  }
}
void EndSessionSafe() {
  EEPROM.put(4, IM);
  EEPROM.put(8, DT);
  EEPROM.put(12, MD);
  EEPROM.put(16, F);
  EEPROM.put(20, AC);
  EEPROM.put(24, MAT);
  EEPROM.put(28, E);
  EEPROM.put(32, Frep);
  EEPROM.put(36, ATime);
  SC = 0; IM = 0; DT = 0; MD = 0; F = 0; AC = 0; MAT = 0; E = 0; Frep = 0; ATime = 0;
  strcpy(DTString, "0");
  strcpy(FString, "0");
  strcpy(FrepString, "0");
}
float DoseCalculator(int IM, float DT, int MD, unsigned int E, float Frep, float F) {
  if ((IM == 2) && (MD == 11)) {
    float k1 = E;
    float k2 = 0;
    if (AC == 0) {
      k2 = 1;
    }
    else {
      k2 = AC;
    }
    float k3 = Frep;

    K = (k1 * k2 * k3);

    return DT / K;
  }
  else if ((IM == 2) && (MD == 12)) {
    return DT / F;
  }
  else if ((IM == 1) && (MD == 11) ) {
    float k1 = E;
    float k2 = 0;
    if (AC == 0) {
      k2 = 1;
    }
    else {
      k2 = AC;
    }
    float k3 = Frep;

    K = (k1 * k2 * k3);

    return DT * K;
  }
  else if ((IM == 1) && (MD == 12)) {
    return DT * F;
  }

}
void DoseRevelator() {
  if (IM == 1) {
    Serial.print(F("Dose to emit..."));
    Dtoget = DoseCalculator(IM, DT, MD, E, Frep, F);
    Serial.println(Dtoget);
  }
  else if (IM == 2) {
    Serial.print(F("Time to emit..."));
    Ttoget = DoseCalculator(IM, DT, MD, E, Frep, F);
    Serial.println(DoseCalculator(IM, DT, MD, E, Frep, F));
  }
}
void RestoreDatabyDefault() {
  NewSessionMakerValidator = false;
  ModeOperationSelectorValidator = false;
  DeliveryRequestValidator =  false;
  MeasureOperationSelectorValidator = false;
  ConversionMethodSelectorValidator = false;
  IsCollimator = false;
  SampleMaterialValidator = false;
  EnergyValidator = false;
  FrecuencyValidator = false;
  NewSessionNeeded = true;
  TotalTime = 0;
  MomentumTimer = 0;
  Beam_OnFlag = false;
  Beam_OffFlag = false;
  Beam_StpFlag = false;
  EndSessionSafe();
}
void SftEvent() {
  CT = micros() - cmdT;
  if (CT >= CTmax) {
    Serial.println(F("System Pause!"));
    digitalWrite(Led1, LOW);
  }
}
byte BeamShouldStart() {
  byte beamok = 0;
  if (NewSessionMakerValidator && ModeOperationSelectorValidator && DeliveryRequestValidator && MeasureOperationSelectorValidator && ConversionMethodSelectorValidator && IsCollimator && SampleMaterialValidator && EnergyValidator && FrecuencyValidator && !NewSessionNeeded ) {
    beamok = 1;
    return beamok;
  }
  else {
    return beamok;
  }
}
void BeamControlExecuter() {
  if (Beam_OnFlag) {
    if (IM == 1) {
      if (ATimeEmitted > 0) {
        ATime = ATimeEmitted;
        ATimeEmitted = 0.00;
      }
      Dmoment = DoseCalculator(IM, ATime, MD, E, Frep, F);
      TIter = micros() - TotalTime;
      ATime += TIter;
      strcpy(EState, "Beam Is Emitting");
      digitalWrite(Led1, HIGH);
      if (Dmoment >= Dtoget) {
        digitalWrite(Led1, LOW);
        Serial.print(ATime);
        Serial.println(" finished");
        SessionEnd();
      }
    }
    else if (IM == 2) {
      TIter = micros() - TotalTime;
      if (ATimeEmitted > 0) {
        ATime = ATimeEmitted;
        ATimeEmitted = 0.00;
      }
      ATime += TIter;
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
