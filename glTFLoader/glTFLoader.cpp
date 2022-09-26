#include "glTFLoader.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>

namespace jcqt
{
	glTFLoader::glTFLoader ()
	{}

	glTFLoader::glTFLoader ( const QString & glTFFilename )
	{
		loadFromFilename ( glTFFilename );
	}
	
	glTFLoader::glTFLoader ( const QJsonDocument & glTFDoc )
		: m_document(glTFDoc)
	{}
	
	glTFLoader::glTFLoader ( const char* glTFFilename )
	{
		QString filename ( glTFFilename );
		loadFromFilename ( filename );
	}
	
	bool glTFLoader::hasDoc () const
	{
		return !m_document.isEmpty ();
	}

	bool glTFLoader::loadFromFilename ( const QString& glTFFilename )
	{
		// check the extension is gltf first
		QFileInfo fi ( glTFFilename );
		QString ext = fi.suffix ();
		if ( ext.compare ( "gltf" ) != 0 )
		{
			qWarning () << "Filename must use 'gltf' extension" << Qt::endl;
			return false;
		}

		QFile loadFile ( glTFFilename );

		if ( !loadFile.open ( QIODevice::ReadOnly ) )
		{
			qWarning () << "Couldn't open " << glTFFilename << Qt::endl;
			return false;
		}

		QByteArray data = loadFile.readAll ();

		QJsonParseError errParse;

		m_document = QJsonDocument::fromJson ( data, &errParse );

		if ( m_document.isNull () )
		{
			qWarning () << "Failed to parse JSON document from " << glTFFilename << Qt::endl << "JsonParseError: " << errParse.errorString () << Qt::endl;
			return false;
		}

		return true;
	}

	QJsonArray glTFLoader::getJsonArray ( const QString& key ) const
	{
		// TODO: implement error checking here
		return m_document.object ().value ( key ).toArray ();
	}

	void glTFLoader::loadScenes ()
	{
		QJsonArray scenes = getJsonArray ( "scenes" );
		qsizetype count = scenes.size ();
		if ( count > 0 )
		{
			m_scenes.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_scenes [ i ] = createSceneFromJson ( scenes.at ( i ).toObject () );
			}
		}
	}

	void glTFLoader::loadNodes ()
	{
		QJsonArray nodes = getJsonArray ( "nodes" );
		qsizetype count = nodes.size ();
		if ( count > 0 )
		{
			m_nodes.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_nodes [ i ] = createNodeFromJson ( nodes.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadMeshes ()
	{
		QJsonArray meshes = getJsonArray ( "meshes" );
		qsizetype count = meshes.size ();
		if ( count > 0 )
		{
			m_meshes.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_meshes [ i ] = createMeshFromJson ( meshes.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadBuffers ()
	{
		QJsonArray buffers = getJsonArray ( "buffers" );
		qsizetype count = buffers.size ();
		if ( count > 0 )
		{
			m_buffers.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_buffers [ i ] = createBufferFromJson ( buffers.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadBufferViews ()
	{
		QJsonArray bufferViews = getJsonArray ( "bufferViews" );
		qsizetype count = bufferViews.size ();
		if ( count > 0 )
		{
			m_bufferViews.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_bufferViews [ i ] = createBufferViewFromJson ( bufferViews.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadAccessors ()
	{
		QJsonArray accessors = getJsonArray ( "accessors" );
		qsizetype count = accessors.size ();
		if ( count > 0 )
		{
			m_accessors.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_accessors [ i ] = createAccessorFromJson ( accessors.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadCameras ()
	{
		QJsonArray cameras = getJsonArray ( "cameras" );
		qsizetype count = cameras.size ();
		if ( count > 0 )
		{
			m_cameras.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_cameras [ i ] = createCameraFromJson ( cameras.at ( i ).toObject () );
			}			
		}
	}
	
	void glTFLoader::loadSkins ()
	{
		QJsonArray skins = getJsonArray ( "skins" );
		qsizetype count = skins.size ();
		if ( count > 0 )
		{
			m_skins.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_skins [ i ] = createSkinFromJson ( skins.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadAnimations ()
	{
		QJsonArray animations = getJsonArray ( "animations" );
		qsizetype count = animations.size ();
		if ( count > 0 )
		{
			m_animations.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_animations [ i ] = createAnimationFromJson ( animations.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadMaterials ()
	{
		QJsonArray materials = getJsonArray ( "materials" );
		qsizetype count = materials.size ();
		if ( count > 0 )
		{
			m_materials.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_materials [ i ] = createMaterialFromJson ( materials.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadTextures ()
	{
		QJsonArray textures = getJsonArray ( "textures" );
		qsizetype count = textures.size ();
		if ( count > 0 )
		{
			m_textures.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_textures [ i ] = createTextureFromJson ( textures.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadSamplers ()
	{
		QJsonArray samplers = getJsonArray ( "samplers" );
		qsizetype count = samplers.size ();
		if ( count > 0 )
		{
			m_samplers.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_samplers [ i ] = createSamplerFromJson ( samplers.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadImages ()
	{
		QJsonArray images = getJsonArray ( "bufferViews" );
		qsizetype count = images.size ();
		if ( count > 0 )
		{
			m_images.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				m_images [ i ] = createImageFromJson ( images.at ( i ).toObject () );
			}
		}
	}
	
	void glTFLoader::loadAll ()
	{
		loadScenes ();
		loadNodes ();
		loadMeshes ();
		loadBuffers ();
		loadBufferViews ();
		loadAccessors ();
		loadCameras ();
		loadSkins ();
		loadAnimations ();
		loadMaterials ();
		loadTextures ();
		loadSamplers ();
		loadImages ();
	}
	
	Scene glTFLoader::getScene ( qsizetype idx ) const
	{
		return m_scenes [ idx ];
	}
	
	Node glTFLoader::getNode ( qsizetype idx ) const
	{
		return m_nodes [ idx ];
	}
	
	Mesh glTFLoader::getMesh ( qsizetype idx ) const
	{
		return m_meshes [ idx ];
	}
	
	Buffer glTFLoader::getBuffer ( qsizetype idx ) const
	{
		return m_buffers [ idx ];
	}
	
	BufferView glTFLoader::getBufferView ( qsizetype idx ) const
	{
		return m_bufferViews [ idx ];
	}
	
	Accessor glTFLoader::getAccessor ( qsizetype idx ) const
	{
		return m_accessors [ idx ];
	}
	
	Camera glTFLoader::getCamera ( qsizetype idx ) const
	{
		return m_cameras [ idx ];
	}
	
	Skin glTFLoader::getSkin ( qsizetype idx ) const
	{
		return m_skins [ idx ];
	}
	
	Animation glTFLoader::getAnimation ( qsizetype idx ) const
	{
		return m_animations [ idx ];
	}
	
	Material glTFLoader::getMaterial ( qsizetype idx ) const
	{
		return m_materials [ idx ];
	}
	
	Texture glTFLoader::getTexture ( qsizetype idx ) const
	{
		return m_textures [ idx ];
	}
	
	Sampler glTFLoader::getSampler ( qsizetype idx ) const
	{
		return m_samplers [ idx ];
	}
	
	Image glTFLoader::getImage ( qsizetype idx ) const
	{
		return m_images [ idx ];
	}

	qsizetype glTFLoader::sceneCount () const
	{
		return m_scenes.size ();
	}
	
	qsizetype glTFLoader::nodeCount () const
	{
		return m_nodes.size();
	}
	
	qsizetype glTFLoader::meshCount () const
	{
		return m_meshes.size();
	}
	
	qsizetype glTFLoader::bufferCount () const
	{
		return m_buffers.size();
	}
	
	qsizetype glTFLoader::bufferViewCount () const
	{
		return m_bufferViews.size ();
	}
	
	qsizetype glTFLoader::accessorCount () const
	{
		return m_accessors.size ();
	}
	
	qsizetype glTFLoader::cameraCount () const
	{
		return m_cameras.size ();
	}
	
	qsizetype glTFLoader::skinCount () const
	{
		return m_skins.size ();
	}
	
	qsizetype glTFLoader::animationCount () const
	{
		return m_animations.size ();
	}
	
	qsizetype glTFLoader::materialCount () const
	{
		return m_materials.size();
	}
	
	qsizetype glTFLoader::textureCount () const
	{
		return m_textures.size ();
	}
	
	qsizetype glTFLoader::samplerCount () const
	{
		return m_samplers.size ();
	}
	
	qsizetype glTFLoader::imageCount () const
	{
		return m_images.size ();
	}
}

