/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCUSEMIHOST_H_
#define MCUSEMIHOST_H_

#include "McuSemihost_config.h"

#include <stdbool.h>
#include <stdint.h>
#include "McuTimeDate.h"
#include "McuShell.h"

extern uint8_t McuSemiHost_DefaultShellBuffer[McuShell_DEFAULT_SHELL_BUFFER_SIZE];
  /*!< default buffer which can be used by the application or shell */

extern McuShell_ConstStdIOType McuSemiHost_stdio;
  /*!< Default standard I/O handler, can be used for a shell intergration */

/*!
 * \brief Return the SWO stdio handle
 * \return Standard I/O handle
 */
McuShell_ConstStdIOTypePtr McuSemiHost_GetStdio(void);

/*!
 * \brief Return the current system time
 * \return System time in seconds since 1970
 */
int McuSemihost_HostTime(void);

/*!
 * \brief Return the number of centi-seconds the executable is running
 * \return -1 for error, otherwise the number of centi-seconds of the execution
 */
int McuSemihost_HostClock(void);

/*!
 * \brief Open a file on the host
 * \param filename
 * \param mode
 * \param fileNameLenght
 * \return -1 if failed, otherwise file handle
 */
int McuSemihost_FileOpen(const unsigned char *filename, int mode);

/*!
 * \brief Closes a file handle
 * \param fh File handle previously opened
 * \return 0: ok, otherwise -1 if failed
 */
int McuSemihost_FileClose(int fh);

/*!
 * \brief Read from a file
 * \param fh File handle
 * \param data Pointer where to store the data
 * \param nofBytes Number of bytes to read
 * \return 0: success. If it is nofBytes, then the call has failed and the end of the file has been reached. If smaller than nofBytes, then the buffer has not been filled.
 */
int McuSemihost_FileRead(int fh, unsigned char *data, size_t nofBytes);

/*!
 * \brief Write data to a file
 * \param fh File handle
 * \param data Pointer to data
 * \param nofBytes Number of data bytes to write
 * \return 0 for success, in error case the number of bytes not written
 */
int McuSemihost_FileWrite(int fh, const unsigned char *data, size_t nofBytes);

/*!
 * \brief Return the length of a file
 * \param fh File handle
 * \return Current length of the file, -1 for an error
 */
int McuSemihost_FileLen(int fh);

/*!
 * \brief Seeks for a specified position in a file
 * \param fh File handle
 * \param pos Target position. Seeking outside of the size of the file is undefined
 * \return 0 for success, negative for an error. McuSemihost_Op_SYS_ERRNO can be used to read the error value.
 */
int McuSemihost_FileSeek(int fh, int pos);

/*!
 * \brief Returns a temporary name for a file identified by a system file identifier.
 * \param fileID target identifier for the file name. Must be in the range 0..255
 * \param buffer Pointer where to store the file name
 * \param bufSize Buffer size in bytes
 * \return 0 for success, -1 for an error
 */
int McuSemihost_TmpName(uint8_t fileID, unsigned char *buffer, size_t bufSize);

/*!
 * \brief Read a character from the console or stdin
 * \return The character read
 */
int McuSemihost_ReadChar(void);

/*!
 * \brief Write a character to the stdout console.
 * \param ch Character to write
 * \return always zero for success
 */
int McuSemihost_WriteChar(char ch);

/*!
 * \brief Decides if a file handle is a standard io handle or not.
 * \param fg File handle
 * \return 1 if it a interactive device, 0 if not, any other value is an error
 */
int McuSemihost_IsTTY(int fh);

/*!
 * \brief Write a zero byte terminated character array (string) to stdout
 * \param str String, zero byte terminated
 * \return 0: ok, -1 error
 */
int McuSemihost_WriteString(const unsigned char *str);

/*!
 * \brief Sending a character to the SWO/ITM console
 * \param ch Character to send
 */
void McuSemiHost_StdIOSendChar(uint8_t ch);

/*!
 * \brief Reads a character from the standard input
 * \param ch Pointer where to store the character, stores '\0' if no character was received
 */
void McuSemiHost_StdIOReadChar(uint8_t *ch);

/*!
 * \brief Checks if there is input from SWO/ITM console
 * \return true if there is input, false otherwise
 */
bool McuSemiHost_StdIOKeyPressed(void);

/*!
 * \brief 'printf'-style writing with semihosting
 * \param fmt Format string
 * \return Number of characters written
 */
unsigned McuSemihost_printf(const char *fmt, ...);

/*!
 * \brief Get the date and time from the host
 * \param time Pointer to where to store the time information
 * \param date Pointer to where to store the date information
 * \param offsetHour Pass -1 if during wintertime
 * \return ERR_OK if ok, error code otherwise
 */
uint8_t McuSemihost_GetTimeDateFromHost(TIMEREC *time, DATEREC *date, int offsetHour);

/*!
 * \brief Testing the semihost functionality and API
 * \return 0 for success, -1 for error
 */
int McuSemiHost_Test(void);

/*!
 * \brief Module de-initialization
 */
void McuSemiHost_Deinit(void);

/*!
 * \brief Module initialization
 */
void McuSemiHost_Init(void);

#endif /* MCUSEMIHOST_H_ */
