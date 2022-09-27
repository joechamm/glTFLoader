/*****************************************************************//**
 * \file   glTFOpenGLScene.h
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
#ifndef __GLTF_OPENGL_SCENE_H__
#define __GLTF_OPENGL_SCENE_H__

#include "gltfopenglscene_global.h"

#include <QList>
#include <QMap>

#include <QOpenGLContext>
#include <QSharedPointer>

#include "../glTFLoader/glTFLoader.h"

class QOpenGLTexture;

namespace jcqt
{
	class GLTFOPENGLSCENE_EXPORT glTFOpenGLScene
	{
	public:
		glTFOpenGLScene ();
		~glTFOpenGLScene ();

		static QOpenGLContext* getContext () 
		{ 
			return s_context.data();
		}
		
		static void setContext ( QOpenGLContext* context )
		{
			s_context.reset ( context );
		}

		glTFLoader* getLoader () const
		{
			return m_loader.data ();
		}

		void setLoader ( glTFLoader* loader )
		{
			m_loader.reset ( loader );
		}

		bool loadTextures ();
		bool loadBuffers ();
		bool loadVertexArrayObjects ();

	private:

		static QSharedPointer<QOpenGLContext>	s_context;

		QSharedPointer<glTFLoader>				m_loader;		
		QList<GLuint>							m_textures;
		QList<GLuint>							m_vaos;
		QList<GLuint>							m_vbos;

		QMap<QString, GLuint>					m_attributeIndexMap;
	};
}


#endif // !__GLTF_OPENGL_SCENE_H__
