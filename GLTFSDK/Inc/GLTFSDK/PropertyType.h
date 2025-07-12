//
// Created by alexus on 12.07.2025.
//

#ifndef PROPERTYTYPE_H
#define PROPERTYTYPE_H
#include <typeindex>

#include "Document.h"


namespace Microsoft::glTF {
class PropertyType {
	std::type_index propertyType;

	struct glTFPropertyAll : glTFProperty
	{
		glTFPropertyAll() = delete;
	};
public:
	explicit PropertyType(const std::type_index &pPropertyType) : propertyType(pPropertyType) {}

	template<typename T>
	bool isType() const { return propertyType == typeid(T); }

	bool isAny() const { return propertyType == typeid(glTFPropertyAll); }

	bool isTextureInfo() const { return propertyType == typeid(TextureInfo); }

	bool isMeshPrimitive() const { return propertyType == typeid(MeshPrimitive); }

	bool isAsset() const { return propertyType == typeid(Asset); }

	bool isAccessor() const { return propertyType == typeid(Accessor); }

	bool isAnimation() const { return propertyType == typeid(Animation); }

	bool isBuffer() const { return propertyType == typeid(Buffer); }

	bool isBufferView() const { return propertyType == typeid(BufferView); }

	bool isCamera() const { return propertyType == typeid(Camera); }

	bool isImage() const { return propertyType == typeid(Image); }

	bool isMaterial() const { return propertyType == typeid(Material); }

	bool isMesh() const { return propertyType == typeid(Mesh); }

	bool isNode() const { return propertyType == typeid(Node); }

	bool isSampler() const { return propertyType == typeid(Sampler); }

	bool isScene() const { return propertyType == typeid(Scene); }

	bool isSkin() const { return propertyType == typeid(Skin); }

	bool isTexture() const { return propertyType == typeid(Texture); }

	bool isDocument() const { return propertyType == typeid(Document); }


};
}


#endif //PROPERTYTYPE_H