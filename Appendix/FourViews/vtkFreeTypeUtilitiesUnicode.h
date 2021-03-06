/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFreeTypeUtilitiesUnicode.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**********************************************************************

  文件名: vtkFreeTypeUtilitiesUnicode.h
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

// .NAME vtkFreeTypeUtilitiesUnicode - FreeType library support
// .SECTION Description
// vtkFreeTypeUtilitiesUnicode provides low-level interface to the FreeType library,
// including font-cache, rasterizing and vectorizing. FreeType cache-subsystem
// is supported only when FreeType version is greater than 2.1.9. 
// Internal use only.
// EXPERIMENTAL for the moment. Also include the old cache.
#ifndef __vtkFreeTypeUtilitiesUnicode_h
#define __vtkFreeTypeUtilitiesUnicode_h

#define VTK_FTFC_CACHE_CAPACITY 150

#include "vtkObject.h"

class vtkImageData;
class vtkTextUnicodeProperty;

// FreeType

#include "vtk_freetype.h"  //since ft2build.h could be in the path
#include FT_FREETYPE_H
#include FT_GLYPH_H

#if (FREETYPE_MAJOR >2 ||(FREETYPE_MAJOR == 2 && ( FREETYPE_MINOR > 1 || (FREETYPE_MINOR == 1 && FREETYPE_PATCH >= 9))))
# include FT_CACHE_H
// This flag will be used to check if Caching support is to be compiled.
# define VTK_FREETYPE_CACHING_SUPPORTED
#endif

//class FTFont;
#include <ftgl/src/FTFont.h>

//----------------------------------------------------------------------------
// Singleton cleanup
class  vtkFreeTypeUtilitiesUnicodeCleanup
{
public:
	vtkFreeTypeUtilitiesUnicodeCleanup();
	~vtkFreeTypeUtilitiesUnicodeCleanup();
};

//----------------------------------------------------------------------------
// Singleton font cache
class vtkFreeTypeUtilitiesUnicode : public vtkObject
{
public:
	vtkTypeMacro(vtkFreeTypeUtilitiesUnicode, vtkObject);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// This is a singleton pattern New. There will be only ONE reference 
	// to a vtkFreeTypeUtilitiesUnicode object per process.  Clients that
	// call this method must use Delete() on the object so that reference
	// counting will work. The single instance will be unreferenced when
	// the program exits. You should just use the static GetInstance() method
	// anyway to get the singleton.
	static vtkFreeTypeUtilitiesUnicode *New();

	// Description:
	// Return the singleton instance with no reference counting.
	static vtkFreeTypeUtilitiesUnicode* GetInstance();

	// Description:
	// Supply a user defined instance. Call Delete() on the supplied
	// instance after setting it to fix the reference count.
	static void SetInstance(vtkFreeTypeUtilitiesUnicode *instance);

	// Old Code
	// Cache entry

	struct Entry
	{
		vtkTextUnicodeProperty *TextProperty;
		ftgl::FTFont *Font;
		float LargestAscender;
		float LargestDescender;
	};

	vtkFreeTypeUtilitiesUnicode::Entry* GetFont(vtkTextUnicodeProperty *tprop,
		double override_color[3] = NULL);

protected:
	vtkFreeTypeUtilitiesUnicode();
	virtual ~vtkFreeTypeUtilitiesUnicode();

private:
	vtkFreeTypeUtilitiesUnicode(const vtkFreeTypeUtilitiesUnicode&);  // Not implemented.
	void operator=(const vtkFreeTypeUtilitiesUnicode&);  // Not implemented.

	// The singleton instance and the singleton cleanup instance
	static vtkFreeTypeUtilitiesUnicode* Instance;
	static vtkFreeTypeUtilitiesUnicodeCleanup Cleanup;

	// Old Code
	void PrintEntry(int i, char *msg = 0);
	void ReleaseEntry(int i);

	void InitializeCache();
	void ReleaseCache();

	Entry *Entries[VTK_FTFC_CACHE_CAPACITY];
	int NumberOfEntries;
};

#endif
