/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app_platform.h"
#if PL_CONFIG_USE_PICO_W
  #include "pico/cyw43_arch.h"
#endif
#include "application.h"
#include "McuRTOS.h"
#if PL_CONFIG_USE_RTT
  #include "McuRTT.h"
#endif
#include "hardware/gpio.h"
#include "McuLED.h"
#include "McuLog.h"
#include "McuWait.h"
#include "McuUtility.h"
#include "McuArmTools.h"
#include "McuTimeDate.h"
#if PL_CONFIG_USE_PCF85063A
  #include "McuPCF85063A.h"
#endif
#if PL_CONFIG_USE_BUTTONS
  #include "buttons.h"
  #include "debounce.h"
#endif
#if PL_CONFIG_HAS_LITTLE_FS
  #include "McuLittleFS.h"
#endif
#if PL_CONFIG_USE_USB_CDC
  /* needed for USB CDC: */
  #include <stdio.h>
  #include "pico/stdlib.h"
#endif
#if PL_CONFIG_IS_APP_LED_COUNTER
  #include "neoCounter.h"
#endif
#if PL_CONFIG_USE_ROAD
  #include "road.h"
#endif
#if PL_CONFIG_USE_GUI_KEY_NAV
  #include "lv.h"
#endif
#if PL_CONFIG_USE_POWER
  #include "power.h"
#endif
#if PL_CONFIG_USE_SHT31
  #include "McuSHT31.h"
#endif
#if PL_CONFIG_USE_SHT40
  #include "McuSHT40.h"
#elif PL_CONFIG_USE_SHT31
  #include "McuSHT31.h"
#endif
#if PL_CONFIG_USE_GUI
  #include "gui.h"
#endif
#if PL_CONFIG_USE_LIGHTS
  #include "lights.h"
#endif
#if PL_CONFIG_USE_ADC
  #include "analog.h"
#endif

#if !PL_CONFIG_USE_PICO_W
  #define LED_PIN   (25) /* GPIO 25 */
#endif

#if PL_CONFIG_USE_BUTTONS
void APP_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds kind) {
  unsigned char buf[32];

  buf[0] = '\0';
  switch(button) {
    case BTN_NAV_UP:      McuUtility_strcat(buf, sizeof(buf), "up"); break;
    case BTN_NAV_LEFT:    McuUtility_strcat(buf, sizeof(buf), "left"); break;
    case BTN_NAV_RIGHT:   McuUtility_strcat(buf, sizeof(buf), "right"); break;
    case BTN_NAV_DOWN:    McuUtility_strcat(buf, sizeof(buf), "down"); break;
    case BTN_NAV_CENTER:  McuUtility_strcat(buf, sizeof(buf), "center"); break;
#if PL_CONFIG_USE_BUTTON_NEXT_PREV
    case BTN_NAV_NEXT:    McuUtility_strcat(buf, sizeof(buf), "next"); break;
    case BTN_NAV_PREV:    McuUtility_strcat(buf, sizeof(buf), "prev"); break;
#endif
    default:              McuUtility_strcat(buf, sizeof(buf), "???"); break;
  }
  switch (kind) {
    case MCUDBNC_EVENT_PRESSED:             McuUtility_strcat(buf, sizeof(buf), " pressed"); break;
    case MCUDBNC_EVENT_PRESSED_REPEAT:      McuUtility_strcat(buf, sizeof(buf), " pressed-repeat"); break;
    case MCUDBNC_EVENT_LONG_PRESSED:        McuUtility_strcat(buf, sizeof(buf), " pressed-long"); break;
    case MCUDBNC_EVENT_LONG_PRESSED_REPEAT: McuUtility_strcat(buf, sizeof(buf), " pressed-long-repeat"); break;
    case MCUDBNC_EVENT_RELEASED:            McuUtility_strcat(buf, sizeof(buf), " released"); break;
    case MCUDBNC_EVENT_LONG_RELEASED:       McuUtility_strcat(buf, sizeof(buf), " long released"); break;
    default:                                McuUtility_strcat(buf, sizeof(buf), "???"); break;
  }
  McuUtility_strcat(buf, sizeof(buf), "\n");
#if 0 && PL_CONFIG_USE_RTT /* debugging only */
  McuRTT_printf(0, buf);
#endif
#if PL_CONFIG_USE_ROAD && !PL_CONFIG_USE_GUI
  if (kind==MCUDBNC_EVENT_RELEASED) {
    Road_SetIsOn(!Road_GetIsOn()); /* use button to start/stop running the car */
  }
#endif
#if PL_CONFIG_IS_APP_LED_COUNTER
  NeoCounter_OnButtonEvent(button, kind);
#endif
#if PL_CONFIG_USE_GUI_KEY_NAV
  uint8_t btn;
  uint16_t eventMask;

  switch(button) {
    case BTN_NAV_UP:      btn = LV_BTN_MASK_UP;  break;
    case BTN_NAV_LEFT:    btn = LV_BTN_MASK_LEFT; break;
    case BTN_NAV_RIGHT:   btn = LV_BTN_MASK_RIGHT; break;
    case BTN_NAV_DOWN:    btn = LV_BTN_MASK_DOWN; break;
    case BTN_NAV_CENTER:  btn = LV_BTN_MASK_CENTER; break;
#if PL_CONFIG_USE_BUTTON_NEXT_PREV
    case BTN_NAV_NEXT:    btn = LV_BTN_MASK_NEXT; break;
    case BTN_NAV_PREV:    btn = LV_BTN_MASK_PREV; break;
#endif
    default:              btn = 0; break;
  }
  switch (kind) {
    case MCUDBNC_EVENT_PRESSED:             eventMask = LV_MASK_PRESSED; break;
    case MCUDBNC_EVENT_PRESSED_REPEAT:      eventMask = LV_MASK_PRESSED;  break;
    case MCUDBNC_EVENT_LONG_PRESSED:        eventMask = LV_MASK_PRESSED;  break;
    case MCUDBNC_EVENT_LONG_PRESSED_REPEAT: eventMask = LV_MASK_PRESSED;  break;
    case MCUDBNC_EVENT_RELEASED:            eventMask = LV_MASK_RELEASED;  break;
    case MCUDBNC_EVENT_LONG_RELEASED:       eventMask = LV_MASK_RELEASED_LONG;  break;
    default:                                eventMask = 0; break;
  }
  LV_ButtonEvent(btn, eventMask);
#endif
}
#endif

#if PL_CONFIG_USE_SHT31 || PL_CONFIG_USE_SHT40
static float App_SensorTemperature, App_SensorHumidity;

uint8_t App_GetSensorValues(float *temperature, float *humidity) {
  *temperature = App_SensorTemperature;
  *humidity = App_SensorHumidity;
  return ERR_OK;
}
#endif

static void AppTask(void *pv) {
#define APP_HAS_ONBOARD_GREEN_LED   !PL_CONFIG_USE_PICO_W && !(PL_CONFIG_HW_ACTIVE_HW_VERSION==PL_CONFIG_HW_VERSION_0_5 || PL_CONFIG_HW_ACTIVE_HW_VERSION==PL_CONFIG_HW_VERSION_0_7)
  uint8_t prevBatteryCharge=200, currBatteryCharge;
  uint8_t prevUSBConnectionStatus = -1, currUSBConnectionStatus;
#if PL_CONFIG_USE_OLED_CLOCK && PL_CONFIG_USE_PCF85063A
  #define RTC_UPDATE_PERIOD_SEC  (60*60)
  int32_t RTCupdateCntrSec = 0;
#endif
#if PL_CONFIG_USE_SHT31 || PL_CONFIG_USE_SHT40
  #define SENSOR_UPDATE_PERIOD_SEC  (5)
  int32_t sensorUpdateCntrSec = 0;
#endif

#if APP_HAS_ONBOARD_GREEN_LED
  /* only for pico boards which have an on-board green LED */
  McuLED_Config_t config;
  McuLED_Handle_t led;

  McuLED_GetDefaultConfig(&config);
  config.hw.pin = LED_PIN;
  config.isLowActive = false;
  led = McuLED_InitLed(&config);
  if (led==NULL) {
    McuLog_fatal("failed initializing LED");
    for(;;){}
  }
#endif
#if PL_CONFIG_HAS_LITTLE_FS
  McuLog_info("Mounting litteFS volume.");
  if(McuLFS_Mount(McuShell_GetStdio())==ERR_FAILED){
    McuLog_info("Mounting failed please format device first");
  }
#endif
#if PL_CONFIG_USE_PCF85063A && PL_CONFIG_HW_ACTIVE_HW_VERSION==PL_CONFIG_HW_VERSION_0_5
  /* on this board, the RTC PCF85063A has the CLKOE put to VCC3, which enables CLKOUT signal. Later boards will have it pulled low */
  if (McuPCF85063A_WriteClockOutputFrequency(McuPCF85063A_COF_FREQ_OFF)!=ERR_OK) {
    McuLog_fatal("failed writing COF");
  }
#endif
  vTaskDelay(pdMS_TO_TICKS(1500));
#if PL_CONFIG_USE_SHT31 || PL_CONFIG_USE_SHT40
  #if POWER_CONFIG_USE_EN_VCC2
    if (!Power_GetVcc2IsOn()) {
      McuLog_info("turn on Vcc2 for sensor");
      Power_SetVcc2IsOn(true);
      vTaskDelay(pdMS_TO_TICKS(500)); /* give hardware time to power up: otherwise sensors or OLED cannot be properly initialized */
    }
  #endif
  #if PL_CONFIG_USE_SHT31
    McuSHT31_Init();
  #elif PL_CONFIG_USE_SHT40
    McuSHT40_Init();
  #endif
#endif
  for(;;) {
  #if APP_HAS_ONBOARD_GREEN_LED
    McuLED_Toggle(led);
  #endif
  #if PL_CONFIG_USE_ADC && ANALOG_CONFIG_HAS_ADC_BAT
    currBatteryCharge = Power_GetBatteryChargeLevel();
    if (currBatteryCharge<POWER_BATTERY_LEVEL_TURN_OFF) {
      McuLog_fatal("charge level below %d%%, turning off system", POWER_BATTERY_LEVEL_TURN_OFF);
      vTaskSuspendAll();
      Road_SetIsOn(false); /* turn off running car */
    #if POWER_CONFIG_USE_EN_VCC2
      Power_SetVcc2IsOn(false); /* turn off power VCC2 for OLED and SHT sensor */
    #endif
      /* \todo: put everything into sleep mode */
      Power_SetEnPwrIsOn(false); /* turn off power, only effective if USB is *not* connected */
      for(;;) {
        McuArmTools_SoftwareReset(); /* perform a new start */
      }
    }
    if (prevBatteryCharge!=currBatteryCharge) {
      GUI_SendEvent(Gui_Event_Battery_Charge_Changed); /* update with new battery charge level */
      prevBatteryCharge = currBatteryCharge;
    }
  #endif
  #if POWER_CONFIG_SENSE_USB
    currUSBConnectionStatus = Power_GetUsbPowerIsOn();
    if (currUSBConnectionStatus!=prevUSBConnectionStatus) {
  #if PL_CONFIG_USE_GUI
      GUI_SendEvent(Gui_Event_USB_Connection_Changed); /* update USB status */
  #endif
      prevUSBConnectionStatus = currUSBConnectionStatus;
    }
  #endif
#if PL_CONFIG_USE_OLED_CLOCK
  #if PL_CONFIG_USE_OLED_CLOCK && PL_CONFIG_USE_PCF85063A
  TIMEREC time;
  DATEREC date;

  if (RTCupdateCntrSec<=0) {
    if (McuPCF85063A_GetTimeDate(&time, &date)==ERR_OK) {
      McuTimeDate_SetTimeDate(&time, &date);
    }
    RTCupdateCntrSec = RTC_UPDATE_PERIOD_SEC;
  }
  RTCupdateCntrSec--;
  GUI_SendEvent(Gui_Event_Clock_Changed);
  #endif
#endif

#if PL_CONFIG_USE_SHT31 || PL_CONFIG_USE_SHT40
   if (sensorUpdateCntrSec<=0) {
     float f, h;

     if (McuSHT40_ReadTempHum(&f, &h)==ERR_OK) {
       App_SensorTemperature = f;
       App_SensorHumidity = h;
     }
     sensorUpdateCntrSec = SENSOR_UPDATE_PERIOD_SEC;
   }
   sensorUpdateCntrSec--;
   GUI_SendEvent(Gui_Event_Sensor_Changed);
#endif
   vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static uint8_t PrintStatus(McuShell_ConstStdIOType *io) {
  unsigned char buf[48];

  McuShell_SendStatusStr((unsigned char*)"app", (const unsigned char*)"Status of application\r\n", io->stdOut);
  McuUtility_Num32uToStr(buf, sizeof(buf), PL_CONFIG_HW_ACTIVE_HW_VERSION);
  McuUtility_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
  McuShell_SendStatusStr((uint8_t*)"  HW", (unsigned char*)buf, io->stdOut);
  return ERR_OK;
}

uint8_t App_ParseCommand(const unsigned char *cmd, bool *handled, const McuShell_StdIOType *io) {
  uint32_t value;
  const unsigned char *p;

  if (McuUtility_strcmp((char*)cmd, McuShell_CMD_HELP)==0 || McuUtility_strcmp((char*)cmd, "app help")==0) {
    McuShell_SendHelpStr((unsigned char*)"app", (const unsigned char*)"Group of application commands\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*)"  help|status", (const unsigned char*)"Print help or status information\r\n", io->stdOut);
    *handled = true;
    return ERR_OK;
  } else if ((McuUtility_strcmp((char*)cmd, McuShell_CMD_STATUS)==0) || (McuUtility_strcmp((char*)cmd, "app status")==0)) {
    *handled = true;
    return PrintStatus(io);
  }
  return ERR_OK;
}

void APP_Run(void) {
  PL_Init();
#if PL_CONFIG_USE_POWER /* check battery level */
  Power_WaitForSufficientBatteryChargeAtStartup();
#endif
  if (xTaskCreate(
      AppTask,  /* pointer to the task */
      "App", /* task name for kernel awareness debugging */
      1000/sizeof(StackType_t), /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY+2,  /* initial priority */
      (TaskHandle_t*)NULL /* optional task handle to create */
    ) != pdPASS)
  {
    for(;;){} /* error! probably out of memory */
  }
  vTaskStartScheduler();
  for(;;) {
    /* shall not get here */
  }
}
