// void build() {  // Построитель формы ввода настроек
//   GP.BUILD_BEGIN();
//   GP.THEME(GP_LIGHT);

//   GP.FORM_BEGIN("/login");
//   GP.TITLE(_NUMBER_SEN);
//   GP.TITLE("Версия " _VERSION_SEN);
  
//   //---///
//   GP.BLOCK_BEGIN(GP_THIN, "", "WiFi", GP_BLUE);
//   GP.LABEL("SSID");
//   GP.TEXT("lg", "Login", lp.ssid);
//   GP.BREAK();
//   GP.LABEL("Пароль");
//   GP.PASS("ps", "Password", lp.pass);
//   GP.BREAK();
//   GP.BLOCK_END();
//   //---///
//   GP.BLOCK_BEGIN(GP_THIN, "", "MQTT", GP_BLUE);
//   //GP.TITLE("MQTT");
//   GP.LABEL("Адрес");
//   GP.TEXT("msrv", "Server", lp.mqttserv);
//   GP.BREAK();
//   //
//   GP.HR();
//   //
//   GP.LABEL("Логин");
//   GP.TEXT("muser", "Login", lp.mqttlogin);
//   GP.BREAK();
//   GP.LABEL("Пароль");
//   GP.PASS("mps", "Password", lp.mqttpass);
//   GP.BREAK();
//   GP.LABEL("Порт");
//   // GP.TEXT("mport", "Port", lp.mqttport);
//   GP.NUMBER("mport", "Port", lp.mqttport);
//   GP.BLOCK_END();
//   //---///
// // GP.BLOCK_BEGIN(GP_THIN, "", "Частота опроса", GP_BLUE);
// //   GP.LABEL("Минут");
// //   GP.NUMBER("Sl_minutes", "Минут", lp.minutes_Sleep);
// //   GP.BREAK();
// //  GP.BLOCK_END();

//   //---///
//   GP.SUBMIT("Записать");
//   GP.FORM_END();

//   GP.BUILD_END();
// }

// //---///////////////////////////////////////////////////
// void actionGP(GyverPortal& p) {
//   if (p.form("/login")) {      // кнопка нажата
//     p.copyStr("lg", lp.ssid);  // копируем себе
//     p.copyStr("ps", lp.pass);
//     p.copyStr("msrv", lp.mqttserv);
//     p.copyStr("muser", lp.mqttlogin);
//     p.copyStr("mps", lp.mqttpass);
//     p.copyInt("mport", lp.mqttport);
// //p.copyInt("Sl_minutes", lp.minutes_Sleep);
//     //---/// записываем
//     writeEEPROM();
//     //---///
//     //  EEPROM.put(_EEPROM_START, lp);              // сохраняем
//     //  EEPROM.commit();                // записываем
//     WiFi.softAPdisconnect();  // отключаем AP
//   }
// }
// //---///////////////////////////////////////////////////

// void loginPortal() {
//   ////////////Serial.println("Portal start");

//   // запускаем точку доступа
//   WiFi.mode(WIFI_AP);
//   WiFi.softAP("WiFi Config");

//   // запускаем портал
//   GyverPortal ui;
//   ui.attachBuild(build);
//   ui.start();
//   ui.attach(actionGP);

// nextapCheckTick=millis();
//   // работа портала
//    while (ui.tick() and (millis() - nextapCheckTick > 300000)); // 5 минут 
  
// }
