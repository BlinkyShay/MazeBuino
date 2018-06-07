#include <Gamebuino-Meta.h>

byte playerX, playerY;
byte mazeWidth = 40, mazeHeight = 32;
byte maze[40][32];
byte startY, endY;

bool devTest = false;

const uint16_t playerData[] = {2, 2, 30, 1, 0xffff, 0, 0xf1df, 0xf1df, 0xea1e, 0xea1e, 0xe23d, 0xe23d, 0xda7c, 0xda7c, 0xd29b, 0xd29b, 0xcada, 0xcada, 0xbb19, 0xc319, 0xbb38, 0xb338, 0xab77, 0xb377, 0xab96, 0xab96, 0x9bd5, 0x9bd5, 0x93f4, 0x93f4, 0x9433, 0x8c33, 0x8452, 0x8452, 0x7c91, 0x7c91, 0x74d0, 0x74d0, 0x6cef, 0x6cef, 0x652e, 0x652e, 0x5d4d, 0x5d4d, 0x558c, 0x558c, 0x4dab, 0x4dab, 0x45ea, 0x45ea, 0x3e09, 0x3e09, 0x3648, 0x3648, 0x2e68, 0x2e87, 0x26a6, 0x26a7, 0x1ee5, 0x1ee5, 0x1705, 0x1705, 0xf44, 0xf44, 0x763, 0x763, 0x782, 0x782, 0xf43, 0xf44, 0x1704, 0x1704, 0x1ee5, 0x1ee5, 0x26a6, 0x26a6, 0x2e87, 0x2e87, 0x3648, 0x3648, 0x3e29, 0x3e29, 0x45ea, 0x45ea, 0x4dcb, 0x4dcb, 0x558c, 0x558c, 0x5d4d, 0x5d4d, 0x652e, 0x652e, 0x6cef, 0x6cef, 0x74d0, 0x74d0, 0x7c91, 0x7c91, 0x8472, 0x8472, 0x8c33, 0x8c33, 0x9414, 0x9414, 0x9bd5, 0x9bd5, 0xa3b6, 0xa396, 0xab77, 0xab77, 0xb358, 0xb338, 0xbb19, 0xbb19, 0xc2da, 0xc2da, 0xcabb, 0xcabb, 0xd27b, 0xd27b, 0xe25d, 0xe25c, 0xe21d, 0xe21d, 0xe9fe, 0xe9de};
Image player = Image(playerData);

const uint16_t wall1Data[] = {2, 2, 1, 1, 0, 0, 0x5acb, 0x630c, 0x528a, 0x39c7};
Image wall1 = Image(wall1Data);

const uint16_t wall2Data[] = {2, 2, 1, 1, 0, 0, 0x52aa, 0x39e7, 0x6b4d, 0x52aa};
Image wall2 = Image(wall2Data);

const uint16_t wall3Data[] = {2, 2, 1, 1, 0, 0, 0x39c7, 0x4a49, 0x4a49, 0x5acb};
Image wall3 = Image(wall3Data);

const uint16_t wall4Data[] = {2, 2, 1, 1, 0, 0, 0x4a69, 0x39e7, 0x630c, 0x4a69};
Image wall4 = Image(wall4Data);

const uint16_t wall5Data[] = {2, 2, 1, 1, 0, 0, 0x630c, 0x4a49, 0x4a49, 0x528a};
Image wall5 = Image(wall5Data);

void setup() {
  gb.begin();
  setWallBorder();
  createMaze();
  setWallsColor();
}

void createMazeRoot(byte posx, byte posy) {
  /*
  0 - sciana / wall
  1 - odwiedzona / visited
  2 - odwidzona_wydrazona / visited_part of the maze
  3 - obramowanie labiryntu / maze border
  4,5,6,7,8 - sciana po wybraniu tekstury / wall after texturing  */
  
  byte previousDirection;
  bool endOfRoot = false;
  if ((posx + 2 < mazeWidth && maze[posx + 2][posy] == 0 ) || (posx - 2 >= 0 && maze[posx - 2][posy] == 0 ) || (posy + 2 < mazeHeight && maze[posx][posy + 2] == 0 ) || (  posy - 2 >= 0 && maze[posx][posy - 2] == 0))
  {
    maze[posx][posy] = 1;
    do
    {
      endOfRoot = false;
      previousDirection = random(4) + 1;

      if ( posx + 2 < mazeWidth && previousDirection == 1 && maze[posx + 2][posy] == 0 )
      {
        maze[posx + 1][posy] = 1;
        posx += 2;
      }
      else if ( posx - 2 >= 0 && previousDirection == 2 && maze[posx - 2][posy] == 0 )
      {
        maze[posx - 1][posy] = 1;
        posx -= 2;
      }
      else if ( posy + 2 < mazeHeight && previousDirection == 3 && maze[posx][posy + 2] == 0 )
      {
        maze[posx][posy + 1] = 1;
        posy += 2;
      }
      else if ( posy - 2 >= 0 && previousDirection == 4 && maze[posx][posy - 2] == 0 )
      {
        maze[posx][posy - 1] = 1;
        posy -= 2;
      }
      else
      {
        endOfRoot = true;
      }
    }
    while (endOfRoot);
  }
  else
  {
    maze[posx][posy] = 2;

    if ( posx + 2 < mazeWidth && maze[posx + 2][posy] == 1 && maze[posx + 1][posy] == 1)
    {
      maze[posx + 1][posy] = 2;
      posx += 2;
    }
    else if ( posx - 2 >= 0 && maze[posx - 2][posy] == 1 && maze[posx - 1][posy] == 1)
    {
      maze[posx - 1][posy] = 2;
      posx -= 2;
    }
    else if (posy + 2 < mazeHeight && maze[posx][posy + 2] == 1 && maze[posx][posy + 1] )
    {
      maze[posx][posy + 1] = 2;
      posy += 2;
    }
    else if (posy - 2 >= 0 && maze[posx][posy - 2] == 1 && maze[posx][posy - 1] == 1)
    {
      maze[posx][posy - 1] = 2;
      posy -= 2;
    }
    else
    {
      return;
    }
  }
  createMazeRoot(posx, posy);
}

void setWallBorder() {

  for (byte i = 0; i < mazeWidth; i++) {
    for (byte j = 0; j < mazeHeight; j++) {

      if ( j == 0)
      {
        maze[i][j] = 3;
      }
      else if ( j == mazeHeight - 1)
      {
        maze[i][j] = 3;
      }
      else if ( i == 0)
      {
        maze[i][j] = 3;
      }
      else if ( i == mazeWidth - 1) {
        maze[i][j] = 3;
      }
      else {
        maze[i][j] = 0;
      }
    }
  }
}

void createMaze() {

  byte posx, posy;
  bool previousDirection;

  posx = random(mazeWidth - 2) + 1;
  posy = random(mazeHeight - 2) + 1;

  createMazeRoot(posx, posy);

  //entrance from left and right
  posx = 0;
  do {
    posy = random(mazeHeight - 2) + 1;
  } while (! ( (maze[posx + 1][posy] == 2) || ( (maze[posx + 1][posy] == 0) && (maze[posx + 2][posy] == 2) ) ) );

  maze[posx][posy] = 2;
  maze[posx + 1][posy] = 2;
  playerX = posx;
  playerY = posy;

  posx = mazeWidth - 1;
  do {
    posy = random(mazeHeight - 2) + 1;
  } while (! ( (maze[posx - 1][posy] == 2) || ( (maze[posx - 1][posy] == 0) && (maze[posx - 2][posy] == 2) ) ) );

  maze[posx][posy] = 2;
  maze[posx - 1][posy] = 2;
  endY = posy;

}

bool endOfGame() {
  if ( (playerX == mazeWidth - 1) && (playerY == endY) ) {
    return true;
  }
  return false;
}

void setWallsColor() {
  for (byte i = 0; i < mazeWidth; i++) {
    for (byte j = 0; j < mazeHeight; j++) {

      if (maze[i][j] == 0 || maze[i][j] == 3) {

        byte num = random(5);

        if (num == 0) {
          maze[i][j] = 4;
        } else if (num == 1) {
          maze[i][j] = 5;
        } else if (num == 2) {
          maze[i][j] = 6;
        } else if (num == 3) {
          maze[i][j] = 7;
        } else if (num == 4) {
          maze[i][j] = 8;
        }
      }
    }
  }
}

void drawMaze() {

  for (byte i = 0; i < mazeWidth; i++) {
    for (byte j = 0; j < mazeHeight; j++) {

      if (maze[i][j] == 2) {
        gb.display.setColor(BLACK);
        gb.display.drawPixel(i * 2, j * 2);
        gb.display.drawPixel(i * 2 + 1, j * 2);
        gb.display.drawPixel(i * 2, j * 2 + 1);
        gb.display.drawPixel(i * 2 + 1, j * 2 + 1);
      }
      else if (maze[i][j] == 4) {
        gb.display.drawImage(i * 2, j * 2, wall1);
      }
      else if (maze[i][j] == 5) {
        gb.display.drawImage(i * 2, j * 2, wall2);
      }
      else if (maze[i][j] == 6) {
        gb.display.drawImage(i * 2, j * 2, wall3);
      }
      else if (maze[i][j] == 7) {
        gb.display.drawImage(i * 2, j * 2, wall4);
      }
      else if (maze[i][j] == 8) {
        gb.display.drawImage(i * 2, j * 2, wall5);
      }
      else { // if something go wrong
        gb.display.setColor(RED);
        gb.display.drawPixel(i, j);
      }
    }
  }
}

void drawPlayer() {
  gb.display.drawImage(playerX * 2, playerY * 2, player);
}

void loop() {

  while (!gb.update());
  gb.display.clear();


  if (gb.buttons.pressed( BUTTON_MENU )) {

    setWallBorder();
    createMaze();
    setWallsColor();
  }

  if (gb.buttons.held(BUTTON_B, 10)) {
    if ( devTest == false) {
      devTest = true;
    } else {
      devTest = false;
    }
  }

  drawMaze();
  drawPlayer();

  if (devTest == true) {
    gb.display.setCursorX(62);
    gb.display.setCursorY(0);
    gb.display.setColor(RED);
    gb.display.print("TEST");
  }

  if (!endOfGame()) {
    if (gb.buttons.repeat(BUTTON_UP, 3) && (maze[playerX][playerY - 1] == 2 || devTest == true)) {
      playerY--;
    } else if (gb.buttons.repeat(BUTTON_DOWN, 3) && (maze[playerX][playerY + 1] == 2 || devTest == true)) {
      playerY++;
    } else if (gb.buttons.repeat(BUTTON_RIGHT, 3) && (maze[playerX + 1][playerY] == 2 || devTest == true)) {
      playerX++;
    } else if (gb.buttons.repeat(BUTTON_LEFT, 3) && (maze[playerX - 1][playerY] == 2 || devTest == true)) {
      playerX--;
    }
  } else {
    gb.display.setCursorX(10);
    gb.display.setCursorY(26);
    gb.display.setFontSize(2);
    gb.display.setColor(ORANGE);
    gb.display.print("ESCAPED!");
  }
}
