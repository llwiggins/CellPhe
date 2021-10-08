/*
 *   CCP4 Molecular Graphics Program
 *
 *   Copyright (C) Stuart McNicholas and Liz Potterton 2004.
 *
 *   This program is free software and is distributed under the terms
 *   and conditions of the CCP4 licence agreement as `Part 0' (Annex 2)
 *   software, which is version 2.1 of the GNU Lesser General Public
 *   Licence (LGPL) with the following additional clause:
 *
 *      `You may also combine or link a "work that uses the Library"
 *      to produce a work containing portions of the Library, and
 *      distribute that work under terms of your choice, provided that
 *      you give prominent notice with each copy of the work that the
 *      specified version of the Library is used in it, and that you
 *      include or provide public access to the complete corresponding
 *      machine-readable source code for the Library including whatever
 *      changes were used in the work. (i.e. If you make changes to the
 *      Library you must distribute those, but you do not need to
 *      distribute source or object code to those portions of the work
 *      not covered by this licence.)'
 *
 *   Note that this clause grants an additional right and does not
 *   impose any additional restriction, and so does not affect
 *   compatibility with the GNU General Public Licence (GPL). If you
 *   wish to negotiate other terms, please contact the maintainer.
 *   You can redistribute it and/or modify the program under the terms
 *   of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2.1 of the License, or (at
 *   your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the CCP4 licence and/or GNU
 *   Lesser General Public License along with this program; if not,
 *   write to the CCP4 Secretary, Daresbury Laboratory, Warrington
 *   WA4 4AD, UK. The GNU Lesser General Public can also be obtained
 *   by writing to the Free Software Foundation, Inc., 59 Temple Place,
 *   Suite 330, Boston, MA 02111-1307 USA
 */

#ifdef _USE_DL_
#include <dlfcn.h>

#include "ppmdl.h"

static void *png_lib_handle;
static void *gif_lib_handle;
static void *jpeg_lib_handle;
static void *tiff_lib_handle;
static void *xpm_lib_handle;

int init_xpm(const char *filename){
#ifdef _USE_DL_
  const char *error;
  error = dlerror();

  if(xpm_lib_handle)
    return 1;

  xpm_lib_handle = dlopen (filename, RTLD_LAZY);
  if(!xpm_lib_handle){
    if ((error = dlerror()) != NULL) 
      fprintf (stderr, "%s\n", error);
    return 0;
  }
#ifdef USE_LIBXPM
  XpmReadFileToXpmImage = dlsym(xpm_lib_handle,"XpmReadFileToXpmImage");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  XpmWriteFileFromXpmImage = dlsym(xpm_lib_handle,"XpmWriteFileFromXpmImage");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
#endif // USE_LIBXPM
#endif // _USE_DL_
  return 1;
}
int init_tiff(const char *filename){
#ifdef _USE_DL_
  const char *error;
  error = dlerror();

  if(tiff_lib_handle)
    return 1;

  tiff_lib_handle = dlopen (filename, RTLD_LAZY);
  if(!tiff_lib_handle){
    if ((error = dlerror()) != NULL) 
      fprintf (stderr, "%s\n", error);
    return 0;
  }
#ifdef USE_LIBTIFF
  TIFFOpen              = dlsym(tiff_lib_handle,"TIFFOpen");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFGetField              = dlsym(tiff_lib_handle,"TIFFGetField");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFSetField              = dlsym(tiff_lib_handle,"TIFFSetField");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  _TIFFmalloc              = dlsym(tiff_lib_handle,"_TIFFmalloc");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFReadRGBAImage        = dlsym(tiff_lib_handle,"TIFFReadRGBAImage");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  _TIFFfree              = dlsym(tiff_lib_handle,"_TIFFfree");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFClose              = dlsym(tiff_lib_handle,"TIFFClose");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFWriteScanline              = dlsym(tiff_lib_handle,"TIFFWriteScanline");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFScanlineSize              = dlsym(tiff_lib_handle,"TIFFScanlineSize");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFNumberOfTiles              = dlsym(tiff_lib_handle,"TIFFNumberOfTiles");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFNumberOfStrips              = dlsym(tiff_lib_handle,"TIFFNumberOfStrips");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFStripSize                  = dlsym(tiff_lib_handle,"TIFFStripSize");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFReadScanline               = dlsym(tiff_lib_handle,"TIFFReadScanline");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFReadTile                   = dlsym(tiff_lib_handle,"TIFFReadTile");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  TIFFTileSize                   = dlsym(tiff_lib_handle,"TIFFTileSize");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
#endif

#endif
  return 1;
}

int init_gif(const char *filename){
#ifdef _USE_DL_
  const char *error;
  error = dlerror();

  if(gif_lib_handle)
    return 1;

  gif_lib_handle = dlopen (filename, RTLD_LAZY);
  if(!gif_lib_handle){
    if ((error = dlerror()) != NULL) 
      fprintf (stderr, "%s\n", error);
    return 0;
  }

  DGifOpenFileName_fun              = dlsym(gif_lib_handle,"DGifOpenFileName");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifOpenFileHandle_fun              = dlsym(gif_lib_handle,"DGifOpenFileHandle");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifOpen_fun              = dlsym(gif_lib_handle,"DGifOpen");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifSlurp_fun              = dlsym(gif_lib_handle,"DGifSlurp");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetScreenDesc_fun              = dlsym(gif_lib_handle,"DGifGetScreenDesc");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetRecordType_fun              = dlsym(gif_lib_handle,"DGifGetRecordType");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetImageDesc_fun              = dlsym(gif_lib_handle,"DGifGetImageDesc");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetLine_fun              = dlsym(gif_lib_handle,"DGifGetLine");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetPixel_fun              = dlsym(gif_lib_handle,"DGifGetPixel");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetExtension_fun              = dlsym(gif_lib_handle,"DGifGetExtension");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetExtensionNext_fun              = dlsym(gif_lib_handle,"DGifGetExtensionNext");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetCode_fun              = dlsym(gif_lib_handle,"DGifGetCode");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetCodeNext_fun              = dlsym(gif_lib_handle,"DGifGetCodeNext");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifGetLZCodes_fun              = dlsym(gif_lib_handle,"DGifGetLZCodes");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  DGifCloseFile_fun              = dlsym(gif_lib_handle,"DGifCloseFile");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  EGifOpenFileName_fun            = dlsym(gif_lib_handle,"EGifOpenFileName");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  MakeMapObject_fun            = dlsym(gif_lib_handle,"MakeMapObject");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  EGifPutScreenDesc_fun            = dlsym(gif_lib_handle,"EGifPutScreenDesc");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  EGifPutImageDesc_fun            = dlsym(gif_lib_handle,"EGifPutImageDesc");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  EGifPutLine_fun            = dlsym(gif_lib_handle,"EGifPutLine");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  EGifCloseFile_fun            = dlsym(gif_lib_handle,"EGifCloseFile");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  EGifSetGifVersion_fun            = dlsym(gif_lib_handle,"EGifSetGifVersion");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

#endif
  return 1;
  
}
int init_jpeg(const char *filename){
#ifdef _USE_DL_
  const char *error;
  error = dlerror();

  if(jpeg_lib_handle)
    return 1;

  jpeg_lib_handle = dlopen (filename, RTLD_LAZY);
  if(!jpeg_lib_handle){
    if ((error = dlerror()) != NULL) 
      fprintf (stderr, "%s\n", error);
    return 0;
  }

  jpeg_std_error_fun              = dlsym(jpeg_lib_handle,"jpeg_std_error");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_CreateCompress_fun              = dlsym(jpeg_lib_handle,"jpeg_CreateCompress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_stdio_dest_fun              = dlsym(jpeg_lib_handle,"jpeg_stdio_dest");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_set_defaults_fun              = dlsym(jpeg_lib_handle,"jpeg_set_defaults");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_start_compress_fun              = dlsym(jpeg_lib_handle,"jpeg_start_compress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_write_scanlines_fun              = dlsym(jpeg_lib_handle,"jpeg_write_scanlines");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_finish_compress_fun              = dlsym(jpeg_lib_handle,"jpeg_finish_compress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_destroy_compress_fun              = dlsym(jpeg_lib_handle,"jpeg_destroy_compress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_CreateDecompress_fun              = dlsym(jpeg_lib_handle,"jpeg_CreateDecompress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_stdio_src_fun              = dlsym(jpeg_lib_handle,"jpeg_stdio_src");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_read_header_fun              = dlsym(jpeg_lib_handle,"jpeg_read_header");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_start_decompress_fun              = dlsym(jpeg_lib_handle,"jpeg_start_decompress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_read_scanlines_fun              = dlsym(jpeg_lib_handle,"jpeg_read_scanlines");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_finish_decompress_fun              = dlsym(jpeg_lib_handle,"jpeg_finish_decompress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_destroy_decompress_fun              = dlsym(jpeg_lib_handle,"jpeg_destroy_decompress");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

  jpeg_set_quality_fun              = dlsym(jpeg_lib_handle,"jpeg_set_quality");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }

#endif
  return 1;
  
}

int init_png(const char *filename){
#ifdef _USE_DL_
  const char *error;
  error = dlerror();

  if(png_lib_handle)
    return 1;

  png_lib_handle = dlopen (filename, RTLD_LAZY);
  if(!png_lib_handle){
    if ((error = dlerror()) != NULL) 
      fprintf (stderr, "%s\n", error);
    return 0;
  }

  png_sig_cmp_fun              = dlsym(png_lib_handle,"png_sig_cmp");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_create_read_struct_fun   = dlsym(png_lib_handle,"png_create_read_struct");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_create_write_struct_fun   = dlsym(png_lib_handle,"png_create_write_struct");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_create_info_struct_fun   = dlsym(png_lib_handle,"png_create_info_struct");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_destroy_info_struct_fun  = dlsym(png_lib_handle,"png_destroy_info_struct");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_destroy_read_struct_fun  = dlsym(png_lib_handle,"png_destroy_read_struct");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_destroy_write_struct_fun = dlsym(png_lib_handle,"png_destroy_write_struct");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_set_filter_fun           = dlsym(png_lib_handle,"png_set_filter");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_init_io_fun              = dlsym(png_lib_handle,"png_init_io");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_set_IHDR_fun             = dlsym(png_lib_handle,"png_set_IHDR");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_get_IHDR_fun             = dlsym(png_lib_handle,"png_get_IHDR");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_malloc_fun               = dlsym(png_lib_handle,"png_malloc");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_write_rows_fun             = dlsym(png_lib_handle,"png_write_rows");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_set_rows_fun             = dlsym(png_lib_handle,"png_set_rows");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_read_png_fun             = dlsym(png_lib_handle,"png_read_png");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_write_png_fun            = dlsym(png_lib_handle,"png_write_png");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_read_end_fun             = dlsym(png_lib_handle,"png_read_end");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_write_end_fun            = dlsym(png_lib_handle,"png_write_end");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_set_sig_bytes_fun        = dlsym(png_lib_handle,"png_set_sig_bytes");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_read_info_fun            = dlsym(png_lib_handle,"png_read_info");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_get_channels_fun         = dlsym(png_lib_handle,"png_get_channels");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_set_strip_alpha_fun      = dlsym(png_lib_handle,"png_set_strip_alpha");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_set_strip_16_fun         = dlsym(png_lib_handle,"png_set_strip_16");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_set_packing_fun          = dlsym(png_lib_handle,"png_set_packing");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
  png_read_image_fun          = dlsym(png_lib_handle,"png_read_image");
  if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      return 0;
  }
#endif
  return 1;
  
}

#endif
