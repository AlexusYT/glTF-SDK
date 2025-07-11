// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <memory>
#include <GLTFSDK/ExtensionsKHR.h>

#include <GLTFSDK/Document.h>

using namespace Microsoft::glTF;

std::shared_ptr<ExtensionSerializer> KHR::GetKHRExtensionSerializer()
{
    using namespace Materials;
    using namespace MeshPrimitives;
    using namespace TextureInfos;

    auto extensionSerializer = std::make_shared<ExtensionSerializer>();
    extensionSerializer->AddHandler<PBRSpecularGlossiness, Material>(PBRSPECULARGLOSSINESS_NAME, SerializePBRSpecGloss);
    extensionSerializer->AddHandler<Unlit, Material>(UNLIT_NAME, SerializeUnlit);
    extensionSerializer->AddHandler<DracoMeshCompression, MeshPrimitive>(DRACOMESHCOMPRESSION_NAME, SerializeDracoMeshCompression);
    extensionSerializer->AddHandler<TextureTransform, TextureInfo>(TEXTURETRANSFORM_NAME, SerializeTextureTransform);
    extensionSerializer->AddHandler<TextureTransform, Material::NormalTextureInfo>(TEXTURETRANSFORM_NAME, SerializeTextureTransform);
    extensionSerializer->AddHandler<TextureTransform, Material::OcclusionTextureInfo>(TEXTURETRANSFORM_NAME, SerializeTextureTransform);
    return extensionSerializer;
}

std::shared_ptr<ExtensionDeserializer> KHR::GetKHRExtensionDeserializer()
{
    using namespace Materials;
    using namespace MeshPrimitives;
    using namespace TextureInfos;

    auto extensionDeserializer = std::make_shared<ExtensionDeserializer>();
    extensionDeserializer->AddHandler<PBRSpecularGlossiness, Material>(PBRSPECULARGLOSSINESS_NAME, DeserializePBRSpecGloss);
    extensionDeserializer->AddHandler<Unlit, Material>(UNLIT_NAME, DeserializeUnlit);
    extensionDeserializer->AddHandler<DracoMeshCompression, MeshPrimitive>(DRACOMESHCOMPRESSION_NAME, DeserializeDracoMeshCompression);
    extensionDeserializer->AddHandler<TextureTransform, TextureInfo>(TEXTURETRANSFORM_NAME, DeserializeTextureTransform);
    extensionDeserializer->AddHandler<TextureTransform, Material::NormalTextureInfo>(TEXTURETRANSFORM_NAME, DeserializeTextureTransform);
    extensionDeserializer->AddHandler<TextureTransform, Material::OcclusionTextureInfo>(TEXTURETRANSFORM_NAME, DeserializeTextureTransform);
    return extensionDeserializer;
}

// KHR::Materials::PBRSpecularGlossiness

KHR::Materials::PBRSpecularGlossiness::PBRSpecularGlossiness() :
    diffuseFactor(1.0f, 1.0f, 1.0f, 1.0f),
    specularFactor(1.0f, 1.0f, 1.0f),
    glossinessFactor(1.0f)
{
}

std::unique_ptr<Extension> KHR::Materials::PBRSpecularGlossiness::Clone() const
{
    return std::make_unique<PBRSpecularGlossiness>(*this);
}

bool KHR::Materials::PBRSpecularGlossiness::IsEqual(const Extension& rhs) const
{
    const auto other = dynamic_cast<const PBRSpecularGlossiness*>(&rhs);

    return other != nullptr
        && glTFProperty::Equals(*this, *other)
        && this->diffuseFactor == other->diffuseFactor
        && this->diffuseTexture == other->diffuseTexture
        && this->specularFactor == other->specularFactor
        && this->glossinessFactor == other->glossinessFactor
        && this->specularGlossinessTexture == other->specularGlossinessTexture;
}

nlohmann::json KHR::Materials::SerializePBRSpecGloss(const Materials::PBRSpecularGlossiness& specGloss, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer)  {
    return specGloss;
}

std::unique_ptr<Extension> KHR::Materials::DeserializePBRSpecGloss(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    Materials::PBRSpecularGlossiness specGloss;
    json.get_to(specGloss);
    specGloss.deserializeExtensions(extensionDeserializer);
    return std::make_unique<PBRSpecularGlossiness>(specGloss);
}

// KHR::Materials::Unlit

std::unique_ptr<Extension> KHR::Materials::Unlit::Clone() const
{
    return std::make_unique<Unlit>(*this);
}

bool KHR::Materials::Unlit::IsEqual(const Extension& rhs) const
{
    return dynamic_cast<const Unlit*>(&rhs) != nullptr;
}

nlohmann::json KHR::Materials::SerializeUnlit(const Materials::Unlit &extension, const Document &gltfDocument,
                                              const ExtensionSerializer &extensionSerializer){
    return extension;
}

std::unique_ptr<Extension> KHR::Materials::DeserializeUnlit(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    Unlit unlit;
    json.get_to(unlit);
    unlit.deserializeExtensions(extensionDeserializer);
    return std::make_unique<Unlit>(unlit);
}

// KHR::MeshPrimitives::DracoMeshCompression

std::unique_ptr<Extension> KHR::MeshPrimitives::DracoMeshCompression::Clone() const
{
    return std::make_unique<DracoMeshCompression>(*this);
}

bool KHR::MeshPrimitives::DracoMeshCompression::IsEqual(const Extension& rhs) const
{
    const auto other = dynamic_cast<const DracoMeshCompression*>(&rhs);

    return other != nullptr
        && glTFProperty::Equals(*this, *other)
        && this->bufferViewId == other->bufferViewId
        && this->attributes == other->attributes;
}

void KHR::MeshPrimitives::DracoMeshCompression::serialize(nlohmann::json &json) const {

    if (!bufferViewId.empty())
        json["bufferView"] = gltfDocument->bufferViews.GetIndex(bufferViewId);

    if (!attributes.empty()) {
        nlohmann::json attributesValue;
        for (const auto& attribute : attributes){
            attributesValue[attribute.first] = attribute.second;
        }

        json["attributes"] = attributesValue;
    }

}

nlohmann::json KHR::MeshPrimitives::SerializeDracoMeshCompression(
    const MeshPrimitives::DracoMeshCompression &dracoMeshCompression, const Document &glTFdoc,
    const ExtensionSerializer &extensionSerializer) {
    return dracoMeshCompression;
}

std::unique_ptr<Extension> KHR::MeshPrimitives::DeserializeDracoMeshCompression(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    auto extension = std::make_unique<DracoMeshCompression>();
    json.get_to(*extension);
    extension->deserializeExtensions(extensionDeserializer);
    return extension;
}

// KHR::TextureInfos::TextureTransform

KHR::TextureInfos::TextureTransform::TextureTransform() :
    offset(Vector2::ZERO),
    rotation(0.0f),
    scale(Vector2::ONE),
    texCoord()
{
}

KHR::TextureInfos::TextureTransform::TextureTransform(const TextureTransform& textureTransform) :
    offset(textureTransform.offset),
    rotation(textureTransform.rotation),
    scale(textureTransform.scale),
    texCoord(textureTransform.texCoord)
{
}

std::unique_ptr<Extension> KHR::TextureInfos::TextureTransform::Clone() const
{
    return std::make_unique<TextureTransform>(*this);
}

bool KHR::TextureInfos::TextureTransform::IsEqual(const Extension& rhs) const
{
    const auto other = dynamic_cast<const TextureTransform*>(&rhs);

    return other != nullptr
        && glTFProperty::Equals(*this, *other)
        && this->offset == other->offset
        && this->rotation == other->rotation
        && this->scale == other->scale
        && this->texCoord == other->texCoord;
}

nlohmann::json KHR::TextureInfos::SerializeTextureTransform(const TextureTransform &textureTransform,
                                                            const Document &gltfDocument,
                                                            const ExtensionSerializer &extensionSerializer){

    return textureTransform;
}

std::unique_ptr<Extension> KHR::TextureInfos::DeserializeTextureTransform(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    TextureTransform textureTransform;
    json.get_to(textureTransform);
    textureTransform.deserializeExtensions(extensionDeserializer);

    return std::make_unique<TextureTransform>(textureTransform);
}
