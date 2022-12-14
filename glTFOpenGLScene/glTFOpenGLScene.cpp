#include "glTFOpenGLScene.h"

#include "../glTFLoader/glTFUtils.h"

#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_4_5_Core>

#include <QOpenGLTexture>

#include <glm/gtx/texture.hpp>

namespace jcqt
{
	QSharedPointer<QOpenGLContext> glTFOpenGLScene::s_context = QSharedPointer<QOpenGLContext> ( nullptr );

	glTFOpenGLScene::glTFOpenGLScene ()
		: m_loader(nullptr)
	{}

	glTFOpenGLScene::~glTFOpenGLScene ()
	{
		// TODO: implement
	}

	bool glTFOpenGLScene::loadBuffers ()
	{
		// first make sure we have everything we need
		if ( m_loader.isNull () || s_context.isNull () )
			return false;

		auto funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core> ( s_context.data () );

		qsizetype bufferViewCount = m_loader->bufferViewCount ();

		m_vbos.resize ( bufferViewCount );
		funcs->glCreateBuffers ( bufferViewCount, m_vbos.data () );

		for ( qsizetype i = 0; i < bufferViewCount; i++ )
		{
			const BufferView& buffView = m_loader->getBufferView ( i );
			const Buffer& buff = m_loader->getBuffer ( buffView.m_bufferIndex );

			GLsizei size = ( GLsizei ) buffView.m_byteLength;
			funcs->glNamedBufferStorage ( m_vbos [ i ], size, buff.m_data.constData () + buffView.m_byteOffset, 0 );
		}

		return true;
	}

	bool glTFOpenGLScene::loadTextures ()
	{
		// first make sure we have everything we need
		if ( m_loader.isNull () || s_context.isNull () )
			return false;

		auto funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core> ( s_context.data () );

		qsizetype texCount = m_loader->textureCount ();
		m_textures.resize ( texCount );

		funcs->glCreateTextures ( GL_TEXTURE_2D, texCount, m_textures.data () );

		for ( qsizetype i = 0; i < texCount; i++ )
		{
			GLuint handle = m_textures [ i ];

			const Texture& tex = m_loader->getTexture ( i );			

			if ( tex.m_samplerIdx != -1 )
			{
				const Sampler& sampler = m_loader->getSampler ( tex.m_samplerIdx );

				switch ( sampler.m_magFilter )
				{
				case eSamplerFilter::eSamplerFilter_NEAREST:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					break;
				default:
					break;
				}

				switch ( sampler.m_minFilter )
				{
				case eSamplerFilter::eSamplerFilter_NEAREST:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
					break;
				case eSamplerFilter::eSamplerFilter_NEAREST_MIPMAP_NEAREST:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR_MIPMAP_NEAREST:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_NEAREST_MIPMAP_LINEAR:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR_MIPMAP_LINEAR:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
					break;
				default:
					break;
				}

				switch ( sampler.m_wrapS )
				{
				case eSamplerWrapMode::eSamplerWrapMode_REPEAT:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_S, GL_REPEAT );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_CLAMP_TO_EDGE:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_MIRRORED_REPEAT:	
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
					break;
				default:
					break;
				}

				switch ( sampler.m_wrapT )
				{
				case eSamplerWrapMode::eSamplerWrapMode_REPEAT:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_T, GL_REPEAT );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_CLAMP_TO_EDGE:
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_MIRRORED_REPEAT:	
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
					break;
				default:
					break;
				}
			}

			if ( tex.m_sourceIdx != -1 )
			{
				const Image& source = m_loader->getImage ( tex.m_sourceIdx );
				const QImage::Format fmt = source.m_image.format ();

				GLenum intrnFmt;

				switch ( fmt )
				{
				case QImage::Format_RGB888:
					intrnFmt = GL_RGB8;
					break;				
				case QImage::Format_RGBA8888:
					intrnFmt = GL_RGBA8;
					break;
				case QImage::Format_RGBA32FPx4:
					intrnFmt = GL_RGBA32F;
					break;
				case QImage::Format_RGBX32FPx4:
					intrnFmt = GL_RGBA32F;
					break;
				case QImage::Format_RGBA16FPx4:
					intrnFmt = GL_RGBA16F;
					break;
				case QImage::Format_RGBX16FPx4:
					intrnFmt = GL_RGBA16F;
					break;
				default:
					intrnFmt = GL_RGBA8;
					break;
				}

				int w = source.m_image.width ();
				int h = source.m_image.height ();
				int mipLevels = glm::levels<2, int> ( glm::tvec2<int> ( w, h ) );

				funcs->glTextureParameteri ( handle, GL_TEXTURE_MAX_LEVEL, mipLevels );

				funcs->glTextureStorage2D ( handle, mipLevels, intrnFmt, (GLsizei)w, (GLsizei)h );	
			}
		}		
	}

	bool glTFOpenGLScene::loadVertexArrayObjects ()
	{
		// first make sure we have everything we need
		if ( m_loader.isNull () || s_context.isNull () )
			return false;

		auto funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core> ( s_context.data () );

		qsizetype meshCount = m_loader->meshCount ();

		qsizetype primCount = 0;

		for ( qsizetype i = 0; i < meshCount; i++ )
		{
			primCount += m_loader->getMesh ( i ).m_primitives.size ();
		}

		m_vaos.resize ( primCount );

		funcs->glCreateVertexArrays ( primCount, m_vaos.data () );

		qsizetype vaoidx = 0;

		for ( qsizetype i = 0; i < meshCount; i++ )
		{
			const Mesh& mesh = m_loader->getMesh ( i );

			primCount = mesh.m_primitives.size ();
			for ( qsizetype j = 0; j < primCount; j++ )
			{
				const MeshPrimitive& meshPrim = mesh.m_primitives [ j ];
				if ( meshPrim.m_indicesIdx != -1 )
				{
					const Accessor& accessor = m_loader->getAccessor ( meshPrim.m_indicesIdx );
					// If accessor type is not SCALAR, we throw an error per https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#reference-mesh-primitive
					if ( accessor.m_type != eAccessorType::eType_SCALAR )
					{
						// TODO: implement error handling

						return false;
					}

					// If the accessor bufferView is defined, grab the corresponding GPU buffer handle to set the ELEMENT_ARRAY buffer
					if ( accessor.m_bufferViewIndex != -1 )
					{
						const BufferView& buffView = m_loader->getBufferView ( accessor.m_bufferViewIndex );
						GLuint indexBufferHandle = m_vbos [ buffView.m_bufferIndex ];

						funcs->glVertexArrayElementBuffer ( m_vaos [ vaoidx ], indexBufferHandle );
					}
					else if ( accessor.m_sparse.m_indices.m_bufferViewIndex != -1 )
					{
						const BufferView& buffView = m_loader->getBufferView ( accessor.m_sparse.m_indices.m_bufferViewIndex );
						// Referenced buffer view MUST NOT have its target or byteStride properties defined. The buffer view and the optional byteOffset MUST be aligned to the componentType byte length per https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#reference-accessor-sparse-indices.
						if ( eBufferViewTarget::eBufferViewTarget_ARRAY_BUFFER == buffView.m_target || 
							eBufferViewTarget::eBufferViewTarget_ELEMENT_ARRAY_BUFFER == buffView.m_target ||
							buffView.m_byteStride != - 1)
						{
							// TODO: error handling
							return false;
						}

						GLuint indexBufferHandle = m_vbos [ buffView.m_bufferIndex ];

						funcs->glVertexArrayElementBuffer ( m_vaos [ vaoidx ], indexBufferHandle );
					}
				}

				const QJsonObject jsonAttribs = meshPrim.m_jsonObject.value ( "attributes" ).toObject ();
				QStringList attribKeys = jsonAttribs.keys ();
				for ( const auto& key : attribKeys )
				{
					qsizetype accessorIdx = jsonAttribs.value ( key ).toInteger();
					const Accessor& accessor = m_loader->getAccessor ( accessorIdx );

					GLenum type;
					GLboolean normalized;
					GLuint offset, bindIdx;
					GLint size;
					
					// If bufferView is undefined, byteOffset MUST NOT be defined per https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#reference-accessor

					// If the accessor buffer view is undefined, we'll check if the sparse values have been set, otherwise all values should be 0
					if ( -1 == accessor.m_bufferViewIndex )
					{
						accessor.m_sparse.m_values.m_bufferViewIndex;
						
						
					}
					
					
					GLenum type = (GLenum)accessor.m_type;
					GLboolean normalized = accessor.m_normalized;

					// TODO: determine how to calculate offset
					GLuint offset = 0;

					// TODO: need to map binding index correctly
					GLuint bindIdx = 0;

					// TODO: need to figure out how to correctly set size here 
					GLint size = 0;
					
					switch ( accessor.m_type )
					{
					case eAccessorType::eType_SCALAR:
						size = 1;
						break;
					case eAccessorType::eType_VEC2:
						size = 2;
						break;
					case eAccessorType::eType_VEC3:
						size = 3;
						break;
					case eAccessorType::eType_VEC4:
						size = 4;
						break;
					case eAccessorType::eType_MAT2:
						size = 4;
						break;
					case eAccessorType::eType_MAT3:
						size = 9;
						break;
					case eAccessorType::eType_MAT4:
						size = 16;
						break;
					default:
						break;
					}

					GLuint attribIdx = m_attributeIndexMap [ key ];					

					funcs->glEnableVertexArrayAttrib ( m_vaos [ vaoidx ], attribIdx );
					funcs->glVertexArrayAttribFormat ( m_vaos [ vaoidx ], attribIdx, size, type, normalized, offset );
					funcs->glVertexArrayAttribBinding ( m_vaos [ vaoidx ], attribIdx, bindIdx );

//					funcs->glVertexArrayVertexBuffer ( vaobj, bindingindex, buffer, offset, stride );
					
				}

				vaoidx++;
			}
			
		}
	}
}

