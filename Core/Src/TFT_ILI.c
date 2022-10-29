/*
 * TFT_ILI.c
 *
 *  Created on: Oct 21, 2022
 *      Author: DOM
 */


#include "main.h"
#include "TFT_ILI.h"


//----------------SD & JPG--------
#if (ILI_USE_SD_CARD == 1)
#if (ILI_USE_JPEG == 1)

#include "jdata_conf.h"
#include "jpeglib.h"
struct jpeg_decompress_struct cinfo;	/* This struct contains the JPEG decompression parameters */
struct jpeg_error_mgr jerr;				/* This struct represents a JPEG error handler */
uint16_t _aucLine[ILI_WIDTH*3];

typedef struct RGB					//tu jest struktura w jaki sposób są poukładane dane w tablicy kolorów
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
}RGB_typedef;

RGB_typedef *RGB_matrix;			//wskaźnik na tablicę kolorów pliku jpg
uint16_t BUFOR16[500];



#endif
#endif

//------------function static use------------------------------

static void ILI_Delay		(uint32_t ms) 		{ HAL_Delay(ms); }
static void ILI_SendCommand	(uint16_t Command) 	{*(__IO uint16_t *)(0x60000000) = Command;}
static void ILI_SendData	(uint16_t Data) 	{*(__IO uint16_t*)(0x60040000) = Data;}
//static void ILI_SendData16	(uint16_t Data) 	{*(__IO uint16_t*)(0x60040000) = Data;}

//##########################################################################################
static void ILI_SendToTFT	(uint16_t *Byte, uint32_t Length)
{
    while (Length > 0U)
    {
    	*(__IO uint16_t*)(0x60040000) = (*Byte);
        Byte++;
        Length--;
    }
}

//##########################################################################################
static void ILI_SendCommandAndData(uint16_t Command, uint16_t *Data, uint32_t Length)
{
	ILI_SendCommand(Command);
	ILI_SendToTFT(Data, Length);
}
//static void ILI_SendCommandAndData(uint8_t Command, uint8_t *Data, uint32_t Length)
//{
//	ILI_SendToTFT(&Command, 1);
//	ILI_SendToTFT(Data, Length);
//}

//##########################################################################################
void ILI_SetRotation(uint16_t Rotation)
{
	if(Rotation > 3)
		return;

	switch(Rotation)	// Set appropriate bits for Rotation
	{
	case 0:
		Rotation = (ILI_MADCTL_MX | ILI_MADCTL_BGR);
		break;
	case 1:
		Rotation = (ILI_MADCTL_MV | ILI_MADCTL_BGR);
		break;
	case 2:
		Rotation = (ILI_MADCTL_MY | ILI_MADCTL_BGR);
		break;
	case 3:
		Rotation = (ILI_MADCTL_MX | ILI_MADCTL_MY | ILI_MADCTL_MV | ILI_MADCTL_BGR);
		break;
	}
	ILI_SendCommandAndData(ILI_MADCTL, &Rotation, 1);  // Write into MAD Control register our Rotation data
}

//##########################################################################################
void ILI_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
	uint16_t DataToTransfer[4];
	// Calculate end ranges
	uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);

	// Fulfill X's buffer
	DataToTransfer[0] = x1 >> 8;
	DataToTransfer[1] = x1 & 0xFF;
	DataToTransfer[2] = x2 >> 8;
	DataToTransfer[3] = x2 & 0xFF;
	// Push X's buffer
	ILI_SendCommandAndData(ILI_COLUMN_ADDR, DataToTransfer, 4);

	// Fulfill Y's buffer
	DataToTransfer[0] = y1 >> 8;
	DataToTransfer[1] = y1 & 0xFF;
	DataToTransfer[2] = y2 >> 8;
	DataToTransfer[3] = y2 & 0xFF;
	// Push Y's buffer
	ILI_SendCommandAndData(ILI_PAGE_ADDR, DataToTransfer, 4);

}
//static void ILI_SetCursorPos(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
//	  	  	  ILI_SendCommand (ILI_COLUMN_ADDR);
//	  	  	  ILI_SendData	  (x1 >> 8);
//	  	  	  ILI_SendData	  (x1 & 0xFF);
//	  	  	  ILI_SendData    (x2 >> 8);
//	  	  	  ILI_SendData    (x2 & 0xFF);
//	  	  	  ILI_SendCommand (ILI_PAGE_ADDR);
//	  	  	  ILI_SendData    (y1 >> 8);
//	  	  	  ILI_SendData    (y1 & 0xFF);
//	  	  	  ILI_SendData    (y2 >> 8);
//	  	  	  ILI_SendData    (y2 & 0xFF);
//	  	  	  ILI_SendCommand (ILI_RAMWR);		}

//##########################################################################################
void ILI_WritePixel(int16_t x, int16_t y, uint16_t color)
{
	if ((x >= 0) && (x < ILI_WIDTH) && (y >= 0) && (y < ILI_HEIGHT))
	{
		// Set Window for 1x1 pixel
		ILI_SetAddrWindow(x, y, 1, 1);
		// Push color bytes to RAM
		ILI_SendData(color);
	}
}


//##########################################################################################
void ILI_DrawImage(uint16_t x, uint16_t y, const uint16_t* img, uint16_t w, uint16_t h)
{
	// Check if image will fit into screen - cannot make it outside by hardware
	if ((x >= 0) && ((x + w) <= ILI_WIDTH) && (y >= 0) && ((y + h) <= ILI_HEIGHT))
	{
		// Set window for image
		ILI_SetAddrWindow(x, y, w, h);

		// Push image to RAM
		*(__IO uint16_t *)(0x60000000) = ILI_RAMWR;

		// Send image to RAM
		const uint16_t *Ptr = img;
		for(uint32_t i = 0; i < (w * h); i++)
		{
			*(__IO uint16_t*)(0x60040000) = (*Ptr);
			Ptr++;
		}
	}
}
//##########################################################################################

void ILI_ClearDisplay(uint16_t Color)
{
	ILI_SetAddrWindow(0, 0, ILI_WIDTH, ILI_HEIGHT);	// Set window for whole screen
	ILI_SendCommand(ILI_RAMWR);						// Set RAM writing		/////TO MOŻNA BY BYŁO WYWALIĆ DO FUNKCJI SET ADDR WINDOW

	for(uint32_t i = 0; i < (ILI_WIDTH * ILI_HEIGHT); i++)
	{
		*(__IO uint16_t*)(0x60040000) = Color;
	}
}

//##########################################################################################


void ILI_Init(void){
	  	  	  ILI_SendCommand (ILI_RESET); 			// software reset comand
	  	  	  ILI_Delay(100);
	  	  	  ILI_SendCommand (ILI_DISPLAY_OFF); 	// display off
//------------power control------------------------------
	  	  	  ILI_SendCommand (ILI_PWCTR1); 		// power control
	  	  	  ILI_SendData    (0x26); 				// GVDD = 4.75v
	  	  	  ILI_SendCommand (ILI_PWCTR2 ); 		// power control
	  	  	  ILI_SendData    (0x11); 				// AVDD=VCIx2, VGH=VCIx7, VGL=-VCIx3
//--------------VCOM-------------------------------------
	  	  	  ILI_SendCommand (ILI_VMCTR1); 		// vcom control
	  	  	  ILI_SendData    (0x35); 				// Set the VCOMH voltage (0x35 = 4.025v)
	  	  	  ILI_SendData    (0x3e); 				// Set the VCOML voltage (0x3E = -0.950v)
//--------------memory control------------------------
	  	  	  ILI_SendCommand (ILI_MADCTL);			//	memory acces control
	  	  	  ILI_SendData	  (0x62);				//	MY MX MV ML BGR MH X X
	  	  	  ILI_SendCommand (ILI_PIXFMT); 		//	interfejs równoległy 16bit
	  	  	  ILI_SendData    (0x55); 				//	X DPI X DBI
////--------------frame------------------------
//	  	  	  ILI_SendCommand (ILI_COLUMN_ADDR); 	//  KOLUMNY				- definiowanie obszaru pamieci
//	  	  	  ILI_SendData    (0x00); 				// 	pozycja X początek	- górne bity
//	  	  	  ILI_SendData    (0x00); 				// 	pozycja X początek 	- dolne bity
//	  	  	  ILI_SendData    (0x01); 				//  pozycja X koniec 	- górne bity
//	  	  	  ILI_SendData    (0x3F); 				//  pozycja X koniec 	- dolne bity
//	  	  	  ILI_SendCommand (ILI_PAGE_ADDR); 		// 	WIERSZE		 		- definiowanie obszaru pamieci
//	  	  	  ILI_SendData    (0x00); 				// 	pozycja Y początek	- górne bity
//	  	  	  ILI_SendData    (0x00); 				// 	pozycja Y początek 	- dolne bity
//	  	  	  ILI_SendData    (0x01); 				// 	pozycja Y koniec 	- górne bity
//	  	  	  ILI_SendData    (0xDF); 				// 	pozycja Y koniec 	- dolne bity
//-----------------display------------------------
	  	  	  ILI_SendCommand (ILI_DFUNCTR); 		// ustawienia wyświetlacza
	  	  	  ILI_SendData    (0x0a);
	  	  	  ILI_SendData    (0x82);
	  	  	  ILI_SendData    (0x3b);
	  	  	  ILI_SendData    (0x00);
	  	  	  ILI_SendCommand (ILI_SLEEP_OUT);
	  	  	  ILI_SendCommand (ILI_DISPLAY_ON);
	  	  	  ILI_SendCommand (ILI_RAMWR);


	  	  	  ILI_SetRotation (ILI_ROTATION); 		// Set selected Rotation
   }

//##########################################################################################
void ILI_Fill(uint16_t color)
{
	uint32_t n = ILI_HEIGHT*ILI_WIDTH;
	ILI_SetAddrWindow(0, 0, ILI_HEIGHT -1, ILI_WIDTH -1);
	while (n)
	{
			n--;
			ILI_SendData(color);
	}
}

//##########################################################################################
uint8_t ILI_DrawImageFromSD(uint16_t x, uint16_t y, const TCHAR* path, uint16_t w, uint16_t h)
{
	FRESULT ImageFsResult;
	FIL ImageFile;

	uint16_t Data[SD_BUFFER_SIZE];
	UINT Length;

	//
	// Open file on SD for reading
	//
	ImageFsResult = f_open(&ImageFile, path, FA_READ);

	//
	// File open error check
	//
	if(ImageFsResult != FR_OK)
	{
		return 1;
	}
	else
	{
		ILI_SetAddrWindow(x, y, w, h);
		ILI_SendCommand (ILI_RAMWR);
		do
		{
			Length = 0;
			f_read(&ImageFile, Data, SD_BUFFER_SIZE, &Length);

			ILI_SendToTFT(Data, Length/2);			//tu dzielę przez 2 ponieważ wysyłam po 2 bajty jednocześnie

		}while(Length > 0);

		ImageFsResult = f_close(&ImageFile);

		return 0;
	}
}



//
////////////////////
//void ILI_DrawImage(uint16_t x, uint16_t y, const uint16_t* img, uint16_t w, uint16_t h)/////////////////
//{
//	// Check if image will fit into screen - cannot make it outside by hardware
//	if ((x >= 0) && ((x + w) <= ILI_WIDTH) && (y >= 0) && ((y + h) <= ILI_HEIGHT))
//	{
//
//		ILI_SetAddrWindow(x, y, w, h);/////////////////////////////////////////////////////
//
//		// Push image to RAM
//		*(__IO uint16_t *)(0x60000000) = ILI_RAMWR;
//
//		// Send image to RAM
//		const uint16_t *Ptr = img;
//		for(uint32_t i = 0; i < (w * h); i++)
//		{
//			*(__IO uint16_t*)(0x60040000) = (*Ptr);
//			Ptr++;
//		}
//	}
//}
///////////////////


















//##########################################################################################
void ConvertRGB888toRGB565(uint16_t *OutputColor, uint32_t sourceColor)
{
	uint32_t red   = (sourceColor & 0x00F80000) >> 8;
	uint32_t green = (sourceColor & 0x0000FC00) >> 5;
	uint32_t blue  = (sourceColor & 0x000000F8) >> 3;
	OutputColor = (uint16_t*)(red|green|blue);				//######################tu wstawiłem * do outputcoloru ale nie jestem pewny
}

//##########################################################################################
uint8_t ILI_DrawJPEG(uint16_t x, uint16_t y,  const TCHAR* path)
{
	FIL ImageFile;

	if(f_open(&ImageFile, path, FA_READ) == FR_OK)
	{

		HAL_GPIO_WritePin (LD3_GPIO_Port, LD3_Pin, 1);

		/* Decode JPEG Image */
		JSAMPROW buffer[2] = { 0 }; /* Output row buffer */
		//uint32_t row_stride = 0; /* Physical row width in image buffer */

		buffer[0] = (JSAMPROW)&_aucLine;

		/* Step 1: Allocate and initialize JPEG decompression object */
		cinfo.err = jpeg_std_error(&jerr);

		/* Step 2: Initialize the JPEG decompression object */
		jpeg_create_decompress(&cinfo);

		jpeg_stdio_src(&cinfo, &ImageFile);

		/* Step 3: read image parameters with jpeg_read_header() */
		jpeg_read_header(&cinfo, TRUE);

		/* Step 4: set parameters for decompression */
		cinfo.dct_method = JDCT_FLOAT;

		/* Step 5: start decompressor */
		jpeg_start_decompress(&cinfo);

		//row_stride = cinfo.output_width * cinfo.output_components; // for RGB cinfo.output_components = 3 (R, G, B)


		//od tego miejsca będzią wysyłane dane na wyświetlacz

		ILI_SetAddrWindow(x, y, x + cinfo.output_width, y + cinfo.output_height);
		ILI_SendCommand(ILI_RAMWR);

		uint16_t RowNumber = 0;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);

			RGB_matrix = (RGB_typedef*)buffer[0];						//to jest tablica z informacją o kolorach


			for(int cnt = 0; cnt < cinfo.output_width; cnt++)			//przechodzimy po wszystkich pikselach
			{

				BUFOR16[cnt]=(uint16_t*)(((RGB_matrix[cnt].R >> 3)<<11)+			//tu mi zgłosił błąd ???????????
							 	 	 	 ((RGB_matrix[cnt].G >> 2)<< 5)+
										 ((RGB_matrix[cnt].B >> 3)    ));

			}



			ILI_SendToTFT(BUFOR16, (cinfo.output_width));  //wysyłamy na wyswietlacz tu usunołem *2 z nawiasu przy cinfo


			RowNumber++;												//liczenie wierszy

			if(RowNumber == cinfo.output_height)
			{
				break;
			}

		} // end while

		  /* Step 6: Finish decompression */
		  jpeg_finish_decompress(&cinfo);

		  /* Step 7: Release JPEG decompression object */
		  jpeg_destroy_decompress(&cinfo);

		 f_close(&ImageFile);

		 return 0;
	} // if

	return 1;
}





