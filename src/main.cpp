#include "pt/pt_display.h"
#include "pt_demo.h"

void setup()
{
  Serial.begin(115200);
  pt_setup_display();
  pt_demo_create_brightness_demo();
}

void loop()
{
  pt_loop_display();
}