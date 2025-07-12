// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <memory>
#include <GLTFSDK/ExtensionsKHR.h>

#include <GLTFSDK/Document.h>
#include <GLTFSDK/PropertyType.h>

using namespace Microsoft::glTF;

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

void KHR::Materials::PBRSpecularGlossiness::serialize(nlohmann::json &json, const PropertyType & pPropertyType) const {
    if (!pPropertyType.isMaterial()) return;
    nlohmann::to_json(json, static_cast<const glTFProperty&>(*this));
    if (diffuseFactor != Color4(1.0f, 1.0f, 1.0f, 1.0f))
        json["diffuseFactor"] = diffuseFactor;

    if (!diffuseTexture.textureId.empty())
        json["diffuseTexture"] = diffuseTexture;

    if (specularFactor != Color3(1.0f, 1.0f, 1.0f))
        json["specularFactor"] = specularFactor;

    if (glossinessFactor != 1.0)
        json["glossinessFactor"] = glossinessFactor;

    if (!specularGlossinessTexture.textureId.empty())
        json["specularGlossinessTexture"] = specularGlossinessTexture;
}

void KHR::Materials::PBRSpecularGlossiness::deserialize(const nlohmann::json &json) {

    nlohmann::from_json(json, static_cast<glTFProperty&>(*this));

    if (auto iter = json.find("diffuseFactor"); iter != json.end())
        iter.value().get_to(diffuseFactor);

    if (auto iter = json.find("diffuseTexture"); iter != json.end())
        iter.value().get_to(diffuseTexture);

    if (auto iter = json.find("specularFactor"); iter != json.end())
        iter.value().get_to(specularFactor);

    glossinessFactor = json.value("glossinessFactor", 1.0f);

    if (auto iter = json.find("specularGlossinessTexture"); iter != json.end())
        iter.value().get_to(specularGlossinessTexture);
}

std::unique_ptr<Extension> KHR::Materials::DeserializePBRSpecGloss(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    auto specGloss = std::make_unique<PBRSpecularGlossiness>();
    specGloss->deserialize(json);
    specGloss->deserializeExtensions(extensionDeserializer);
    return specGloss;
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

void KHR::Materials::Unlit::serialize(nlohmann::json &json, const PropertyType &pPropertyType) const {
    if (!pPropertyType.isMaterial()) return;
    nlohmann::to_json(json, static_cast<const glTFProperty&>(*this));
}

std::unique_ptr<Extension> KHR::Materials::DeserializeUnlit(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    auto unlit = std::make_unique<Unlit>();
    unlit->deserialize(json);
    unlit->deserializeExtensions(extensionDeserializer);
    return unlit;
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

void KHR::MeshPrimitives::DracoMeshCompression::serialize(nlohmann::json &json, const PropertyType & pPropertyType) const {
    if (!pPropertyType.isMeshPrimitive()) return;
    nlohmann::to_json(json, static_cast<const glTFProperty&>(*this));

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

void KHR::MeshPrimitives::DracoMeshCompression::deserialize(const nlohmann::json &json) {
    nlohmann::from_json(json, static_cast<glTFProperty&>(*this));

    if (auto iter = json.find("bufferView"); iter != json.end())
        bufferViewId = std::to_string(iter.value().get<uint32_t>());


    if (auto it = json.find("attributes"); it != json.end()) {
        if (!it.value().is_object())
            throw GLTFException("Member attributes of " + std::string(DRACOMESHCOMPRESSION_NAME) + " is not an object.");

        for (const auto& attribute : it.value().items()) {
            auto &name = attribute.key();
            if (!attribute.value().is_number_integer())
                throw GLTFException("Attribute " + std::string(name) + " of " + std::string(DRACOMESHCOMPRESSION_NAME) + " is not a number.");

            attributes.emplace(name, attribute.value().get<uint32_t>());
        }
    }
}

std::unique_ptr<Extension> KHR::MeshPrimitives::DeserializeDracoMeshCompression(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    auto extension = std::make_unique<DracoMeshCompression>();
    extension->deserialize(json);
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

void KHR::TextureInfos::TextureTransform::serialize(nlohmann::json &json, const PropertyType & pPropertyType) const {
    if (!pPropertyType.isTextureInfo() &&
        !pPropertyType.isType<Material::NormalTextureInfo>() &&
        !pPropertyType.isType<Material::OcclusionTextureInfo>()) return;

    nlohmann::to_json(json, static_cast<const glTFProperty&>(*this));
    if (offset != Vector2::ZERO) json["offset"] = offset;
    if (rotation != 0.0f) json["rotation"] = rotation;
    if (scale != Vector2::ONE) json["scale"] = scale;
    if (texCoord) json["texCoord"] = texCoord.Get();
}

void KHR::TextureInfos::TextureTransform::deserialize(const nlohmann::json &json) {
    nlohmann::from_json(json, static_cast<glTFProperty&>(*this));


    if (auto iter = json.find("offset"); iter != json.end()){
        auto &val = iter.value();
        if (!val.is_array())
            throw GLTFException("Offset member of " + std::string(TEXTURETRANSFORM_NAME) + " must be an array.");

        if (val.size() != 2)
            throw GLTFException("Offset member of " + std::string(TEXTURETRANSFORM_NAME) + " must have two values.");

        val.get_to(offset);
    }

    rotation = json.value("rotation", 0.0f);

    if (auto iter = json.find("scale"); iter != json.end()) {
        auto &val = iter.value();
        if (!val.is_array())
            throw GLTFException("Scale member of " + std::string(TEXTURETRANSFORM_NAME) + " must be an array.");

        if (val.size() != 2)
            throw GLTFException("Scale member of " + std::string(TEXTURETRANSFORM_NAME) + " must have two values.");

        val.get_to(scale);
    }

    if (auto iter = json.find("texCoord"); iter != json.end())
        texCoord = iter.value().get<size_t>();
}

std::unique_ptr<Extension> KHR::TextureInfos::DeserializeTextureTransform(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer)
{
    auto textureTransform = std::make_unique<TextureTransform>();
    textureTransform->deserialize(json);
    textureTransform->deserializeExtensions(extensionDeserializer);

    return textureTransform;
}
