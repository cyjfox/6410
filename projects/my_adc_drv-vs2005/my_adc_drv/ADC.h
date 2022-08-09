//----------------------------------------------------------------------------//
//	NAME OF PROJECT: uniprOS										Version: 1.0 (development)	  //
//----------------------------------------------------------------------------//
//	NAME OF SOURCE FILE: SPI.h							 					  											//
//----------------------------------------------------------------------------//
//																		  																			//
//	Data of creation: 2010.02.15										  												//
//	Data of last modification: 2010.02.15								  										//
//																		  																			//
//	CONTENTS:	unipro CNC module.			  									  									//
//																		  																			//
//----------------------------------------------------------------------------//
//	NAME OF PROJECT FILE: uniprOS.exe									  											//
//----------------------------------------------------------------------------//
//	UTILIZING COMPILER: Visual Studio 2005								  									//
//----------------------------------------------------------------------------//
//							YUE Wei - Unipro							  															//
//----------------------------------------------------------------------------//

#ifndef BEE_H
#define BEE_H

// constants define
#define IOCTL_ADC_START \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 60, METHOD_BUFFERED, FILE_ANY_ACCESS) //60???
#define IOCTL_ADC_STOP \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)



#endif
