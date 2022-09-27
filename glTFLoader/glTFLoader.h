/*****************************************************************//**
 * \file   glTFLoader.h
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
#ifndef __GLTF_LOADER_H__
#define __GLTF_LOADER_H__

#include "gltfloader_global.h"

#include <QJsonDocument>

#include "glTFUtils.h"
#include "glTFAccessor.h"
#include "glTFAnimation.h"
#include "glTFBuffer.h"
#include "glTFBufferView.h"
#include "glTFCamera.h"
#include "glTFImage.h"
#include "glTFMaterial.h"
#include "glTFMesh.h"
#include "glTFNode.h"
#include "glTFSampler.h"
#include "glTFSkin.h"

namespace jcqt
{
	class GLTFLOADER_EXPORT glTFLoader
	{
	public:
		glTFLoader ();
		glTFLoader ( const QString& glTFFilename );
		glTFLoader ( const QJsonDocument& glTFDoc );
		glTFLoader ( const char* glTFFilename );

		// returns true if m_document isn't empty
		bool hasDoc () const;

		// TODO: implement an 'isValid' method

		// try to load glTF json document from the supplied filename
		bool loadFromFilename ( const QString& glTFFilename );

		// return the json array corresponding to 'key'
		QJsonArray getJsonArray ( const QString& key ) const;
		
		// load the glTF objects into arrays
		void loadScenes ();
		void loadNodes ();
		void loadMeshes ();
		void loadBuffers ();
		void loadBufferViews ();
		void loadAccessors ();
		void loadCameras ();
		void loadSkins ();
		void loadAnimations ();
		void loadMaterials ();
		void loadTextures ();
		void loadSamplers ();
		void loadImages ();
		void loadAll ();

		// getters
		const Scene&		getScene ( qsizetype idx ) const;
		const Node&			getNode ( qsizetype idx ) const;
		const Mesh&			getMesh ( qsizetype idx ) const;
		const Buffer&		getBuffer ( qsizetype idx ) const;
		const BufferView&	getBufferView ( qsizetype idx ) const;
		const Accessor&		getAccessor ( qsizetype idx ) const;
		const Camera&		getCamera ( qsizetype idx ) const;
		const Skin&			getSkin ( qsizetype idx ) const;
		const Animation&	getAnimation ( qsizetype idx ) const;
		const Material&		getMaterial ( qsizetype idx ) const;
		const Texture&		getTexture ( qsizetype idx ) const;
		const Sampler&		getSampler ( qsizetype idx ) const;
		const Image&		getImage ( qsizetype idx ) const;

		qsizetype sceneCount () const;
		qsizetype nodeCount () const;
		qsizetype meshCount () const;
		qsizetype bufferCount () const;
		qsizetype bufferViewCount () const;
		qsizetype accessorCount () const;
		qsizetype cameraCount () const;
		qsizetype skinCount () const;
		qsizetype animationCount () const;
		qsizetype materialCount () const;
		qsizetype textureCount () const;
		qsizetype samplerCount () const;
		qsizetype imageCount () const;

	private:
		QJsonDocument		m_document;

		QList<Scene>		m_scenes;
		QList<Node>			m_nodes;
		QList<Mesh>			m_meshes;
		QList<Buffer>		m_buffers;
		QList<BufferView>	m_bufferViews;
		QList<Accessor>		m_accessors;
		QList<Camera>		m_cameras;
		QList<Skin>			m_skins;
		QList<Animation>	m_animations;
		QList<Material>		m_materials;
		QList<Texture>		m_textures;
		QList<Sampler>		m_samplers;
		QList<Image>		m_images;
	};
}

#endif // !__GLTF_LOADER_H__



