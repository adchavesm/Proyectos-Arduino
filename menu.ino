#include "U8glib.h"
#include <Button.h>
#include <TimerOne.h>
//-----------------------------------------------------------------------------------------
//U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9, 8);  // D0=13, D1=11, CS=10, DC=9, Reset=8
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST);  // Dev 0, Fast I2C / TWI
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_FAST); // Dev 0, Fast I2C / TWI
//-----------------------------------------------------------------------------------------

const unsigned char campana[] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x08,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1C,
    0x00,
    0x00,
    0xE3,
    0x80,
    0x01,
    0x00,
    0x40,
    0x01,
    0x10,
    0x40,
    0x02,
    0x20,
    0x20,
    0x04,
    0x40,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x80,
    0x10,
    0x04,
    0x3F,
    0x90,
    0x1C,
    0x00,
    0x1C,
    0x20,
    0x00,
    0x02,
    0x20,
    0x00,
    0x02,
    0x1F,
    0xFF,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

//------Buttons:Keys------------------------------------------------------------------------
Button keyPrev(2, true, true, 25);
Button keyBack(3, true, true, 25);
Button keyNext(4, true, true, 25);
Button keySelect(5, true, true, 25);

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4
#define ON 1
#define OFF 0
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second = 55;
unsigned char minute = 59;
unsigned char hour = 23;
unsigned char day = 6;
unsigned char month = 11;
unsigned int year = 2019;
unsigned char minuteM = 59;
unsigned char hourM = 23;
unsigned char dayM = 6;
unsigned char monthM = 11;
unsigned int yearM = 2019;
int m = 0;
unsigned char secondA = 0;
unsigned char minuteA = 0;
unsigned char hourA = 00;
int al = 0;
boolean alarma = true;

const char *mes[7] = {"ALARMA!", "ENTER FOR OUT", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const char *menu_days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
uint8_t keyCode = KEY_NONE;
bool drawFlag = true;
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------

//-----------------Menu---------------------------------------------------------------------
#define MENU_ITEMS 3
const char *menu_strings[MENU_ITEMS] = {"Set Time", "SetAlarm", "Otro"};
uint8_t menu_current; //seleccion actual del menu
uint8_t page_redraw;  //activacion redibujar pagina
//------------------------------------------------------------------------------------------

//-----------Draw pages---------------------------------------------------------------------
#define PAGE_ITEMS 5
int p;
int numerador = 0; //drawPage selector
void draw();
void pageMenu(); // drawPage-0
void SetTime();  // drawPage-1
void SetAlarm(); // drawPage-2
void pageInfo(); // drawPage-4
void (*pages[PAGE_ITEMS])() = {draw, pageMenu, SetTime, SetAlarm, pageInfo};
int x1, y1;
int x2, y2, y3;
long currentMillis;
//Task1_AnimatePage0
long previousMillis1;
long previousMillis2;
long previousMillis3;
long previousMillis5;
long previousMillis4;

bool forward = true;
//------------------------------------------------------------------------------------------

//---------------------setup and loop-------------------------------------------------------

void setup()
{
    u8g.setFont(u8g_font_6x13);
    menu_current = 0;
    page_redraw = 1; // force initial redraw
    keyCode = KEY_NONE;
    drawFlag == true;
    Timer1.initialize(500000);         //timing for 500ms
    Timer1.attachInterrupt(TimingISR); //declare the interrupt serve routine:TimingISR
}

void loop()
{
    u8g.firstPage();
    do
    {
        (*pages[p])();
    } while (u8g.nextPage());
    currentMillis = millis();
    // Task3_AnimatePage();
    keyNext.read();
    keyPrev.read();
    keySelect.read();
    keyBack.read();
    readKeys();    // check for key press
    processKeys(); // process the pressed key
}
//----------------------------------------------------------------------------------------------------
//--------------read and processing keys--------------------------------------------------------------
void readKeys(void)
{
    if (keyNext.wasPressed())
        keyCode = KEY_NEXT;
    else if (keyPrev.wasPressed())
        keyCode = KEY_PREV;
    else if (keyBack.wasPressed())
        keyCode = KEY_BACK;
    else if (keySelect.wasPressed())
        keyCode = KEY_SELECT;
    else
        keyCode = KEY_NONE;
}

void processKeys(void)
{
    switch (keyCode)
    {
    case KEY_NEXT:
        keyCode = KEY_NONE;
        if (p == 0)
        {
        }
        else if (p == 2)
        {
            if (m == 0)
            {
                if (dayM == 6)
                {
                    dayM = 0;
                }
                else
                    dayM++;
            }
            else if (m == 1)
            {
                if (monthM == 11)
                {
                    monthM = 0;
                }
                else
                    monthM++;
            }
            else if (m == 2)
            {
                yearM++;
            }
            else if (m == 3)
            {
                if (hourM == 23)
                {
                    hourM = 0;
                }
                else
                    hourM++;
            }
            else if (m == 4)
            {
                if (minuteM == 59)
                {
                    minuteM = 0;
                }
                else
                    minuteM++;
            }
        }
        else if (p == 3)
        {
            if (al == 0)
            {
                if (hourA == 23)
                {
                    hourA = 0;
                }
                else
                {
                    hourA++;
                }
            }
            else if (al == 1)
            {
                if (minuteA == 59)
                {
                    minuteA = 0;
                }
                else
                {
                    minuteA++;
                }
            }
            else
            {
                alarma = !alarma;
            }
        }
        else
        {

            menu_current++;
            if (menu_current >= MENU_ITEMS)
                menu_current = 0;
            page_redraw = 1;
        }
        break;
    case KEY_PREV:
        keyCode = KEY_NONE;
        if (p == 0)
        {
        }
        else if (p == 2)
        {
            if (m == 0)
            {
                if (dayM == 0)
                {
                    dayM = 6;
                }
                else
                    dayM--;
            }
            else if (m == 1)
            {
                if (monthM == 0)
                {
                    monthM = 11;
                }
                else
                    monthM--;
            }
            else if (m == 2)
            {
                yearM--;
            }
            else if (m == 3)
            {
                if (hourM == 0)
                {
                    hourM = 23;
                }
                else
                    hourM--;
            }
            else if (m == 4)
            {
                if (minuteM == 0)
                {
                    minuteM = 59;
                }
                else
                    minuteM--;
            }
        }
        else if (p == 3)
        {
            if (al == 0)
            {
                if (hourA == 0)
                {
                    hourA = 23;
                }
                else
                {
                    hourA--;
                }
            }
            else if (al == 1)
            {
                if (minuteA == 0)
                {
                    minuteA = 59;
                }
                else
                {
                    minuteA--;
                }
            }
            else
            {
                alarma = !alarma;
            }
        }
        else
        {

            if (menu_current == 0)
                menu_current = MENU_ITEMS;
            menu_current--;
            page_redraw = 1;
        }
        break;
    case KEY_SELECT:
        keyCode = KEY_NONE;
        if (p == 0)
        {
            p = 1;
        }
        else if (p == 4)
        {
            //p = 1;
        }
        else if (p == 2)
        {
            if (m == 4)
            {
                year = yearM;
                month = monthM;
                day = dayM;
                hour = hourM;
                minute = minuteM;
                m = 0;
                p = 0;
            }
            else
                m++;
        }
        else if (p == 3)
        {
            if (al == 2)
            {
                al = 0;
                p = 0;
            }
            else
            {
                al++;
            }
        }
        else
        {
            p = menu_current + 2;
            if (p == 2)
            {
                yearM = year;
                monthM = month;
                dayM = day;
                hourM = hour;
                minuteM = minute;
            }
            page_redraw = 1;
        }
        break;
    case KEY_BACK:
        keyCode = KEY_NONE;
        if (p == 1)
        {
            p = 0;
        }
        else if (p == 4)
        {
            p = 0;
        }
        else if (p == 2 || p == 3)
        {
            p = 1;
        }

        page_redraw = 1;
        break;
    }
}

//---------------------------------------------------------------------------------------------------
//----------------drawPages--------------------------------------------------------------------------
void pageMenu(void)
{
    u8g.setFont(u8g_font_unifont);
    uint8_t i, h;
    u8g_uint_t w, d;
    u8g.setFontRefHeightText();
    u8g.setFontPosTop();

    h = u8g.getFontAscent() - u8g.getFontDescent();
    w = u8g.getWidth();
    for (i = 0; i < MENU_ITEMS; i++)
    {
        d = (w - u8g.getStrWidth(menu_strings[i])) / 2;
        u8g.setDefaultForegroundColor();
        if (i == menu_current)
        {
            u8g.drawBox(0, i * h + 1, w, h);
            u8g.setDefaultBackgroundColor();
        }
        u8g.drawStr(d, i * h, menu_strings[i]);
    }
}

void draw()
{
    u8g.setRot90();
    u8g.setFont(u8g_font_freedoomr25n);
    if ((hour / 10) == 0)
    {
    }
    else
    {
        u8g.setPrintPos(0, 62);
        u8g.print(hour / 10);
    }
    u8g.setPrintPos(20, 62);
    u8g.print(hour % 10);
    if ((minute / 10) == 0)
    {
    }
    else
    {
        u8g.setPrintPos(0, 95);
        u8g.print(minute / 10);
    }
    u8g.setPrintPos(20, 95);
    u8g.print(minute % 10);
    if ((second / 10) == 0)
    {
    }
    else
    {
        u8g.setPrintPos(0, 128);
        u8g.print(second / 10);
    }
    u8g.setPrintPos(20, 128);
    u8g.print(second % 10);

    // u8g.undoRotation();
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 15);
    u8g.print(menu_days[day]);
    u8g.setPrintPos(0, 30);
    u8g.print(month);
    u8g.drawStr(17, 30, "/");
    u8g.setPrintPos(30, 30);
    u8g.print(year);
    if (alarma)
        u8g.drawBitmapP(40, 102, 3, 24, campana);
}

void SetTime()
{
    u8g.setRot90();
    u8g.setFont(u8g_font_freedoomr25n);
    if ((hourM / 10) == 0)
    {
    }
    else
    {
        u8g.setPrintPos(0, 62);
        u8g.print(hourM / 10);
    }
    u8g.setPrintPos(20, 62);
    u8g.print(hourM % 10);
    if ((minuteM / 10) == 0)
    {
    }
    else
    {
        u8g.setPrintPos(0, 95);
        u8g.print(minuteM / 10);
    }
    u8g.setPrintPos(20, 95);
    u8g.print(minuteM % 10);
    // u8g.undoRotation();
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 15);
    u8g.print(menu_days[dayM]);
    u8g.setPrintPos(0, 30);
    u8g.print(monthM);
    u8g.drawStr(17, 30, "/");
    u8g.setPrintPos(30, 30);
    u8g.print(yearM);
}
void SetAlarm()
{
    u8g.setFont(u8g_font_freedoomr25n);
    if ((hourA / 10) == 0)
    {
    }
    else
    {
        u8g.setPrintPos(0, 62);
        u8g.print(hourA / 10);
    }
    u8g.setPrintPos(20, 62);
    u8g.print(hourA % 10);
    if ((minuteA / 10) == 0)
    {
    }
    else
    {
        u8g.setPrintPos(0, 95);
        u8g.print(minuteA / 10);
    }
    u8g.setPrintPos(20, 95);
    u8g.print(minuteA % 10);
    if (alarma)
        u8g.drawBitmapP(40, 102, 3, 24, campana);

    // u8g.undoRotation();
}
void pageInfo()
{

    u8g.drawBitmapP(20, 80, 3, 24, campana);
}
void TimingISR()
{
    Update = ON;
    halfsecond++;
    if (halfsecond == 2)
    {
        second++;
        if (second == 60)
        {
            minute++;

            if (minute == 60)
            {
                hour++;
                if (hour == 24)
                {
                    day++;
                    if (day == 7)
                    {
                        month++;
                        if (month == 12)
                        {
                            year++;
                            month = 1;
                        }
                        day = 0;
                    }
                    hour = 0;
                }
                minute = 0;
            }
            second = 0;
        }
        halfsecond = 0;
    }
    if (minute == minuteA && hour == hourA && second == secondA)
    {
        p = 4;
    }
}
//------------------------------------------------------------------------------------------------
