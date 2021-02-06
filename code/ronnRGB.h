/*
 * LED RGB FUNCTION 
 * ***********************************************************************************
 * Code by : fahroni|ganteng
 * contact me : fahroniganteng@gmail.com
 * Date : jan 2021
 * License :  MIT
 * 
 * Some comment in indonesian language.... sorry ^___^
 */



/*
 * COLOR MODE
 * ***************************************
 */
int rgb_modeColor1[] = {//RGB
  255,0,0,//red
  0,255,0,//green
  0,0,255,//blue
};
int rgb_modeColor2[] = {//mode 2
  255,200,20,
  70,255,70,
  150,150,255,
  255,255,255,
};
int rgb_modeColor3[] = {//mix all rgb_color
  255,  0,  0,
  255,  0,255,
  255,255,255,
  255,0  ,255,
  0  ,0  ,255,
  0  ,255,255,
  255,255,255,
  0  ,255,255,
  0  ,255,  0,
  255,255,  0,
  255,255,255,
  255,255,  0,
};

int rgb_brig[2];//rgb_brigHTNESS {LOW,HIGH} ==> if common anode will reverse
int rgb_index[3] = {0,0};
int rgb_timer;
int *rgb_color;
int rgb_colorSize;
int rgb_speed = 5;

void rgb_runFadeMode(int v_rgb_color[], int sizeOfArrrgb_color, int i){
  rgb_color = v_rgb_color;
  rgb_colorSize = sizeOfArrrgb_color;
  tmrRGB.stop(rgb_timer);
  i=(i< rgb_colorSize/3)?i:0;//jumlah / 3 ==> R+G+B ambil 3 rgb_index
  rgb_index[0] = i;  //rgb_index rgb_color existing
  rgb_index[1] = (i+1 < rgb_colorSize/3)?i+1:0;//rgb_index ganti ke rgb_color selanjutnya; ==> jika warna terakhir ganti ke warna pertama
  rgb_index[2] = 0;  //rgb_index looping perubahan rgb_color
  
  rgb_timer = tmrRGB.every(50,[](){ //rgb_timer every 100(ms) * rgb_speed(detik) * 10(ms) = rgb_speed (ms)
    int buf;
    for (int i = 0; i < arrSize(PIN_RGB); i++) {
      buf = map(rgb_index[2], 0, rgb_speed*20, rgb_color[rgb_index[0]*3+i], rgb_color[rgb_index[1]*3+i]);
      buf = map(buf, 0, 255, rgb_brig[0], rgb_brig[1]);
      analogWrite(PIN_RGB[i], buf);
      //PRINT(String(buf) + ", ");
    }
    //PRINTLN();
    
    if(rgb_index[2]>=rgb_speed*20){ //rgb_timer every 100(ms) * rgb_speed(detik) * 10(ms) = rgb_speed (ms)
      tmrRGB.stop(rgb_timer);
      rgb_runFadeMode(rgb_color,rgb_colorSize,rgb_index[0]+1);
    }
    else rgb_index[2]++;
  });
}

void rgb_runBlinkMode(int v_rgb_color[], int sizeOfArrrgb_color, int i){
  if(i==0){
    rgb_color = v_rgb_color;
    rgb_colorSize = sizeOfArrrgb_color;
    tmrRGB.stop(rgb_timer);
  }
  i=(i< rgb_colorSize/3)?i:0;//jumlah / 3 ==> R+G+B ambil 3 rgb_index
  rgb_index[0] = i;  //rgb_index rgb_color existing
  rgb_index[1] = (i+1 < rgb_colorSize/3)?i+1:0;//rgb_index ganti ke rgb_color selanjutnya; ==> jika warna terakhir ganti ke warna pertama
  rgb_index[2] = 0;  //rgb_index looping perubahan rgb_color
  
  rgb_timer = tmrRGB.every(rgb_speed*10,[](){ //rgb_timer every 10(ms) * rgb_speed(s) * 10(ms) = rgb_speed (ms)
    int buf;
    if(rgb_index[2]%10==0 || (rgb_index[2]-2)%10==0){
      for (int i = 0; i < arrSize(PIN_RGB); i++) {
        if(E_MODE==4)//manual blink
          buf = E_RGB[i];
        else
          buf = map(rgb_index[2], 0, 99, rgb_color[rgb_index[0]*3+i], rgb_color[rgb_index[1]*3+i]);
          
        buf = map(buf, 0, 255, rgb_brig[0], rgb_brig[1]);      
        analogWrite(PIN_RGB[i], buf);
      }
    }
    else{
      digitalWrite(PIN_RGB[0], COMMON_ANODE?HIGH:LOW);
      digitalWrite(PIN_RGB[1], COMMON_ANODE?HIGH:LOW);
      digitalWrite(PIN_RGB[2], COMMON_ANODE?HIGH:LOW);
    }
    if(rgb_index[2]<99){ //rgb_timer every 100(ms) * rgb_speed(detik) * 10(ms) = rgb_speed (ms)
      rgb_index[2]++;
    }
    else {
      rgb_index[0] = (rgb_index[0]< rgb_colorSize/3)?rgb_index[0]+1:0;//jumlah / 3 ==> R+G+B ambil 3 rgb_index
      rgb_index[1] = (rgb_index[0]+1 < rgb_colorSize/3)?rgb_index[0]+1:0;//rgb_index ganti ke rgb_color selanjutnya; ==> jika warna terakhir ganti ke warna pertama
      rgb_index[2] = 0;  //rgb_index looping perubahan rgb_color
    }
  });
}
void rgb_runManualMode(){
  tmrRGB.stop(rgb_timer);
  int buf;
  for (int i = 0; i < arrSize(PIN_RGB); i++) {
    buf = map(E_RGB[i], 0, 255, rgb_brig[0], rgb_brig[1]);
    analogWrite(PIN_RGB[i], buf);
    //PRINT(String(buf) + ", ");
  }
  //PRINTLN();
}
void rgb_runManualFadeMode(){
  tmrRGB.stop(rgb_timer);
  rgb_index[0] = 0;
  rgb_timer = tmrRGB.every(50,[](){
    int buf;
    for (int i = 0; i < arrSize(PIN_RGB); i++) {
        buf = E_RGB[i];
        if(rgb_index[0]<rgb_speed*20/2)
          buf = map(rgb_index[0], 0-(rgb_speed*20/5), rgb_speed*20/2, 0, buf);
        else
          buf = map(rgb_index[0], rgb_speed*20/2, (rgb_speed*20)+(rgb_speed*20/5), buf, 0);
        buf = map(buf, 0, 255, rgb_brig[0], rgb_brig[1]);
        analogWrite(PIN_RGB[i], buf);
        //PRINT(String(buf) + ", ");
    }
    //PRINTLN("");
    rgb_index[0] = rgb_index[0]>=rgb_speed*20?0:rgb_index[0]+1;
  });
}
void rgb_runCandleMode(){
  tmrRGB.stop(rgb_timer);
  rgb_timer = tmrRGB.every(200,[](){
    int buf;
    int rnd = random(0,30); //random 10%
    for (int i = 0; i < arrSize(PIN_RGB); i++){
      buf = E_RGB[i] - (rnd * E_RGB[i]/100);
      buf = buf<0?0:buf;
      buf = map(buf, 0, 255, rgb_brig[0], rgb_brig[1]);
      analogWrite(PIN_RGB[i], buf);
      //PRINT(String(buf)+ ", ");
    }
     //PRINTLN("");
  });
}


void rgb_checkBrightness(){
  //minimal rgb_brigHTNESS = 10 ==> ARDUINO analogWrite 0-255 ==> if use ESP change to 1023 (analogWrite ESP8266 : 0-1023)
  rgb_brig[0]  = COMMON_ANODE?255:0;
  rgb_brig[1]  = COMMON_ANODE?map(E_BRIG,1,255,245,0):map(E_BRIG,1,255,10,255);
}
void rgb_stop(){
  tmrRGB.stop(rgb_timer);
  for (int i = 0; i < arrSize(PIN_RGB); i++) {
    digitalWrite(PIN_RGB[i],COMMON_ANODE?HIGH:LOW); // turn OFF RGB led
  }
}
void rgb_start(){
  PRINT("Lamp Mode : ");
  PRINTLN(E_MODE);
  rgb_checkBrightness();
  switch(E_MODE){
    case 1: rgb_runManualMode();  break;  //manual color
    case 2: rgb_runCandleMode(); break;  // manual color - candle mode
    case 3: rgb_runManualFadeMode();break;  //manual color - fade mode
    case 4: rgb_runBlinkMode(rgb_modeColor1,3,0); break;// manual color - blink mode
    
    //FADE---------------------------------------
    case 5: rgb_runFadeMode(rgb_modeColor1,arrSize(rgb_modeColor1),0); break; // color mode 1 - fade
    case 6: rgb_runFadeMode(rgb_modeColor2,arrSize(rgb_modeColor2),0); break; // color mode 2 - fade
    case 7: rgb_runFadeMode(rgb_modeColor3,arrSize(rgb_modeColor3),0); break; // color mode 3 - fade
    
    //BLINK----------------------------------------
    case 8: rgb_runBlinkMode(rgb_modeColor1,arrSize(rgb_modeColor1),0); break; // color mode 1 - blink
    case 9: rgb_runBlinkMode(rgb_modeColor2,arrSize(rgb_modeColor2),0); break; // color mode 2 - blink
    case 10: rgb_runBlinkMode(rgb_modeColor3,arrSize(rgb_modeColor3),0); break; // color mode 2 - blink
  }
}

void rgb_starting(){ // fade in-out green 
  E_BRIG = 100;
  rgb_checkBrightness();
  for(int i=0;i<100;i++){
    i<50?
      analogWrite(PIN_RGB[1], map(i, 0, 49, rgb_brig[0], rgb_brig[1])):
      analogWrite(PIN_RGB[1], map(i, 99, 50, rgb_brig[0], rgb_brig[1]));
    delay(10);
  }
  E_BRIG = EEPROM[BRIG];
  rgb_stop();
}
void rgb_blinkInfo(int blinkCount, int col[3]){ // blink count, color
  for(int x=0;x<blinkCount;x++){
    for (int i = 0; i < arrSize(PIN_RGB); i++)
      digitalWrite(PIN_RGB[i],COMMON_ANODE?HIGH:LOW); // turn OFF RGB led
    delay(50);
    for (int i = 0; i < arrSize(PIN_RGB); i++)
      analogWrite(PIN_RGB[i], map(col[i], 0, 255, rgb_brig[0], rgb_brig[1]));
    delay(50);
  }
  delay(50);
}

void rgb_changeMode(){
  rgb_stop();
  E_MODE = E_MODE>=10?1:E_MODE+1;
  rgb_start();
}


//
void rgb_viewColor(){
  int buf;
  for (int i = 0; i < arrSize(PIN_RGB); i++){
    buf = map(E_RGB[i], 0, 255, rgb_brig[0], rgb_brig[1]);
    analogWrite(PIN_RGB[i], buf);
  }
}

// change brightness
void rgb_step_increaseBrightness(){
  E_BRIG = E_BRIG<255?E_BRIG+1:255;
  if(E_BRIG==255)rgb_blinkInfo(1,red);
  rgb_checkBrightness();
  rgb_viewColor();
}
void rgb_step_decreaseBrightness(){
  E_BRIG = E_BRIG>1?E_BRIG-1:1;
  if(E_BRIG==1)rgb_blinkInfo(1,red);
  rgb_checkBrightness();
  rgb_viewColor();
}
void rgb_increaseBrightness(){
  E_BRIG = E_BRIG+50>255?255:E_BRIG+50;
  if(E_BRIG==255)rgb_blinkInfo(1,red);
  rgb_checkBrightness();
  rgb_viewColor();
}
void rgb_decreaseBrightness(){
  E_BRIG = E_BRIG-50<1?1:E_BRIG-50;
  if(E_BRIG==1)rgb_blinkInfo(1,red);
  rgb_checkBrightness();
  rgb_viewColor();
}

//change color
void rgb_changeColor(){
  int col[][4] ={ // R,G,B,step_chColor
    {255,255,255,0}, //0 white 
    {255,150,50,0}, //1 warm
    {255,0,0,0}, //2 red
    {255,0,255,256}, //3 pink
    {0,0,255,512}, //4 blue
    {0,255,255,768}, //5 cyan
    {0,255,0,1024}, //6 green
    {255,255,0,1280} //7 yellow
  };
  chColor = chColor+1>=arrSize(col)?0:chColor+1;
  for(int i=0;i<3;i++)
    E_RGB[i] = col[chColor][i];

  step_chColor = col[chColor][3];
  rgb_viewColor();
  
  //DEBUG
  PRINTLN("RGB : "+String(E_RGB[0]) + " " + String(E_RGB[1]) + " " + String(E_RGB[2]))
}
void rgb_step_changeColor(){
  step_chColor = step_chColor+1>1535?0:step_chColor+1;
  //  0 - 255 red-pink
  if(step_chColor<=255){
    chColor = 2;
    E_RGB[0] = 255;
    E_RGB[1] = 0;
    E_RGB[2] = map(step_chColor,0,255,0,255);
  }
  //  256 - 511 pink-blue
  else if(step_chColor<=511){
    chColor = 3;
    E_RGB[0] = map(step_chColor,256,511,255,0);
    E_RGB[1] = 0;
    E_RGB[2] = 255;
  }
  //  512 - 767 blue-cyan
  else if(step_chColor<=767){
    chColor = 4;
    E_RGB[0] = 0;
    E_RGB[1] = map(step_chColor,512,767,0,255);
    E_RGB[2] = 255;
  }
  //  768 - 1023  cyan-green
  else if(step_chColor<=1023){
    chColor = 5;
    E_RGB[0] = 0;
    E_RGB[1] = 255;
    E_RGB[2] = map(step_chColor,768,1023,255,0);
  }
  //  1024  - 1279  green-yellow
  else if(step_chColor<=1279){
    chColor = 6;
    E_RGB[0] = map(step_chColor,1024,1279,0,255);
    E_RGB[1] = 255;
    E_RGB[2] = 0;
  }
  //  1280  - 1535  yellow-red
  else if(step_chColor<=1535){
    chColor = 7;
    E_RGB[0] = 255;
    E_RGB[1] = map(step_chColor,1280,1535,255,0);
    E_RGB[2] = 0;
  }

  rgb_viewColor();
}
