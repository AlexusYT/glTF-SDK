//
// Created by alexus on 07.07.2025.
//

#include <GLTFSDK/Document.h>
#include <GLTFSDK/ExtensionHandlers.h>
#include <GLTFSDK/GLTF.h>

namespace Microsoft::glTF {
void glTFProperty::serializeExtensions( nlohmann::json& json) const {
    const auto registeredExtensionsVec = GetExtensions();

    if (extensions.empty() && registeredExtensionsVec.empty()) return;
    // Add registered extensions
    for (const auto& extension : registeredExtensionsVec) {
        const auto extensionPair = gltfDocument->serializer->Serialize(extension, *this, *gltfDocument);

        if (HasUnregisteredExtension(extensionPair.name))
            throw GLTFException("Registered extension '" + extensionPair.name + "' is also present as an unregistered extension.");

        if (!gltfDocument->extensionsUsed.contains(extensionPair.name))
            throw GLTFException("Registered extension '" + extensionPair.name + "' is not present in extensionsUsed");

        //TODO: validate the returned document against the extension schema!
        json[extensionPair.name] = extensionPair.value;
    }

    // Add unregistered extensions
    for (const auto& extension : this->extensions) {
        if (!gltfDocument->extensionsUsed.contains(extension.first))
            throw GLTFException("Unregistered extension '" + extension.first + "' is not present in extensionsUsed");

        //TODO: validate the returned document against the extension schema!
        json[extension.first] = extension.second;
    }
}

void glTFProperty::deserializeExtensions(const std::shared_ptr<ExtensionDeserializer> &pDeserializer) {
    if (!pDeserializer) return;
    std::unordered_map<std::string, nlohmann::json> filteredExtensions;
    for (auto &[name, value] : extensions) {
        if (pDeserializer->HasHandler(name, *this) ||
            pDeserializer->HasHandler(name)) {
            ExtensionPair extensionPair = { name, value };
            auto ext = pDeserializer->Deserialize(extensionPair, *this);
            if (auto prop = dynamic_cast<glTFProperty*>(ext.get())) {
                prop->setGltfDocument(gltfDocument);
            }
            SetExtension(std::move(ext));
        } else filteredExtensions.emplace(name, value);
    }
    extensions = std::move(filteredExtensions);
}

void BufferView::serialize(nlohmann::json &json) const {
    json["buffer"] = gltfDocument->buffers.GetIndex(bufferId);
    json["byteOffset"] = byteOffset;
    json["byteLength"] = byteLength;
    if (byteStride) json["byteStride"] = byteStride.Get();
    if (target) json["target"] = target.Get();
}

void BufferView::deserialize(const nlohmann::json &json) {

    bufferId = std::to_string(json.at("buffer").get<uint32_t>());
    byteOffset = json.value("byteOffset", 0);;
    json.at("byteLength").get_to(byteLength);

    if (auto itByteStride = json.find("byteStride"); itByteStride != json.end())
        byteStride = itByteStride.value().get<size_t>();

    // When target is not provided, the bufferView contains animation or skin data
    if (auto itTarget = json.find("target"); itTarget != json.end())
        target = static_cast<BufferViewTarget>(itTarget.value().get<uint32_t>()); }

void Accessor::Sparse::serialize(nlohmann::json &json) const {
    json["count"] = count;

    nlohmann::json indices;
    nlohmann::json values;

    indices["bufferView"] = gltfDocument->bufferViews.GetIndex(indicesBufferViewId);
    if (indicesByteOffset != 0U) indices["byteOffset"] = indicesByteOffset;

    indices["componentType"] = static_cast<uint32_t>(indicesComponentType);

    values["bufferView"]  = gltfDocument->bufferViews.GetIndex(valuesBufferViewId);
    if (valuesByteOffset != 0U) values["byteOffset"] = valuesByteOffset;

    json["count"] = count;
    json["indices"] = indices;
    json["values"] = values;

}

void Accessor::Sparse::deserialize(const nlohmann::json &json) {

    json.at("count").get_to(count);

    const auto& sparseIndicesMember = json.at("indices");
    const auto& sparseValuesMember = json.at("values");

    indicesBufferViewId = std::to_string(sparseIndicesMember.at("bufferView").get<uint32_t>());

    indicesComponentType = Accessor::GetComponentType(sparseIndicesMember.at("componentType").get<uint32_t>());
    indicesByteOffset = json.value("byteOffset", 0);

    valuesBufferViewId =  std::to_string(sparseValuesMember.at("bufferView").get<uint32_t>());
    valuesByteOffset = json.value("byteOffset", 0);
}

void Accessor::serialize(nlohmann::json &json) const {
    if (!bufferViewId.empty()) {
        json["bufferView"] = gltfDocument->bufferViews.GetIndex(bufferViewId);
    }
    if (byteOffset != 0) json["byteOffset"] = byteOffset;
    json["componentType"] = static_cast<size_t>(componentType);
    if (normalized) json["normalized"] = normalized;
    json["count"] = count;
    if (!min.empty()) json["min"] = min;
    json["type"] = GetAccessorTypeName(type);
    if (!max.empty()) json["max"] = max;
    if (sparse.count > 0U) json["sparse"] = sparse;
}

void Accessor::deserialize(const nlohmann::json &json) {

    if (auto iter = json.find("bufferView"); iter != json.end())
        bufferViewId = std::to_string(iter.value().get<uint32_t>());
    byteOffset = json.value("byteOffset", 0);
    componentType = GetComponentType(json.at("componentType").get<size_t>());
    normalized = json.value("normalized", false);
    count = json.at("count").get<size_t>();
    type = ParseType(json.at("type").get<std::string>());
    if (auto iter = json.find("min"); iter != json.end()) iter->get_to(min);
    if (auto iter = json.find("max"); iter != json.end()) iter->get_to(max);

    if (auto iter = json.find("sparse"); iter != json.end()) {
        iter.value().get_to(sparse);
    }

}

std::string InterpolationTypeToString(InterpolationType interpolationType)
{
    switch (interpolationType)
    {
        case INTERPOLATION_LINEAR:
            return INTERPOLATIONTYPE_NAME_LINEAR;
        case INTERPOLATION_STEP:
            return INTERPOLATIONTYPE_NAME_STEP;
        case INTERPOLATION_CUBICSPLINE:
            return INTERPOLATIONTYPE_NAME_CUBICSPLINE;
        default:
            return "";
    }
}

void MorphTarget::serialize(nlohmann::json &json) const {
    if (!positionsAccessorId.empty()) json[ACCESSOR_POSITION] = gltfDocument->accessors.GetIndex(positionsAccessorId);
    if (!normalsAccessorId.empty()) json[ACCESSOR_NORMAL] = gltfDocument->accessors.GetIndex(normalsAccessorId);
    if (!tangentsAccessorId.empty()) json[ACCESSOR_TANGENT] = gltfDocument->accessors.GetIndex(tangentsAccessorId);
}

void MeshPrimitive::serialize(nlohmann::json &json) const {
    if (!attributes.empty()) {
        nlohmann::json attributesJson;
        for (auto &attribute : attributes) {
            attributesJson[attribute.first] = gltfDocument->accessors.GetIndex(attribute.second);
        }
        json["attributes"] = attributesJson;
    }

    if (!indicesAccessorId.empty())
        json["indices"] = gltfDocument->accessors.GetIndex(indicesAccessorId);

    if (!materialId.empty())
        json["material"] = gltfDocument->materials.GetIndex(materialId);

    if (mode != MESH_TRIANGLES) json["mode"] = static_cast<uint32_t>(mode);

    if (!targets.empty()) json["targets"] = targets;
}

void TextureInfo::serialize(nlohmann::json &json) const {

    json["index"]= gltfDocument->textures.GetIndex(textureId);
    if (texCoord != 0) json["texCoord"] = texCoord;
}

void Texture::serialize(nlohmann::json &json) const {
    if (!samplerId.empty())
        json["sampler"] = gltfDocument->samplers.GetIndex(samplerId);
    if (!imageId.empty())
        json["source"] = gltfDocument->images.GetIndex(imageId);
}

void Image::serialize(nlohmann::json &json) const {

    if (uri.empty()) {
        if (bufferViewId.empty() || mimeType.empty())
            throw InvalidGLTFException("Invalid image: " + id + ". Images must have either a uri or a bufferView and a mimeType.");
    }
    else if (!bufferViewId.empty())
        throw InvalidGLTFException("Invalid image: " + id + ". Images can only have a uri or a bufferView, but not both.");

    if (!uri.empty()) json["uri"]= uri;
    if (!mimeType.empty()) json["mimeType"] = mimeType;
    if (!bufferViewId.empty()) json["bufferView"] = gltfDocument->bufferViews.GetIndex(bufferViewId);
}

void Node::serialize(nlohmann::json &json) const {
    if (!children.empty()) {
        nlohmann::json childrenJson;
        for (auto &child : children) {
            childrenJson.push_back(gltfDocument->nodes.GetIndex(child));
        }
        json["children"] = childrenJson;
    }
    if (!meshId.empty()) json["mesh"] = gltfDocument->meshes.GetIndex(meshId);
    if (!skinId.empty()) json["skin"] = gltfDocument->skins.GetIndex(skinId);
    if (!cameraId.empty()) json["camera"] = gltfDocument->cameras.GetIndex(cameraId);

    if (!HasValidTransformType())
        throw DocumentException("Node " + id + " doesn't have a valid transform type");


    if (GetTransformationType() == TRANSFORMATION_MATRIX)
        json["matrix"] = matrix;
    else if (GetTransformationType() == TRANSFORMATION_TRS) {
        if (translation != Vector3::ZERO) json["translation"] =translation;
        if (rotation != Quaternion::IDENTITY) json["rotation"] =rotation;
        if (scale != Vector3::ONE) json["scale"] =scale;
    }

    if (!weights.empty()) json["matrix"] = weights;

}

void Scene::serialize(nlohmann::json &json) const {
    if (!nodes.empty()) {
        nlohmann::json nodesJson;
        for (auto &node : nodes) {
            nodesJson.push_back(gltfDocument->nodes.GetIndex(node));
        }
        json["nodes"] = nodesJson;
    }
}

void AnimationTarget::serialize(nlohmann::json &json) const {
    json["node"] = gltfDocument->nodes.GetIndex(nodeId);
    json["path"] = TargetPathToString(path);
}

void AnimationSampler::serialize(nlohmann::json &json) const {

    json["input"] = getGltfDocument()->accessors.GetIndex(inputAccessorId);
    json["output"] = gltfDocument->accessors.GetIndex(outputAccessorId);
    if (auto str = InterpolationTypeToString(interpolation); !str.empty())
        json["interpolation"] = str;

}

void Skin::serialize(nlohmann::json &json) const {

    /*json["channels"] = pType.channels;*/
    if (!inverseBindMatricesAccessorId.empty()) {
        json["inverseBindMatrices"] = gltfDocument->accessors.GetIndex(inverseBindMatricesAccessorId);
    }
    if (!skeletonId.empty()) {
        json["skeleton"] = gltfDocument->nodes.GetIndex(skeletonId);
    }
    std::vector<uint32_t> joints;
    for (const auto & joint : jointIds) joints.emplace_back(gltfDocument->nodes.GetIndex(joint));
    json["joints"] = joints;
}
}
