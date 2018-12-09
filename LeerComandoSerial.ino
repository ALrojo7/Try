void SerialTranslator() {
  String cadena = "";
  if (strncmp(reg, "STMP", 4) == 0) {
    if (act[1] == 'P') {
      if (BeamShouldStart == 0) {
        resp += SC; resp.concat('/');
        resp += IM; resp.concat('/');
        resp += DTString; resp.concat('/');
        resp += MD; resp.concat('/');
        resp += FString; resp.concat('/');
        resp += AC; resp.concat('/');
        resp += MAT; resp.concat('/');
        resp += E; resp.concat('/');
        resp += FrepString;
      }
      else {
        resp += SC; resp.concat('/');
        resp += IM; resp.concat('/');
        resp += DTString; resp.concat('/');
        resp += MD; resp.concat('/');
        resp += FString; resp.concat('/');
        resp += AC; resp.concat('/');
        resp += MAT; resp.concat('/');
        resp += E; resp.concat('/');
        resp += FrepString; resp.concat('/');
        resp += ATime; resp.concat('/');
        resp += EState;

      }
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "SC", 2) == 0) {
    if (act[1] == 'R') {
      resp += SC;
    }
    else if (act[1] == 'W') {
      if (!NewSessionNeeded) {
        Serial.println(F("New Session Is Just Created..."));
      }
      else {
        NewSessionMaker();
      }
      resp += SC;
    }
    else {
      badcmd;
    }
    goto endsub;
  }

  if (strncmp(reg, "IM", 2) == 0) {
    if (act[1] == 'R') {
      resp += IM;
    }
    else if ((act[1] == 'W') && (nParam != 0) ) {
      ModeOperationSelector();
      resp += IM;
    }
    else {
      badcmd;
    }
    goto endsub;
  }

  if (strncmp(reg, "DT", 2) == 0) {
    if (act[1] == 'R') {
      resp += DTString;
    }
    else if ((act[1] == 'W') && (nParam != 0)) {
      DeliveryCatcher();
      resp += DTString;
    }
    else {
      badcmd;
    }
    goto endsub;
  }

  if (strncmp(reg, "MD", 2) == 0) {
    if (act[1] == 'R') {
      resp += MD;
    }
    else if ((act[1] == 'W') && (nParam != 0) ) {
      if ((inParam == 11) || (inParam == 12)) {
        MeasureOperationSelector();
        resp += MD;
      }
      else {
        badsyn();
      }
    }
    else {
      badcmd();
    }
    goto endsub;
  }

  if (strncmp(reg, "F", 1) == 0) {
    if (act[1] == 'R') {
      resp += FString;
    }
    else if ((act[1] == 'W') && (nParam != 0)) {
      ConversionMethodCatcher();
      resp += FString;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "AC", 2) == 0) {
    if (act[1] == 'R') {
      resp += AC;
    }
    else if ((act[1] == 'W') && (nParam != 0)) {
      CollimatorCatcher();
      resp += AC;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "MAT", 3) == 0) {
    if (act[1] == 'R') {
      resp += MAT;
    }
    else if ((act[1] == 'W') && (nParam != 0)) {
      SampleMaterialCatcher();
      resp += MAT;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "E", 1) == 0) {
    if (act[1] == 'R') {
      resp += E;
    }
    else if ((act[1] == 'W') && (nParam != 0)) {
      EnergyCatcher();
      resp += E;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "FR", 2) == 0) {
    if (act[1] == 'R') {
      resp += FrepString;
    }
    else if ((act[1] == 'W') && (nParam != 0)) {
      FrequencyCatcher();
      resp += FrepString;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "BMON", 4) == 0) {
    if (act[1] == 'A') {
      DoseRevelator();
      Beam_StpFlag = false;
      Beam_OnFlag = true;
      MomentumTimer = micros();
      resp += ATime;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "BSTP", 4) == 0) {
    if (act[1] == 'A') {
      Beam_StpFlag = true;
      Beam_OnFlag = false;
      resp += ATime;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  if (strncmp(reg, "BOFF", 4) == 0) {
    if (act[1] == 'A') {
      Beam_OffFlag = true;
      Beam_OnFlag = false;
      Beam_StpFlag = false;
      resp += ATime;
    }
    else {
      badcmd;
    }
    goto endsub;
  }
  //MODE DEBUG
  if ( strncmp(reg, "DBUG", 4) == 0 ) {
    if ( act[1] == 'R' ) {
      resp += debug;

      //server.print("(r/"); server.print(reg); server.print("/");
      //server.print(debug); server.println(")");
    }
    else if ( act[1] == 'W' ) {
      if ( (inParam == 0 || inParam == 1) ) {
        debug = inParam;
        resp += debug;
      }
      else {
        badsyn();
      }
    }
    else {
      badcmd();
    }
    goto endsub;
  }
  //MODE STATUS
  if ( strncmp(reg, "MODE", 4) == 0 ) {
    if ( act[1] == 'R' ) {
      resp += mode;

      //server.print("(r/"); server.print(reg); server.print("/");
      //server.print(mode); server.println(")");
    }
    else if ( act[1] == 'W') {
      if ( (inParam == 0 || inParam == 1) ) {
        mode = inParam;
        resp += mode;

        //server.print("(w/"); server.print(reg); server.print("/");
        //server.print(mode); server.println(")");
      }
      else {
        badsyn();
      }
    }
    else {
      badcmd();
    }
    goto endsub;
  }
endsub:
  {
    // activate to run with MP Controller
    /*
      echo(); // on every cmd request send echo to MPC controller
    */
    cmdT = millis(); //actualize timestamp of last CMD

    if (resp != "") {
      response = (act[1] == 'W' ? "(w/" : (act[1] == 'R' ? "(r/" : (act[1] == 'A' ? "(a/" : "(p/")));
      response.concat(reg); response.concat('/'); response.concat(resp); response.concat(')');
      Serial.println(response);

      //clear string to be sent to the operator
      response = "";
      resp = "";
    }

  }
}

void badcmd() {
  Serial.print("(#/"); Serial.print(reg); Serial.println(")"); //error message
}
void badsyn() {
  Serial.print("(!/"); Serial.print(reg); Serial.println(")"); //error message
}
