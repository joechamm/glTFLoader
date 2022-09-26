/*****************************************************************//**
 * \file   glTFImage.h
 * \licence MIT License

Copyright (c) 2022 Joseph Cunningham

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 * \brief  
 * 
 * \author joechamm
 * \date   September 2022
 *********************************************************************/
#ifndef __GLTF_IMAGE_H__
#define __GLTF_IMAGE_H__

#include <QObject>
#include <QJsonObject>
#include <QImage>
#include <QUrl>
#include <QDir>

#include "glTFUtils.h"

namespace jcqt
{
	struct GLTFLOADER_EXPORT Image
	{
		QImage		m_image;
		QJsonObject m_jsonObject;
	};

	Image GLTFLOADER_EXPORT createImageFromJson ( const QJsonObject& jsonObj )
	{
		Image image;

		try
		{
			if ( !isValidGLTFImageFromJson ( jsonObj ) )
			{
				throw new GLTFException ( "INVALID JSON EXCEPTION" );
			}

			if ( jsonObj.contains ( "uri" ) )
			{
				QString uriStr = jsonObj.value ( "uri" ).toString ();
				if ( isInternalGLTFImageFromUriString ( uriStr ) )
				{
					QString mimeType = jsonObj.value ( "mimeType" ).toString ();
					if ( mimeType.isEmpty () ) // use image/jpeg for default mimeType
					{
						loadGLTFImageFromInternalUriString ( uriStr, image.m_image, "image/jpeg" );
					}
					else
					{
						loadGLTFImageFromInternalUriString ( uriStr, image.m_image, mimeType.toStdString ().c_str () );
					}
				}
				else
				{
					QString filename = getGLTFImageFilenameFromUriString ( uriStr );
					image.m_image.load ( filename );
				}
			}

			image.m_jsonObject = jsonObj;
			return image;
		}
		catch ( const GLTFException& e )
		{
			qWarning () << e.what () << Qt::endl;
		}

		return image;
	}
}

#endif // __GLTF_IMAGE_H__