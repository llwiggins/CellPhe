/**********************************************************************************************************

1.  	LICENSE.  The University of York grants to you the USER a non-exclusive, non-transferable right 

        to use the SOFTWARE on file servers connected to a maximum number of user computers, or on a 

		maximum number of user computers, not exceeding the number of user computers specified either 

		on the packaging or by the accompanying letter if one is so provided, and if neither is provided 

		the maximum number shall be one.  USER agrees not to use such SOFTWARE for any commercial purpose,

		and limit use of SOFTWARE for the purposes of research only. It is further agreed that the 

		furnishing of SOFTWARE to USER shall not constitute any grant or license to USER under any legal 

		rights now or hereinafter held by University of York.  



2.  	USER agrees that USER will only copy the SOFTWARE into any machine readable or printed form as 

        necessary to use it in accordance with this Agreement or for backup purposes in support of USER’s 

		use of the SOFTWARE. 



3.  	COPYRIGHT.  The SOFTWARE is protected by copyright law and international treaty provisions.  

        USER acknowledges that no title to the intellectual property in the SOFTWARE is transferred to 

		USER.  USER further acknowledge that title and full ownership rights to the SOFTWARE will remain 

		the exclusive property of University of York, and USER will not acquire any rights to the SOFTWARE

		except as expressly set forth in this Agreement. 



4.      LIMITATIONS.  USER agrees that it will not attempt to reverse compile, modify, translate, or 

        disassemble the SOFTWARE in whole or in part.  USER may not rent, lease, transfer or sublicense 

		the SOFTWARE to third parties.  USER may not modify the SOFTWARE or create derivative works based 

		upon the SOFTWARE.  



5.  	UPGRADES.  If this copy of the SOFTWARE is an upgrade from an earlier version of the software, it 

        is provided to USER on an exchange basis.  USER agrees by its installation and use of this copy of

		the SOFTWARE to voluntarily terminate USER’s earlier version of the SOFTWARE nor transfer it to 

		another.



6.  	LIMITED WARRANTIES.  University of York warrants that the media on which the SOFTWARE is furnished 

	    will be free from defects in materials and workmanship under normal use.  



7.      DISCLAIMER OF WARRANTY: EXCEPT AS SET FORTH HEREIN, SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF 

        ANY KIND.  TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, UNIVERSITY OF YORK FURTHER DISCLAIMS 

		ALL EXPRESSED AND IMPLIED WARRANTIES, INCLUDING WITHOUT LIMITATIONS ANY WARRANTIES OF MERCHANTABILITY,

		FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS DISCLAIMER OF WARRANTY CONSTITUTES AN 

		ESSENTIAL PART OF THIS AGREEMENT.



8.      GOVERNING LAW.  This Agreement will be governed by the laws of England without regard to conflict 

        of laws.



9.	    NO LIABILITY FOR CONSEQUENTIAL DAMAGES: IN NO EVENT SHALL UNIVERSITY OF YORK BE LIABLE TO USER FOR 

        ANY CONSEQUENTIAL, SPECIAL, INCIDENTAL OR INDIRECT DAMAGES OF ANY KIND ARISING OUT OF THE DELIVERY, 

		PERFORMANCE OR USE OF THE SOFTWARE, EVEN IF UNIVERSITY OF YORK HAS BEEN ADVISED OF THE POSSIBILITY 

		OF SUCH DAMAGES.  IN NO EVENT WILL UNIVERSITY OF YORK’S LIABILITY FOR ANY CLAIM, WHETHER IN CONTRACT, 

		TORT OR ANY OTHER THEORY OF LIABILITY, EXCEED THE AGREEMENT FEE PAID BY USER, IF ANY.

 ************************************************************************************************************/

#include <ppmutil.h>
#include "loadimage.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PixOFFSET 220

int* loadImage(char *filename, int *width, int *height, int *colour)
{
  image_info iinfo(filename);
  if(iinfo.get_colourspace()==4)
    iinfo.convert_greyscale();

  *width  = iinfo.get_width();
  *height = iinfo.get_height();
  *colour = iinfo.get_colourspace();

  /*printf("%d %d %d\n", *width, *height, *colour);*/
  unsigned char* pixels = iinfo.get_pixels();

  int *ipix = (int*) malloc(sizeof(int)*(iinfo.get_width()*iinfo.get_height()*iinfo.get_colourspace()));
  for(int i=0;i<iinfo.get_width()*iinfo.get_height()*iinfo.get_colourspace();i++)
    ipix[i] = pixels[i];

  return ipix;
}

int saveImage(char *filename, int *ipix, int width, int height, int colour)
{
  unsigned char *pixels = new unsigned char[width*height*colour];

  for(int i=0;i<width*height*colour;i++)
    pixels[i] = ipix[i];

  image_info iinfo;
  if(colour==1){
    iinfo = image_info(width,height,pixels,IMAGEINFO_MONO);
  }else if(colour==3){
    iinfo = image_info(width,height,pixels,IMAGEINFO_RGB);
  }else if(colour==4){
    iinfo = image_info(width,height,pixels,IMAGEINFO_RGBA);
  }
  iinfo.write(filename);

  return 0;
}

