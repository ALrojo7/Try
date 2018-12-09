void NewSessionMaker() {
  Serial.print(F("New Session Is Creating... Number: "));
  NewSessionCreation();
  NewSessionNeeded = false;
  Serial.println(SC);
  NewSessionMakerValidator = true;
}
void ModeOperationSelector() {
  if (inParam == 2) {
    IM = 2;
    ModeOperationSelectorValidator = true;
  }
  if (inParam == 1) {
    IM = 1;
    ModeOperationSelectorValidator = true;
  }
}
void DeliveryCatcher() {
  DT = flParam;
  strcpy(DTString,stParam);
  Serial.print(F("Delivery is register like..."));
  Serial.print(DTString);
  DeliveryRequestValidator = true;
}
void MeasureOperationSelector() {
  if (inParam == 11) {
    MD = 11;
    Serial.println(F("Default Analytic Method Selected..."));
    MeasureOperationSelectorValidator = true;
    ConversionMethodSelectorValidator = true;
  }
  else if (inParam == 12) {
    MD = 12;
    Serial.println(F("Conversion Analytic Method Selected..."));
    MeasureOperationSelectorValidator = true;
    IsCollimator = true;
    SampleMaterialValidator = true;
    EnergyValidator = true;
    FrecuencyValidator = true;
    ConversionMethodSelectorValidator = false;
  }
  else if (inParam == 20) {
    Serial.println(F("Not implemented yet"));
  }
}
void ConversionMethodCatcher() {
  F = flParam;
  strcpy(FString,stParam);
  Serial.print(F("The Conversion factor is register like..."));
  Serial.println(F);
  ConversionMethodSelectorValidator = true;
}
void CollimatorCatcher() {
  if (inParam > 0) {
    strcpy(AC,stParam);
    IsCollimator = true;
    Serial.println(F("Collimator Suposed!"));
  }
  else if (inParam = 0) {
    AC = '0';
    IsCollimator = true;
    Serial.println(F("Direct Emission!"));
  }
}
void SampleMaterialCatcher() {
  MAT = inParam;
  Serial.print("Sample Material is register like...");
  Serial.println(MAT);
  SampleMaterialValidator = true;
}
void EnergyCatcher() {
  E = inParam;
  Serial.print(F("The Energy of the Beam is register like..."));
  Serial.println(E);
  EnergyValidator = true;
}
void FrequencyCatcher() {
  Frep = flParam;
  strcpy(FrepString,stParam);
  Serial.print(F("The Frecuency of pulse repetition is register like..."));
  Serial.println(Frep);
  FrecuencyValidator = true;
}
void SessionEnd() {
  digitalWrite(Led1, LOW);
  Serial.println(resp);
  RestoreDatabyDefault();
}
