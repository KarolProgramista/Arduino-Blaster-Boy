#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Button input values
#define ATTACKBUTTONLOW  795
#define ATTACKBUTTONHIGH  825
#define LEFTBUTTONLOW  595
#define LEFTBUTTONHIGH  625
#define RIGHTBUTTONLOW  0
#define RIGHTBUTTONHIGH  10
#define TOPBUTTONLOW  200
#define TOPBUTTONHIGH  220
#define DOWNBUTTONLOW  385
#define DOWNBUTTONHIGH  415

//Reading from button on lcd
int reading;

// Cool down for player gun
int weapon_cool_down = 1000;

// Weapon timer
int weapon_timer = 200;

// Player character
char player_char = '>';

// Bullet struct
typedef struct bullet
{
  char bullet_char = '*';
  int x;
  int y;
  int dir;
  int exist;
}bullet;

// Bullet at screen
bullet bullets[100];

// Button input pin
int button_pin = 0;

int rows = 2;
int columns = 16;

// Player coords varibles
int player_x = 8;
int player_y = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(button_pin, INPUT);
  lcd.begin(rows, columns);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("* Blaster Boy **");
  delay(2000);
}

void get_input()
{
  int reading = analogRead(button_pin);
  if(reading >= ATTACKBUTTONLOW and reading <= ATTACKBUTTONHIGH)
  {
    if(millis() - weapon_timer >= weapon_cool_down)
    {
      attack(); 
    }
  }
  if(reading >= LEFTBUTTONLOW and reading <= LEFTBUTTONHIGH)
  {
    go_left();
  }
  if(reading >= RIGHTBUTTONLOW and reading <= RIGHTBUTTONHIGH)
  {
    go_right();
  }
  if(reading >= TOPBUTTONLOW and reading <= TOPBUTTONHIGH)
  {
    go_top();
  }
  if(reading >= DOWNBUTTONLOW and reading <= DOWNBUTTONHIGH)
  {
    go_down();
  }
}

void attack()
{
  bullet new_bullet = bullet{};
  if(player_char == '>')
  {
    new_bullet.x = player_x + 1;
    new_bullet.y = player_y;
    new_bullet.dir = 1;
  }
  else if(player_char == '<')
  {
    new_bullet.x = player_x - 1;
    new_bullet.y = player_y;
    new_bullet.dir = -1;
  }

  new_bullet.exist = 1;
  int i;
  int bullets_len = sizeof(bullets) / sizeof(bullet);

  for (i=0; i < bullets_len; i++)
  {
    bullet current_bullet = bullets[i];
    if(current_bullet.exist == 0)
    {
      bullets[i] = new_bullet;
      break;;
    }
  }
  weapon_timer = millis();
}

void go_left()
{
  if(player_x > 0)
  {
    player_x --;
    player_char = '<';
  }
}

void go_right()
{
  if(player_x < 15)
  {
    player_x ++;
    player_char = '>';
  }
}

void go_top()
{
  if(player_y == 1)
  {
    player_y = 0;
  }
}

void go_down()
{
  if(player_y == 0)
  {
    player_y = 1; 
  }
}

void draw_player()
{
  lcd.setCursor(player_x, player_y);
  lcd.write(player_char);
}

void draw_bullets()
{
  int i;
  int bullets_len = sizeof(bullets) / sizeof(bullet);
  for(i=0; i < bullets_len; i++)
  {
    if(bullets[i].exist == 1)
    {
      if(bullets[i].dir == 1 and bullets[i].x == 16)
      {
        bullets[i].exist = 0;
      }
      else if(bullets[i].dir == -1 and bullets[i].x == 0)
      {
        bullets[i].exist = 0;
      }
      else
      {
        bullets[i].x = bullets[i].x + bullets[i].dir;
        Serial.println(bullets[i].x);
        lcd.setCursor(bullets[i].x, bullets[i].y);
        lcd.write(bullets[i].bullet_char);    
      }

    }

  }
}

void loop() 
{
  lcd.clear();
  draw_player();
  draw_bullets();
  
  get_input();

  delay(100);

}
