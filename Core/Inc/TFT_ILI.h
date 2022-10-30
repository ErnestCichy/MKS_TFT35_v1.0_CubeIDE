/*
 * TFT_ILI.h
 *
 *  Created on: Oct 21, 2022
 *      Author: DOM
 */

#ifndef INC_TFT_ILI_H_
#define INC_TFT_ILI_H_


#define ILI_ROTATION 3 		// 0 - 0*, 1 - 90*, 2 - 180*, 3 - 270*

#define ILI_USE_SD_CARD		1
#if (ILI_USE_SD_CARD == 1)
#define ILI_USE_JPEG		1

#define SD_BUFFER_SIZE 			2048

#include "fatfs.h"
#endif


//
// Registers
//
#if (ILI_ROTATION == 0) || (ILI_ROTATION == 2)
#define ILI_WIDTH 	320  	///< ILI max TFT width
#define ILI_HEIGHT 	480 	///< ILI max TFT height
#elif (ILI_ROTATION == 1) || (ILI_ROTATION == 3)
#define ILI_WIDTH 	480  	///< ILI max TFT width
#define ILI_HEIGHT 	320 	///< ILI max TFT height
#endif

//ROTATION COMMANDS
#define ILI_MADCTL_MY  			0x80	///< Bottom to top
#define ILI_MADCTL_MX  			0x40	///< Right to left
#define ILI_MADCTL_MV  			0x20	///< Reverse Mode
#define ILI_MADCTL_ML  			0x10	///< LCD refresh Bottom to top
#define ILI_MADCTL_RGB 			0x00	///< Red-Green-Blue pixel order
#define ILI_MADCTL_BGR 			0x08	///< Blue-Green-Red pixel order
#define ILI_MADCTL_MH  			0x04	///< LCD refresh right to left

//--------------------------------------------------------------

#define ILI_NOP 			    0x00    ///< No-op register
#define ILI_RESET 			    0x01 	///< Software reset register
#define ILI_RDDID 			    0x04   	///< Read display identification information
#define ILI_RDDST 			    0x09   	///< Read Display Status

#define ILI_SLEEP_IN 		    0x10  	///< Enter Sleep Mode
#define ILI_SLEEP_OUT 		    0x11 	///< Sleep Out
#define ILI_PTLON 			    0x12  	///< Partial Mode ON
#define ILI_NORON 			    0x13  	///< Normal Display Mode ON

#define ILI_RDMODE 			    0x0A    ///< Read Display Power Mode
#define ILI_RDMADCTL 		    0x0B   	///< Read Display MADCTL
#define ILI_RDPIXFMT 		    0x0C   	///< Read Display Pixel Format
#define ILI_RDIMGFMT 		    0x0D   	///< Read Display Image Format
#define ILI_RDSELFDIAG 		    0x0F 	///< Read Display Self-Diagnostic Result

#define ILI_INVOFF 			    0x20   	///< Display Inversion OFF
#define ILI_INVON 			    0x21    ///< Display Inversion ON
#define ILI_GAMMA	 		    0x26 	///< Gamma Set
#define ILI_DISPLAY_OFF	    	0x28  	///< Display OFF
#define ILI_DISPLAY_ON	    	0x29   	///< Display ON

#define ILI_COLUMN_ADDR		    0x2A 	///< Column Address Set
#define ILI_PAGE_ADDR		    0x2B 	///< Page Address Set
#define ILI_RAMWR 			    0x2C 	///< Memory Write
#define ILI_RAMRD 			    0x2E 	///< Memory Read

#define ILI_PTLAR 			    0x30    ///< Partial Area
#define ILI_VSCRDEF 		    0x33  	///< Vertical Scrolling Definition
#define ILI_TEARING_OFF			0x34	///< Tearing Effect Line OFF
#define ILI_TEARING_ON			0x35	///< Tearing Effect Line ON
#define ILI_MADCTL		  		0x36   	///< Memory Access Control
#define ILI_VSCRSADD 		    0x37 	///< Vertical Scrolling Start Address
#define ILI_PIXFMT 			    0x3A   	///< COLMOD: Pixel Format Set

#define ILI_FRMCTR1    		    0xB1 	///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI_FRMCTR2 		    0xB2 	///< Frame Rate Control (In Idle Mode/8 colors)
#define ILI_FRMCTR3     	    0xB3 	///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI_INVCTR 			    0xB4  	///< Display Inversion Control
#define ILI_DFUNCTR 		    0xB6 	///< Display Function Control

#define ILI_PWCTR1 			    0xC0 	///< Power Control 1
#define ILI_PWCTR2 			    0xC1 	///< Power Control 2
#define ILI_PWCTR3 			    0xC2 	///< Power Control 3
#define ILI_PWCTR4 			    0xC3 	///< Power Control 4
#define ILI_PWCTR5 			    0xC4 	///< Power Control 5
#define ILI_VMCTR1 			    0xC5 	///< VCOM Control 1
#define ILI_VMCTR2 			    0xC7 	///< VCOM Control 2

#define ILI_RDID1 			    0xDA 	///< Read ID 1
#define ILI_RDID2 			    0xDB 	///< Read ID 2
#define ILI_RDID3 			    0xDC 	///< Read ID 3
#define ILI_RDID4 			    0xDD 	///< Read ID 4

#define ILI_GMCTRP1 		    0xE0 	///< Positive Gamma Correction
#define ILI_GMCTRN1 		    0xE1 	///< Negative Gamma Correction

// Color definitions
#define ILI_BLACK 			    0x0000   ///<   0,   0,   0
#define ILI_NAVY 			    0x000F   ///<   0,   0, 123
#define ILI_DARKGREEN 	        0x03E0   ///<   0, 125,   0
#define ILI_DARKCYAN 	        0x03EF   ///<   0, 125, 123
#define ILI_MAROON 		        0x7800   ///< 123,   0,   0
#define ILI_PURPLE 		        0x780F   ///< 123,   0, 123
#define ILI_OLIVE 		        0x7BE0   ///< 123, 125,   0
#define ILI_LIGHTGREY 	        0xC618   ///< 198, 195, 198
#define ILI_DARKGREY 	        0x7BEF   ///< 123, 125, 123
#define ILI_BLUE 		        0x001F   ///<   0,   0, 255
#define ILI_GREEN 		        0x07E0   ///<   0, 255,   0
#define ILI_CYAN 		        0x07FF   ///<   0, 255, 255
#define ILI_RED 		        0xF800   ///< 255,   0,   0
#define ILI_MAGENTA 	        0xF81F   ///< 255,   0, 255
#define ILI_YELLOW 		        0xFFE0   ///< 255, 255,   0
#define ILI_WHITE 		        0xFFFF   ///< 255, 255, 255
#define ILI_ORANGE 		        0xFD20   ///< 255, 165,   0
#define ILI_GREENYELLOW         0xAFE5 	 ///< 173, 255,  41
#define ILI_PINK 		        0xFC18   ///< 255, 130, 198

void ILI_Init(void);
void ILI_SetRotation(uint16_t Rotation);
void ILI_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);
void ILI_WritePixel(int16_t x, int16_t y, uint16_t color);
void ILI_ClearDisplay(uint16_t Color);
void ILI_DrawImage(uint16_t x, uint16_t y, const uint16_t* img, uint16_t w, uint16_t h);
void ILI_Fill(uint16_t color);

uint8_t ILI_DrawImageFromSD(uint16_t x, uint16_t y, const TCHAR* path, uint16_t w, uint16_t h);
void ConvertRGB888toRGB565(uint16_t *OutputColor, uint32_t sourceColor);
uint8_t ILI_DrawJPEG(uint16_t x, uint16_t y,  const TCHAR* path);



#if (ILI_USE_SD_CARD == 1)
uint8_t ILI9341_DrawImageFromSD(uint16_t x, uint16_t y, const TCHAR* path, uint16_t w, uint16_t h);
#if (ILI_USE_JPEG == 1)
uint8_t ILI_DrawJPEG(uint16_t x, uint16_t y,  const TCHAR* path);
#endif

#endif // ILI_USE_SD_CARD

#endif /* INC_TFT_ILI_H_ */
