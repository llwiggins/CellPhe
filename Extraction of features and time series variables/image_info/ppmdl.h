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


#ifndef _PPMDL_H_
#define _PPMDL_H_


#ifdef __cplusplus
extern "C"{
#endif
int init_png(const char *filename);
int init_gif(const char *filename);
int init_jpeg(const char *filename);
int init_tiff(const char *filename);
int init_xpm(const char *filename);
#ifdef __cplusplus
}
#endif

#ifdef _USE_DL_

#ifdef USE_LIBPNG
#include <png.h>
#endif
#ifdef USE_LIBGIF
#include <gif_lib.h>
#endif
#ifdef USE_LIBXPM
#include <X11/xpm.h>
#endif
#ifdef USE_LIBTIFF
#include <tiffio.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif

#ifdef USE_LIBTIFF
TIFF*(*TIFFOpen_fun)(const char *FileName, const char *mode);
int (*TIFFGetField_fun)(TIFF*, ttag_t, ...);
int (*TIFFSetField_fun)(TIFF*, ttag_t, ...);
void* (*_TIFFmalloc_fun)(int size);
int (*TIFFReadRGBAImage_fun)(TIFF*, uint32, uint32, uint32*, int);
void (*_TIFFfree_fun)(tdata_t);
int (*TIFFWriteScanline_fun)(TIFF*, tdata_t, uint32, tsample_t);
void (*TIFFClose_fun)(TIFF*);
tsize_t (*TIFFScanlineSize_fun)(TIFF*);
ttile_t (*TIFFNumberOfTiles_fun)(TIFF*);
tstrip_t (*TIFFNumberOfStrips_fun)(TIFF*);
tsize_t (*TIFFStripSize_fun)(TIFF*);
int (*TIFFReadScanline_fun)(TIFF*, tdata_t, uint32, tsample_t);
tsize_t (*TIFFReadTile_fun)(TIFF*, tdata_t, uint32, uint32, uint32, tsample_t);
tsize_t (*TIFFTileSize_fun)(TIFF*);
#endif

#ifdef USE_LIBPNG
int(*png_sig_cmp_fun)(png_bytep,png_size_t,png_size_t);
png_structp(*png_create_read_struct_fun)(png_const_charp user_png_ver, 
				   png_voidp error_ptr, png_error_ptr error_fn,
				   png_error_ptr warn_fn);
png_infop(*png_create_info_struct_fun)(png_structp png_ptr);
void(*png_destroy_info_struct_fun)(png_structp png_ptr, png_infopp info_ptr_ptr);
void(*png_destroy_read_struct_fun)(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr);
void(*png_destroy_write_struct_fun)(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr);

void(*png_set_filter_fun)(png_structp png_ptr, int method,  int filters);
void(*png_init_io_fun)(png_structp png_ptr, png_FILE_p fp);
void(*png_set_IHDR_fun)(png_structp png_ptr, png_infop info_ptr, png_uint_32 width, png_uint_32 height, 
		  int bit_depth, int color_type, int interlace_method, int compression_method, int filter_method);
png_voidp(*png_malloc_fun)(png_structp png_ptr, png_uint_32 size);
void(*png_set_rows_fun)(png_structp png_ptr, png_infop info_ptr, png_bytepp row_pointers);
void(*png_write_rows_fun)(png_structp png_ptr, png_bytepp row, png_uint_32 num_rows);
void(*png_write_png_fun)(png_structp png_ptr, png_infop info_ptr, int transforms, png_voidp params);
void(*png_read_png_fun)(png_structp png_ptr, png_infop info_ptr, int transforms, png_voidp params);
void(*png_write_end_fun)(png_structp png_ptr, png_infop info_ptr);
void(*png_read_end_fun)(png_structp png_ptr, png_infop info_ptr);
void(*png_set_sig_bytes_fun)(png_structp png_ptr, int num_bytes);
void(*png_read_info_fun)(png_structp png_ptr, png_infop info_ptr);
png_uint_32(*png_get_IHDR_fun)(png_structp png_ptr, png_infop info_ptr, png_uint_32 *width, png_uint_32 *height, 
			 int *bit_depth, int *color_type, int *interlace_method, int *compression_method, 
			 int *filter_method);
png_byte(*png_get_channels_fun)(png_structp png_ptr,png_infop info_ptr);
void(*png_set_strip_alpha_fun)(png_structp png_ptr);
void(*png_set_strip_16_fun)(png_structp png_ptr);
void(*png_set_packing_fun)(png_structp png_ptr);
void (*png_read_image_fun) (png_structp png_ptr, png_bytepp image);
png_structp (*png_create_write_struct_fun)(png_const_charp user_png_ver, png_voidp error_ptr,png_error_ptr error_fn, png_error_ptr warn_fn);
#endif
  
#ifdef USE_LIBGIF
GifFileType*(*EGifOpenFileName_fun)(const char *GifFileName, int GifTestExistance);
ColorMapObject*(*MakeMapObject_fun)(int, int);
int(*EGifPutScreenDesc_fun)(GifFileType *,int,int,int,int,ColorMapObject*);
int(*EGifPutImageDesc_fun)(GifFileType *GifFile,int GifLeft,int GifTop,int Width,int GifHeight,int GifInterlace,ColorMapObject *GifColorMap);

int(*EGifPutLine_fun)(GifFileType *GifFile, GifPixelType *GifLine, int GifLineLen);
int(*EGifCloseFile_fun)(GifFileType *GifFile);
void(*EGifSetGifVersion_fun)(const char *Version);

GifFileType*(* DGifOpenFileName_fun)(const char *GifFileName);
GifFileType*(*DGifOpenFileHandle_fun)(int GifFileHandle);
GifFileType*(*DGifOpen_fun)( void* userPtr, InputFunc readFunc );  /* new one (TVT) */
int(*DGifSlurp_fun)(GifFileType *GifFile);
int(*DGifGetScreenDesc_fun)(GifFileType *GifFile);
int(*DGifGetRecordType_fun)(GifFileType *GifFile, GifRecordType *GifType);
int(*DGifGetImageDesc_fun)(GifFileType *GifFile);
int(*DGifGetLine_fun)(GifFileType *GifFile, GifPixelType *GifLine, int GifLineLen);
int(*DGifGetPixel_fun)(GifFileType *GifFile, GifPixelType GifPixel);
int(*DGifGetExtension_fun)(GifFileType *GifFile, int *GifExtCode,
		       GifByteType **GifExtension);
int(*DGifGetExtensionNext_fun)(GifFileType *GifFile, GifByteType **GifExtension);
int(*DGifGetCode_fun)(GifFileType *GifFile, int *GifCodeSize,
		  GifByteType **GifCodeBlock);
int(*DGifGetCodeNext_fun)(GifFileType *GifFile, GifByteType **GifCodeBlock);
int(*DGifGetLZCodes_fun)(GifFileType *GifFile, int *GifCode);
int(*DGifCloseFile_fun)(GifFileType *GifFile);
#endif

#ifdef USE_LIBJPEG
#include<jpeglib.h>
struct jpeg_error_mgr*(*jpeg_std_error_fun)(struct jpeg_error_mgr * err);
void (*jpeg_CreateCompress_fun)(j_compress_ptr cinfo, int,size_t );
void (*jpeg_stdio_dest_fun)(j_compress_ptr cinfo, FILE * outfile);
void (*jpeg_set_defaults_fun)(j_compress_ptr cinfo);
void (*jpeg_start_compress_fun)(j_compress_ptr cinfo, boolean write_all_tables);
JDIMENSION (*jpeg_write_scanlines_fun)(j_compress_ptr cinfo, JSAMPARRAY scanlines,  JDIMENSION num_lines);
void (*jpeg_finish_compress_fun)(j_compress_ptr cinfo);
void (*jpeg_destroy_compress_fun)(j_compress_ptr cinfo);
void (*jpeg_CreateDecompress_fun)(j_decompress_ptr cinfo, int,size_t );
void (*jpeg_stdio_src_fun)(j_decompress_ptr cinfo, FILE * infile);
int (*jpeg_read_header_fun)(j_decompress_ptr cinfo, boolean require_image);
boolean (*jpeg_start_decompress_fun)(j_decompress_ptr cinfo);
JDIMENSION (*jpeg_read_scanlines_fun)(j_decompress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION max_lines);
boolean (*jpeg_finish_decompress_fun)(j_decompress_ptr cinfo);
void (*jpeg_destroy_decompress_fun)(j_decompress_ptr cinfo);
void (*jpeg_set_quality_fun)(j_compress_ptr cinfo, int quality, boolean force_baseline);
#endif

#ifdef USE_LIBXPM
int (*XpmReadFileToXpmImage_fun)(char *filename, XpmImage* image, XpmInfo* info); 
int (*XpmWriteFileFromXpmImage_fun)(char *filename, XpmImage* image, XpmInfo* info); 
#endif

#ifdef __cplusplus
}
#endif

#ifdef USE_LIBPNG
#define png_sig_cmp png_sig_cmp_fun
#define png_create_read_struct png_create_read_struct_fun
#define png_create_write_struct png_create_write_struct_fun
#define png_create_info_struct png_create_info_struct_fun
#define png_destroy_info_struct png_destroy_info_struct_fun
#define png_destroy_read_struct png_destroy_read_struct_fun
#define png_destroy_write_struct png_destroy_write_struct_fun
#define png_set_filter png_set_filter_fun
#define png_init_io png_init_io_fun
#define png_set_IHDR png_set_IHDR_fun
#define png_malloc png_malloc_fun
#define png_set_rows png_set_rows_fun
#define png_write_rows png_write_rows_fun
#define png_write_png png_write_png_fun
#define png_read_png png_read_png_fun
#define png_write_end png_write_end_fun
#define png_read_end png_read_end_fun
#define png_set_sig_bytes png_set_sig_bytes_fun
#define png_read_info png_read_info_fun
#define png_get_IHDR png_get_IHDR_fun
#define png_get_channels png_get_channels_fun
#define png_set_strip_alpha png_set_strip_alpha_fun
#define png_set_strip_16 png_set_strip_16_fun
#define png_set_packing png_set_packing_fun
#define png_read_image png_read_image_fun
#endif

#ifdef USE_LIBGIF
#define DGifOpenFileName DGifOpenFileName_fun
#define DGifOpenFileHandle DGifOpenFileHandle_fun
#define DGifOpen DGifOpen_fun
#define DGifSlurp DGifSlurp_fun
#define DGifGetScreenDesc DGifGetScreenDesc_fun
#define DGifGetRecordType DGifGetRecordType_fun
#define DGifGetImageDesc DGifGetImageDesc_fun
#define DGifGetLine DGifGetLine_fun
#define DGifGetPixel DGifGetPixel_fun
#define DGifGetExtension DGifGetExtension_fun
#define DGifGetExtensionNext DGifGetExtensionNext_fun
#define DGifGetCode DGifGetCode_fun
#define DGifGetCodeNext DGifGetCodeNext_fun
#define DGifGetLZCodes DGifGetLZCodes_fun
#define DGifCloseFile DGifCloseFile_fun

#define EGifOpenFileName EGifOpenFileName_fun
#define MakeMapObject MakeMapObject_fun
#define EGifPutScreenDesc EGifPutScreenDesc_fun
#define EGifPutImageDesc EGifPutImageDesc_fun
#define EGifPutLine EGifPutLine_fun
#define EGifCloseFile EGifCloseFile_fun
#define EGifSetGifVersion EGifSetGifVersion_fun
#endif

#ifdef USE_LIBJPEG
#define jpeg_std_error jpeg_std_error_fun
#define jpeg_CreateCompress jpeg_CreateCompress_fun
#define jpeg_stdio_dest jpeg_stdio_dest_fun
#define jpeg_set_defaults jpeg_set_defaults_fun
#define jpeg_start_compress jpeg_start_compress_fun
#define jpeg_write_scanlines jpeg_write_scanlines_fun
#define jpeg_finish_compress jpeg_finish_compress_fun
#define jpeg_destroy_compress jpeg_destroy_compress_fun
#define jpeg_CreateDecompress jpeg_CreateDecompress_fun
#define jpeg_stdio_src jpeg_stdio_src_fun
#define jpeg_read_header jpeg_read_header_fun
#define jpeg_start_decompress jpeg_start_decompress_fun
#define jpeg_read_scanlines jpeg_read_scanlines_fun
#define jpeg_finish_decompress jpeg_finish_decompress_fun
#define jpeg_destroy_decompress jpeg_destroy_decompress_fun
#define jpeg_set_quality jpeg_set_quality_fun
#endif

#ifdef USE_LIBTIFF
#define TIFFOpen TIFFOpen_fun
#define TIFFGetField TIFFGetField_fun
#define TIFFSetField TIFFSetField_fun
#define _TIFFmalloc _TIFFmalloc_fun
#define TIFFReadRGBAImage TIFFReadRGBAImage_fun
#define _TIFFfree _TIFFfree_fun
#define TIFFWriteScanline TIFFWriteScanline_fun
#define TIFFClose TIFFClose_fun
#define TIFFScanlineSize TIFFScanlineSize_fun
#define TIFFNumberOfTiles TIFFNumberOfTiles_fun
#define TIFFNumberOfStrips TIFFNumberOfStrips_fun
#define TIFFStripSize TIFFStripSize_fun
#define TIFFReadScanline TIFFReadScanline_fun
#define TIFFReadTile TIFFReadTile_fun
#define TIFFTileSize TIFFTileSize_fun
#endif

#ifdef USE_LIBXPM
#define XpmReadFileToXpmImage XpmReadFileToXpmImage_fun
#define XpmWriteFileFromXpmImage XpmWriteFileFromXpmImage_fun
#endif

#endif

#endif
