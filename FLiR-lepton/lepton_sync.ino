void lepton_sync(void){
  int i;
  int data = 0x0f;

  digitalWriteDirect(22, HIGH);
  delay(185);
  
  while (data & 0x0f == 0x0f){
    digitalWriteDirect(22, LOW);
    data = SPI.transfer(0x00) << 8;
    data |= SPI.transfer(0x00);
    digitalWriteDirect(22, HIGH);

    for (i = 0; i < ((VOSPI_FRAME_SIZE - 2) / 2); i++){
      digitalWriteDirect(22, LOW);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      digitalWriteDirect(22, HIGH);
    }
  }
}
