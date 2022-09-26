/*****************************************************************//**
 * \file   glTFUtils.h
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
#ifndef __GLTF_UTILS_H__
#define __GLTF_UTILS_H__

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QImage>

#include "gltfloader_global.h"
#include "glTFException.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::quat;

namespace jcqt
{
	bool GLTFLOADER_EXPORT isValidGLTFBufferFromJson ( const QJsonObject& jsonObj )
	{
		return ( jsonObj.contains ( "byteLength" ) );
	}

	QString GLTFLOADER_EXPORT getBufferFilenameFromUriString ( const QString& uriString )
	{
		QUrl url ( uriString );
		return url.path ();
	}

	bool GLTFLOADER_EXPORT isValidGLTFImageFromJson ( const QJsonObject& jsonObj )
	{
		bool hasUri = jsonObj.contains ( "uri" );
		bool hasBufferView = jsonObj.contains ( "bufferView" );
		bool hasMimeType = jsonObj.contains ( "mimeType" );

		if ( hasUri )
		{
			return !hasBufferView;
		}

		if ( hasBufferView )
		{
			return hasMimeType;
		}

		return false;
	}

	bool GLTFLOADER_EXPORT isInternalGLTFImageFromUriString ( const QString& uriStr )
	{
		return uriStr.startsWith ( "data:", Qt::CaseInsensitive );
	}

	bool GLTFLOADER_EXPORT loadGLTFImageFromInternalUriString ( const QString& uriStr, QImage& image, const char* mimeType )
	{
		try
		{
			const QString base64Str = "base64,";
			qsizetype idxBase64 = uriStr.indexOf ( base64Str );
			if ( -1 != idxBase64 )
			{
				QString dataStr = uriStr.sliced ( idxBase64 + base64Str.size () );
				QByteArray bufferData = dataStr.toUtf8 ();
				QByteArray::FromBase64Result bufferDataDecoded = QByteArray::fromBase64Encoding ( bufferData );
				if ( bufferDataDecoded.decodingStatus != QByteArray::Base64DecodingStatus::Ok )
				{
					throw new GLTFException ( "BASE 64 DECODING FAILED" );
				}

				QByteArray decodedData = bufferDataDecoded.decoded;

				image = QImage::fromData ( decodedData, mimeType );
				return true;
			}

			const QString octStreamStr = "application/octet-stream,";
			qsizetype idxOctStream = uriStr.indexOf ( octStreamStr );
			if ( -1 != idxOctStream )
			{
				QString dataStr = uriStr.sliced ( idxOctStream + octStreamStr.size () );
				QByteArray bufferData = dataStr.toUtf8 ();
				QByteArray::FromBase64Result bufferDataDecoded = QByteArray::fromBase64Encoding ( bufferData );
				if ( bufferDataDecoded.decodingStatus != QByteArray::Base64DecodingStatus::Ok )
				{
					throw new GLTFException ( "BASE 64 DECODING FAILED" );
				}

				QByteArray decodedData = bufferDataDecoded.decoded;

				image = QImage::fromData ( decodedData, mimeType );
				return true;
			}

			const QString gltfBufferStr = "application/gltf-buffer,";
			qsizetype gltfBufferIdx = uriStr.indexOf ( gltfBufferStr );
			if ( -1 != gltfBufferIdx )
			{
				QString dataStr = uriStr.sliced ( gltfBufferIdx + gltfBufferStr.size () );
				QByteArray bufferData = dataStr.toUtf8 ();
				QByteArray::FromBase64Result bufferDataDecoded = QByteArray::fromBase64Encoding ( bufferData );
				if ( bufferDataDecoded.decodingStatus != QByteArray::Base64DecodingStatus::Ok )
				{
					throw new GLTFException ( "BASE 64 DECODING FAILED" );
				}

				QByteArray decodedData = bufferDataDecoded.decoded;

				image = QImage::fromData ( decodedData, mimeType );
				return true;
			}
		}
		catch ( const GLTFException& e )
		{
			qWarning () << e.what () << Qt::endl;
			return false;
		}

		return false;
	}

	QString GLTFLOADER_EXPORT getGLTFImageFilenameFromUriString ( const QString& uriStr )
	{
		QUrl url ( uriStr );
		return url.path ();
	}


	quat GLTFLOADER_EXPORT getQuatFromJsonArray ( const QJsonArray& jsonArr )
	{
		float x = static_cast< float >( jsonArr.at ( 0 ).toDouble () );
		float y = static_cast< float >( jsonArr.at ( 1 ).toDouble () );
		float z = static_cast< float >( jsonArr.at ( 2 ).toDouble () );
		float w = static_cast< float >( jsonArr.at ( 3 ).toDouble ( 1.0 ) );
		return quat ( w, x, y, z );
	}

	vec3 GLTFLOADER_EXPORT getTransFromJsonArray ( const QJsonArray& jsonArr )
	{
		float x = static_cast< float >( jsonArr.at ( 0 ).toDouble () );
		float y = static_cast< float >( jsonArr.at ( 1 ).toDouble () );
		float z = static_cast< float >( jsonArr.at ( 2 ).toDouble () );
		return vec3 ( x, y, z );
	}

	vec3 GLTFLOADER_EXPORT getScaleFromJsonArray ( const QJsonArray& jsonArr )
	{
		float x = static_cast< float >( jsonArr.at ( 0 ).toDouble ( 1.0 ) );
		float y = static_cast< float >( jsonArr.at ( 1 ).toDouble ( 1.0 ) );
		float z = static_cast< float >( jsonArr.at ( 2 ).toDouble ( 1.0 ) );
		return vec3 ( x, y, z );
	}

	mat4 GLTFLOADER_EXPORT createMat4FromJsonArray ( const QJsonArray& jsonArr )
	{
		mat4 matrix;

		for ( qsizetype col = 0; col < 4; col++ )
		{
			vec4 columnVec;
			for ( qsizetype row = 0; row < 4; row++ )
			{
				columnVec [ row ] = static_cast< float >( jsonArr.at ( col * 4 + row ).toDouble ( ( row == col ) ? 1.0 : 0.0 ) );
			}

			matrix [ col ] = columnVec;
		}

		return matrix;
	}

	mat4 GLTFLOADER_EXPORT createMat4FromTRS ( const QJsonArray& trans, const QJsonArray& rot, const QJsonArray& scale )
	{
		vec3 scaleVec = getScaleFromJsonArray ( scale );
		vec3 transVec = getTransFromJsonArray ( trans );
		quat rotQuat = getQuatFromJsonArray ( rot );
		mat4 scaleMat = glm::scale ( mat4 ( 1.0f ), scaleVec );
		mat4 transMat = glm::translate ( mat4 ( 1.0f ), transVec );
		mat4 rotMat = glm::mat4_cast ( rotQuat );
		return transMat * rotMat * scaleMat;
	}

	void GLTFLOADER_EXPORT getChildrenFromJsonArray ( const QJsonArray& jsonArr, QList<qsizetype>& children )
	{
		qsizetype count = jsonArr.size ();
		children.resize ( count );
		for ( qsizetype i = 0; i < count; i++ )
		{
			children [ i ] = jsonArr.at ( i ).toInteger ();
		}
	}

	void GLTFLOADER_EXPORT getWeightsFromJsonArray ( const QJsonArray& jsonArr, QList<float>& weights )
	{
		qsizetype count = jsonArr.size ();
		weights.resize ( count );
		for ( qsizetype i = 0; i < count; i++ )
		{
			weights [ i ] = static_cast< float >( jsonArr.at ( i ).toDouble ( 1.0 ) );
		}
	}
}

#endif // !__GLTF_UTILS_H__
