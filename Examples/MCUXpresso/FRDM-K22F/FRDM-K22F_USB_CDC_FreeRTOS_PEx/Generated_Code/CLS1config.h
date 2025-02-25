/**
 * \file
 * \brief Configuration header file for Shell
 * Copyright (c) 2020, Erich Styger
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * This header file is used to configure settings of the Shell module.
 */

#ifndef __CLS1_CONFIG_H
#define __CLS1_CONFIG_H

#ifndef CLS1_CONFIG_BLOCKING_SEND_ENABLED
  #define CLS1_CONFIG_BLOCKING_SEND_ENABLED                (1)
    /*!< 1: Sending is blocking (with an optional timeout); 0: Do not block on sending */
#endif

#ifndef CLS1_CONFIG_BLOCKING_SEND_TIMEOUT_MS
  #define CLS1_CONFIG_BLOCKING_SEND_TIMEOUT_MS             (20)
    /*!< Total blocking time (timeout) in milliseconds, uses 0 for blocking without a timeout */
#endif

#ifndef CLS1_CONFIG_BLOCKING_SEND_TIMEOUT_WAIT_MS
  #define CLS1_CONFIG_BLOCKING_SEND_TIMEOUT_WAIT_MS        (5)
    /*!< waiting time during blocking, use 0 (zero) for polling */
#endif

#ifndef CLS1_CONFIG_BLOCKING_SEND_RTOS_WAIT
  #define CLS1_CONFIG_BLOCKING_SEND_RTOS_WAIT              (1)
    /*!< 1: Use WaitmsOS() instead of Waitms(); 0: Use Waitms() instead of WaitOSms() */
#endif

#ifndef CLS1_CONFIG_USE_MUTEX
  #define CLS1_CONFIG_USE_MUTEX                            (0)
    /*!< 1: use RTOS mutex; 0: do not use RTOS mutex */
#endif

#ifndef CLS1_CONFIG_DEFAULT_SHELL_BUFFER_SIZE
  #define CLS1_CONFIG_DEFAULT_SHELL_BUFFER_SIZE            (48)
    /*!< default buffer size for shell command parsing */
#endif

#ifndef CLS1_CONFIG_DEFAULT_SERIAL
  #define CLS1_CONFIG_DEFAULT_SERIAL                       (1)
    /*!< 1: the shell implements its own StdIO which is returned by GetStdio(); 0: The shell does not implement its own standard I/O */
#endif

#if CLS1_CONFIG_DEFAULT_SERIAL
  #ifndef CLS1_CONFIG_DEFAULT_SERIAL_INCLUDE
    #define CLS1_CONFIG_DEFAULT_SERIAL_INCLUDE             "USB1.h"
      /*!< Include for the functions below */
  #endif

  #ifndef CLS1_CONFIG_DEFAULT_SERIAL_RECEIVE_FCT_NAME
    #define CLS1_CONFIG_DEFAULT_SERIAL_RECEIVE_FCT_NAME    USB1_RecvChar
      /*!< Function name to read a character and returning ERR_OK if it was successful */
  #endif

  #ifndef CLS1_CONFIG_DEFAULT_SERIAL_SEND_FCT_NAME
    #define CLS1_CONFIG_DEFAULT_SERIAL_SEND_FCT_NAME       USB1_SendChar
      /*!< Function name to send a character and returning ERR_OK if it was successful */
  #endif

  #ifndef CLS1_CONFIG_DEFAULT_SERIAL_RXAVAIL_FCT_NAME
    #define CLS1_CONFIG_DEFAULT_SERIAL_RXAVAIL_FCT_NAME    USB1_GetCharsInRxBuf
    /*!< Function name to check if there is anything available to receive and returns TRUE, otherwise FALSE */
  #endif
#endif

#ifndef CLS1_CONFIG_PROMPT_STRING
  #define CLS1_CONFIG_PROMPT_STRING                        "CMD> "
#endif

#ifndef CLS1_CONFIG_PROJECT_NAME_STRING
  #define CLS1_CONFIG_PROJECT_NAME_STRING                  "USB CDC on K22F"
#endif

#ifndef CLS1_CONFIG_MULTI_CMD_ENABLED
  #define CLS1_CONFIG_MULTI_CMD_ENABLED                    (0) /* 1: enabled, 0: disabled */
#endif

#ifndef CLS1_CONFIG_MULTI_CMD_SIZE
  #define CLS1_CONFIG_MULTI_CMD_SIZE                       (32) /* max size of each command */
#endif

#ifndef CLS1_CONFIG_MULTI_CMD_CHAR
  #define CLS1_CONFIG_MULTI_CMD_CHAR                       ';' /* separation character */
#endif

#ifndef CLS1_CONFIG_HISTORY_ENABLED
  #define CLS1_CONFIG_HISTORY_ENABLED                      (0)
    /*!< 1: history enabled with <cursor-up> and <cursor-down>; 0: no history functionality */
#endif

#ifndef CLS1_CONFIG_HISTORY_NOF_ITEMS
  #define CLS1_CONFIG_HISTORY_NOF_ITEMS                    (5)
    /*!< number of items in the history */
#endif

#ifndef CLS1_CONFIG_HISTORY_ITEM_LENGTH
  #define CLS1_CONFIG_HISTORY_ITEM_LENGTH                  (32)
    /*!< length of a history item */
#endif

#ifndef CLS1_CONFIG_HISTORY_CHAR_PREV
  #define CLS1_CONFIG_HISTORY_CHAR_PREV                    '\e'
    /*!< character to go to previous item in history */
#endif

#ifndef CLS1_CONFIG_HISTORY_CHAR_NEXT
  #define CLS1_CONFIG_HISTORY_CHAR_NEXT                    '\t'
    /*!< character to go to next item in history */
#endif

#ifndef CLS1_CONFIG_ECHO_ENABLED
  #define CLS1_CONFIG_ECHO_ENABLED                         (0)
    /*!< 1: shell implements local echo; 0: no echo functionality
     Note that echo needs to be enabled in each I/O too (io->echoEnabled) */
#endif

#ifndef CLS1_CONFIG_STATUS_COLON_POS
  #define CLS1_CONFIG_STATUS_COLON_POS                     (13)
   /*!< position of the ':' after the item string for the 'status' command */
#endif

#ifndef CLS1_CONFIG_HELP_SEMICOLON_POS
  #define CLS1_CONFIG_HELP_SEMICOLON_POS                   (26)
 /*!< position of the ';' after the command string for the 'help' command */
#endif

#ifndef CLS1_CONFIG_SILENT_PREFIX_CHAR
  #define CLS1_CONFIG_SILENT_PREFIX_CHAR                   '#'
    /*!< With this char as first character in the cmd, printing is silent. Use a space to disable it */
#endif

#endif /* __CLS1_CONFIG_H */
