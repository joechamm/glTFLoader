/*****************************************************************//**
 * \file   glTFNode.h
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
#ifndef __GLTF_NODE_H__
#define __GLTF_NODE_H__

#include <QJsonObject>
#include <QJsonArray>
#include <QList>

#include "glTFUtils.h"

namespace jcqt
{
	struct GLTFLOADER_EXPORT Node
	{
		qsizetype			m_cameraIdx = -1;
		qsizetype			m_skinIdx = -1;
		qsizetype			m_meshIdx = -1;
		mat4				m_matrix = mat4 ( 1.0f );
		QList<qsizetype>	m_children;
		QList<float>		m_weights;
		QJsonObject			m_jsonObject;
	};

	struct GLTFLOADER_EXPORT Scene
	{
		QList<qsizetype>	m_nodes;
		QJsonObject			m_jsonObject;
	};

	Node GLTFLOADER_EXPORT createNodeFromJson ( const QJsonObject& jsonObj )
	{
		Node node;
		if ( jsonObj.contains ( "camera" ) )
			node.m_cameraIdx = jsonObj.value ( "camera" ).toInteger ();

		if ( jsonObj.contains ( "skin" ) )
			node.m_skinIdx = jsonObj.value ( "skin" ).toInteger ();

		if ( jsonObj.contains ( "mesh" ) )
			node.m_meshIdx = jsonObj.value ( "mesh" ).toInteger ();

		if ( jsonObj.contains ( "matrix" ) )
		{
			node.m_matrix = createMat4FromJsonArray ( jsonObj.value ( "matrix" ).toArray () );
		}
		else
		{
			node.m_matrix = createMat4FromTRS ( jsonObj.value ( "translation" ).toArray (), jsonObj.value ( "rotation" ).toArray (), jsonObj.value ( "scale" ).toArray () );
		}

		if ( jsonObj.contains ( "children" ) )
		{
			getChildrenFromJsonArray ( jsonObj.value ( "children" ).toArray (), node.m_children );
		}

		if ( jsonObj.contains ( "weights" ) )
		{
			getWeightsFromJsonArray ( jsonObj.value ( "weights" ).toArray (), node.m_weights );
		}

		node.m_jsonObject = jsonObj;
		return node;
	}

	Scene GLTFLOADER_EXPORT createSceneFromJson ( const QJsonObject& jsonObj )
	{
		Scene scene;
		if ( jsonObj.contains ( "nodes" ) )
		{
			QJsonArray nodes = jsonObj.value ( "nodes" ).toArray ();
			qsizetype count = nodes.size ();
			scene.m_nodes.resize ( count );
			for ( qsizetype i = 0; i < count; i++ )
			{
				scene.m_nodes [ i ] = nodes.at ( i ).toInteger ();
			}
		}

		scene.m_jsonObject = jsonObj;
		return scene;
	}
}

#endif // !__GLTF_NODE_H__