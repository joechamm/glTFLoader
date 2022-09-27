#include "glTFOpenGLScene.h"

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

		qsizetype bufferCount = m_loader->bufferCount ();
		m_vbos.resize ( bufferCount );
		
		funcs->glCreateBuffers ( bufferCount, m_vbos.data () );

		for ( qsizetype i = 0; i < bufferCount; i++ )
		{
			const Buffer& buff = m_loader->getBuffer ( i );
			qsizetype bufferSize = buff.m_data.size ();
			GLuint handle = m_vbos [ i ];
			funcs->glNamedBufferStorage ( handle, bufferSize, buff.m_data.constData (), 0);
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
	//				m_textures [ i ].setMagnificationFilter ( QOpenGLTexture::Nearest );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR:
	//				m_textures [ i ].setMagnificationFilter ( QOpenGLTexture::Linear );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					break;
				default:
					break;
				}

				switch ( sampler.m_minFilter )
				{
				case eSamplerFilter::eSamplerFilter_NEAREST:
	//				m_textures [ i ].setMinificationFilter ( QOpenGLTexture::Nearest );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR:
	//				m_textures [ i ].setMinificationFilter ( QOpenGLTexture::Linear );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
					break;
				case eSamplerFilter::eSamplerFilter_NEAREST_MIPMAP_NEAREST:
	//				m_textures [ i ].setMinificationFilter ( QOpenGLTexture::NearestMipMapNearest );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR_MIPMAP_NEAREST:
	//				m_textures [ i ].setMinificationFilter ( QOpenGLTexture::LinearMipMapNearest );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
					break;
				case eSamplerFilter::eSamplerFilter_NEAREST_MIPMAP_LINEAR:
	//				m_textures [ i ].setMinificationFilter ( QOpenGLTexture::NearestMipMapLinear );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
					break;
				case eSamplerFilter::eSamplerFilter_LINEAR_MIPMAP_LINEAR:
	//				m_textures [ i ].setMinificationFilter ( QOpenGLTexture::LinearMipMapLinear );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
					break;
				default:
					break;
				}

				switch ( sampler.m_wrapS )
				{
				case eSamplerWrapMode::eSamplerWrapMode_REPEAT:
	//				m_textures [ i ].setWrapMode ( QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_S, GL_REPEAT );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_CLAMP_TO_EDGE:
	//				m_textures [ i ].setWrapMode ( QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_MIRRORED_REPEAT:
	//				m_textures [ i ].setWrapMode ( QOpenGLTexture::DirectionS, QOpenGLTexture::MirroredRepeat );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
					break;
				default:
					break;
				}

				switch ( sampler.m_wrapT )
				{
				case eSamplerWrapMode::eSamplerWrapMode_REPEAT:
	//				m_textures [ i ].setWrapMode ( QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_T, GL_REPEAT );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_CLAMP_TO_EDGE:
	//				m_textures [ i ].setWrapMode ( QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge );
					funcs->glTextureParameteri ( handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
					break;
				case eSamplerWrapMode::eSamplerWrapMode_MIRRORED_REPEAT:
	//				m_textures [ i ].setWrapMode ( QOpenGLTexture::DirectionT, QOpenGLTexture::MirroredRepeat );
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
					break;
				}

				int w = source.m_image.width ();
				int h = source.m_image.height ();
				int mipLevels = glm::levels<2, int> ( glm::tvec2<int> ( w, h ) );

				funcs->glTextureParameteri ( handle, GL_TEXTURE_MAX_LEVEL, mipLevels );

				funcs->glTextureStorage2D ( handle, mipLevels, intrnFmt, (GLsizei)w, (GLsizei)h );

	//			m_textures [ i ].setData ( source.m_image, QOpenGLTexture::GenerateMipMaps );			
			}
		}
		
	}
}

