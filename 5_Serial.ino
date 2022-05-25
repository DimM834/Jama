////---////////////////////////////////
//// функция для расчёта crc
//byte crc8_bytes(byte *buffer, byte size) {
//  byte crc = 0;
//  for (byte i = 0; i < size; i++) {
//    byte data = buffer[i];
//    for (int j = 8; j > 0; j--) {
//      crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
//      data >>= 1;
//    }
//  }
//  return crc;
//}
//
//void SerialReadArduino() 
//{
//  boolean Flag_Bottom, Flag_Top, Flag_Relay_Work ;
//
//  Flag_Bottom = BUF_SER.Flag_Ur_N;
//  Flag_Top = BUF_SER.Flag_Ur_V;
//  Flag_Relay_Work = BUF_SER.Flag_Relay;
//
//  //---//////////////////////////////
//  // читаем родным методом readBytes()
//  // указываем ему буфер-структуру, но приводим тип к byte*
//  // размер можно указать через sizeof()
//  if (Serial.readBytes((byte*)&BUF_SER, sizeof(BUF_SER))) {
//    // считаем crc пакета:
//    // передаём буфер, преобразовав его к (byte*)
//    // а также его ПОЛНЫЙ размер, включая байт crc
//    byte crc = crc8_bytes((byte*)&BUF_SER, sizeof(BUF_SER));
//    // если crc равен 0, данные верны (такой у него алгоритм расчёта)
//    if (crc == 0) {
//      // были изменения
//      if (Flag_Bottom != BUF_SER.Flag_Ur_N or
//          Flag_Top != BUF_SER.Flag_Ur_V or
//          Flag_Relay_Work != BUF_SER.Flag_Relay) {
//        // ставим на отправку MQTT
//        sendMQTT_Serial ();
//
//      }
//    }
//
//    else {
//      //Восстанавливаем значения
//      BUF_SER.Value_N = 1023;
//      BUF_SER.Value_V = 1023;
//      BUF_SER.Flag_Ur_N = Flag_Bottom ;
//      BUF_SER.Flag_Ur_V = Flag_Top;
//      BUF_SER.Flag_Relay = Flag_Relay_Work ;
//      //---///Заглушка!!!
//      //sendMQTT_Serial ();
//    }
//    //Serial.print("Work = ");
//    //Serial.println(BUF_SER.Work);
//
//    //Serial.print("Value_N = ");
//    //Serial.println(BUF_SER.Value_N);
//
//    //Serial.print("Value_V = ");
//    //Serial.println(BUF_SER.Value_V);
//
//    //Serial.print("Flag_Ur_N = ");
//    //Serial.println(BUF_SER.Flag_Ur_N);
//
//    //Serial.print("Flag_Ur_V = ");
//    //Serial.println(BUF_SER.Flag_Ur_V);
//
//    //Serial.print("Flag_Relay = ");
//    //Serial.println(BUF_SER.Flag_Relay);
//  }
//}
