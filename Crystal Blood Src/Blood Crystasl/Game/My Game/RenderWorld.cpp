/// \file renderworld.cpp
/// \brief Code for the render world CRenderWorld.

#include "renderworld.h"

/// Load game images. Gets file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ //load images
  InitBackground();
  LoadBackground(); 
  LoadSplash();

  //Numbers
  Load(N_0, "0");
  Load(N_1, "1");
  Load(N_2, "2");
  Load(N_3, "3");
  Load(N_4, "4");
  Load(N_5, "5");
  Load(N_6, "6");
  Load(N_7, "7");
  Load(N_8, "8");
  Load(N_9, "9");
  Load(N_10, "10");
  
  //Load sprites
  Load(CURSOR, "cursor");
  Load(CURSOR_HAND, "cursor_hand");
  Load(START, "start");
  Load(START_HOVER, "hoverStart");
  Load(ENDTURN, "endTurn");
  Load(ENDTURNHOVER, "endTurnHover");
  Load(ATTACKACTIVE, "attackActive");
  Load(ATTACK, "attack");
  Load(ATTACKHOVER, "attackHover");
  Load(WAIT, "wait");
  Load(WAITHOVER, "waitHover");
  Load(WAITACTIVE, "waitActive");
  Load(CAPTURE, "capture");
  Load(CAPTUREHOVER, "capHover");
  Load(CAPTUREACTIVE, "capActive");
  Load(INFO, "info");
  Load(INFOHOVER, "infoHover");
  Load(SIDE1, "side1");
  Load(SIDE2, "side2");
  Load(WIN1, "win1");
  Load(WIN2, "win2");
  Load(CROSSHAIR, "crosshair");

  //attack animation
  Load(ATTACK0, "attack0");
  Load(ATTACK1, "attack1");
  Load(ATTACK2, "attack2");
  Load(ATTACK3, "attack3");

  //Blood Crystle
  Load(BLOODCRYSTLE0, "bCrystle0");
  Load(BLOODCRYSTLE1, "bCrystle1");
  Load(BLOODCRYSTLE2, "bCrystle2");
  Load(BLOODCRYSTLE3, "bCrystle3");
  Load(BLOODCRYSTLE4, "bCrystle4");
  
  //infoPane
  Load(INFOPANE, "infoPane");
  Load(HEALTH_ICON, "health");
  Load(DEFENSE_ICON, "defense");
  Load(MOVE_ICON, "move");
  Load(ATTACK_ICON, "attack");

  Load(TITLE_NAME, "title");
  Load(BLOOD, "blood");

  //Arrow
  Load(DOWNA, "downA");
  Load(DOWNBUTT, "downButt");
  Load(HORZ, "horz");
  Load(LEFTA, "leftA");
  Load(LEFTBUTT, "leftButt");
  Load(LEFTUP, "leftUp");
  Load(RIGHTA, "rightA");
  Load(RIGHTBUTT, "rightButt");
  Load(RIGHTDOWN, "rightDown");
  Load(RIGHTUP, "rightUp");
  Load(UPA, "upA");
  Load(UPBUTT, "upButt");
  Load(VERT, "vert");

  //blue soldier
  Load(BSIDLE0, "BSidle0");
  Load(BSIDLE1, "BSidle1");
  Load(BSIDLE2, "BSidle2");
  Load(BSIDLE3, "BSidle3");

  Load(BSRIGHT0, "BSright0");
  Load(BSRIGHT1, "BSright1");
  Load(BSRIGHT2, "BSright2");
  Load(BSRIGHT3, "BSright3");

  Load(BSLEFT0, "BSleft0");
  Load(BSLEFT1, "BSleft1");
  Load(BSLEFT2, "BSleft2");
  Load(BSLEFT3, "BSleft3");

  Load(BSUP0, "BSup0");
  Load(BSUP1, "BSup1");
  Load(BSUP2, "BSup2");
  Load(BSUP3, "BSup3");

  Load(BSDOWN0, "BSdown0");
  Load(BSDOWN1, "BSdown1");
  Load(BSDOWN2, "BSdown2");
  Load(BSDOWN3, "BSdown3");

  Load(BSMOVED0, "BSmoved0");
  Load(BSMOVED1, "BSmoved1");
  Load(BSMOVED2, "BSmoved2");
  Load(BSMOVED3, "BSmoved3");

  //blue heavy soldier
  Load(BHIDLE0, "BHidle0");
  Load(BHIDLE1, "BHidle1");
  Load(BHIDLE2, "BHidle0");
  Load(BHIDLE3, "BHidle1");

  Load(BHRIGHT0, "BHright0");
  Load(BHRIGHT1, "BHright1");
  Load(BHRIGHT2, "BHright2");
  Load(BHRIGHT3, "BHright3");

  Load(BHLEFT0, "BHleft0");
  Load(BHLEFT1, "BHleft1");
  Load(BHLEFT2, "BHleft2");
  Load(BHLEFT3, "BHleft3");

  Load(BHUP0, "BHup0");
  Load(BHUP1, "BHup1");
  Load(BHUP2, "BHup2");
  Load(BHUP3, "BHup3");

  Load(BHDOWN0, "BHdown0");
  Load(BHDOWN1, "BHdown1");
  Load(BHDOWN2, "BHdown2");
  Load(BHDOWN3, "BHdown3");

  Load(BHMOVED0, "BHmoved0");
  Load(BHMOVED1, "BHmoved1");
  Load(BHMOVED2, "BHmoved0");
  Load(BHMOVED3, "BHmoved1");

  //blue tank
  Load(BTIDLE0, "BTidle0");
  Load(BTIDLE1, "BTidle1");
  Load(BTIDLE2, "BTidle2");
  Load(BTIDLE3, "BTidle1");

  Load(BTRIGHT0, "BTright0");
  Load(BTRIGHT1, "BTright1");
  Load(BTRIGHT2, "BTright2");
  Load(BTRIGHT3, "BTright1");

  Load(BTLEFT0, "BTleft0");
  Load(BTLEFT1, "BTleft1");
  Load(BTLEFT2, "BTleft2");
  Load(BTLEFT3, "BTleft1");

  Load(BTUP0, "BTup0");
  Load(BTUP1, "BTup1");
  Load(BTUP2, "BTup2");
  Load(BTUP3, "BTup1");

  Load(BTDOWN0, "BTdown0");
  Load(BTDOWN1, "BTdown1");
  Load(BTDOWN2, "BTdown2");
  Load(BTDOWN3, "BTdown1");

  Load(BTMOVED0, "BTmoved0");
  Load(BTMOVED1, "BTmoved1");
  Load(BTMOVED2, "BTmoved2");
  Load(BTMOVED3, "BTmoved1");
  
  //red soldier
  Load(RSIDLE0, "RSidle0");
  Load(RSIDLE1, "RSidle1");
  Load(RSIDLE2, "RSidle2");
  Load(RSIDLE3, "RSidle3");

  Load(RSRIGHT0, "RSright0");
  Load(RSRIGHT1, "RSright1");
  Load(RSRIGHT2, "RSright2");
  Load(RSRIGHT3, "RSright3");

  Load(RSLEFT0, "RSleft0");
  Load(RSLEFT1, "RSleft1");
  Load(RSLEFT2, "RSleft2");
  Load(RSLEFT3, "RSleft3");

  Load(RSUP0, "RSup0");
  Load(RSUP1, "RSup1");
  Load(RSUP2, "RSup2");
  Load(RSUP3, "RSup3");

  Load(RSDOWN0, "RSdown0");
  Load(RSDOWN1, "RSdown1");
  Load(RSDOWN2, "RSdown2");
  Load(RSDOWN3, "RSdown3");

  Load(RSMOVED0, "RSmoved0");
  Load(RSMOVED1, "RSmoved1");
  Load(RSMOVED2, "RSmoved2");
  Load(RSMOVED3, "RSmoved3");

  //red heavy soldier
  Load(RHIDLE0, "RHidle0");
  Load(RHIDLE1, "RHidle1");
  Load(RHIDLE2, "RHidle0");
  Load(RHIDLE3, "RHidle1");

  Load(RHRIGHT0, "RHright0");
  Load(RHRIGHT1, "RHright1");
  Load(RHRIGHT2, "RHright2");
  Load(RHRIGHT3, "RHright3");

  Load(RHLEFT0, "RHleft0");
  Load(RHLEFT1, "RHleft1");
  Load(RHLEFT2, "RHleft2");
  Load(RHLEFT3, "RHleft3");

  Load(RHUP0, "RHup0");
  Load(RHUP1, "RHup1");
  Load(RHUP2, "RHup2");
  Load(RHUP3, "RHup3");

  Load(RHDOWN0, "RHdown0");
  Load(RHDOWN1, "RHdown1");
  Load(RHDOWN2, "RHdown2");
  Load(RHDOWN3, "RHdown3");

  Load(RHMOVED0, "RHmoved0");
  Load(RHMOVED1, "RHmoved1");
  Load(RHMOVED2, "RHmoved0");
  Load(RHMOVED3, "RHmoved1");

  //red tank
  Load(RTIDLE0, "RTidle0");
  Load(RTIDLE1, "RTidle1");
  Load(RTIDLE2, "RTidle2");
  Load(RTIDLE3, "RTidle1");

  Load(RTRIGHT0, "RTright0");
  Load(RTRIGHT1, "RTright1");
  Load(RTRIGHT2, "RTright2");
  Load(RTRIGHT3, "RTright1");

  Load(RTLEFT0, "RTleft0");
  Load(RTLEFT1, "RTleft1");
  Load(RTLEFT2, "RTleft2");
  Load(RTLEFT3, "RTleft1");

  Load(RTUP0, "RTup0");
  Load(RTUP1, "RTup1");
  Load(RTUP2, "RTup2");
  Load(RTUP3, "RTup1");

  Load(RTDOWN0, "RTdown0");
  Load(RTDOWN1, "RTdown1");
  Load(RTDOWN2, "RTdown2");
  Load(RTDOWN3, "RTdown1");

  Load(RTMOVED0, "RTmoved0");
  Load(RTMOVED1, "RTmoved1");
  Load(RTMOVED2, "RTmoved2");
  Load(RTMOVED3, "RTmoved1");

  //map tiles
  Load(RED_BASE, "redBase"); 
  Load(BLUE_BASE, "blueBase");
  Load(GRASS0, "grass0"); 
  Load(GRASS1, "grass1");
  Load(MOUNTAIN0, "mountain0");
  Load(MOUNTAIN1, "mountain1");
  Load(MOUNTAIN2, "mountain2");
  Load(MOUNTAIN3, "mountain3");
  Load(MOUNTAIN4, "mountain4");
  Load(MOUNTAIN5, "mountain5");
  Load(RIVER0, "river0");
  Load(RIVER1, "river1");
  Load(RIVER2, "river2");
  Load(ROAD0, "road0");
  Load(ROAD1, "road1");
  Load(ROAD2, "road2");
  Load(ROAD3, "road3");
  Load(ROAD4, "road4");
  Load(ROAD5, "road5");
  Load(ROAD6, "road6");
  Load(ROAD7, "road7");
  Load(ROAD8, "road8");
  Load(ROAD9, "road9");
  Load(ROAD10, "road10");
  Load(TREE0, "tree0");
  Load(TREE1, "tree1");
  Load(WALLPAPER, "greyWallpaper");
  //RED MAP TILES
  Load(RED_BASE_R, "redBase_r");
  Load(BLUE_BASE_R, "blueBase_r");
  Load(GRASS0_R, "grass0_r");
  Load(GRASS1_R, "grass1_r");
  Load(MOUNTAIN0_R, "mountain0_r");
  Load(MOUNTAIN1_R, "mountain1_r");
  Load(MOUNTAIN2_R, "mountain2_r");
  Load(MOUNTAIN3_R, "mountain3_r");
  Load(MOUNTAIN4_R, "mountain4_r");
  Load(MOUNTAIN5_R, "mountain5_r");
  Load(ROAD0_R, "road0_r");
  Load(ROAD1_R, "road1_r");
  Load(ROAD2_R, "road2_r");
  Load(ROAD3_R, "road3_r");
  Load(ROAD4_R, "road4_r");
  Load(ROAD5_R, "road5_r");
  Load(ROAD6_R, "road6_r");
  Load(ROAD7_R, "road7_r");
  Load(ROAD8_R, "road8_r");
  Load(ROAD9_R, "road9_r");
  Load(ROAD10_R, "road10_r");
  Load(TREE0_R, "tree0_r");
  Load(TREE1_R, "tree1_r");
  Load(WALLPAPER_R, "redWallpaper");
  //BLUE MAP TILES
  Load(RED_BASE_B, "redBase_b");
  Load(BLUE_BASE_B, "blueBase_b");
  Load(GRASS0_B, "grass0_b");
  Load(GRASS1_B, "grass1_b");
  Load(MOUNTAIN0_B, "mountain0_b");
  Load(MOUNTAIN1_B, "mountain1_b");
  Load(MOUNTAIN2_B, "mountain2_b");
  Load(MOUNTAIN3_B, "mountain3_b");
  Load(MOUNTAIN4_B, "mountain4_b");
  Load(MOUNTAIN5_B, "mountain5_b");
  Load(ROAD0_B, "road0_b");
  Load(ROAD1_B, "road1_b");
  Load(ROAD2_B, "road2_b");
  Load(ROAD3_B, "road3_b");
  Load(ROAD4_B, "road4_b");
  Load(ROAD5_B, "road5_b");
  Load(ROAD6_B, "road6_b");
  Load(ROAD7_B, "road7_b");
  Load(ROAD8_B, "road8_b");
  Load(ROAD9_B, "road9_b");
  Load(ROAD10_B, "road10_b");
  Load(TREE0_B, "tree0_b");
  Load(TREE1_B, "tree1_b");
  Load(WALLPAPER_B, "blueWallpaper");
  
  Load(EX0, "ex0");
  Load(EX1, "ex1");
  Load(EX2, "ex2");
  Load(EX3, "ex3");
  Load(EX4, "ex4");
  Load(EX5, "ex5");
  Load(EX6, "ex6");
  Load(EX7, "ex7");
  Load(EX8, "ex8");
  Load(EX9, "ex9");
  Load(EX10, "ex10");
  Load(EX11, "ex11");
  Load(EX12, "ex12");
  Load(EX13, "ex13");
  Load(EX14, "ex14");
  Load(EX15, "ex15");
  Load(EX16, "ex16");
  Load(EX17, "ex17");
  Load(EX18, "ex18");
  Load(EX19, "ex19");
  Load(EX20, "ex20");
  Load(EX21, "ex21");
  Load(EX22, "ex22");
  Load(EX23, "ex23");
} //LoadImages