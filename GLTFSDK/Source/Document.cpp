// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <GLTFSDK/Document.h>

using namespace Microsoft::glTF;

Document::Document() = default;

Document::Document(Asset&& asset) : asset(std::move(asset))
{
}

bool Document::IsExtensionUsed(const std::string& extension) const
{
    return extensionsUsed.find(extension) != extensionsUsed.end();
}

bool Document::IsExtensionRequired(const std::string& extension) const
{
    return extensionsRequired.find(extension) != extensionsRequired.end();
}

bool Document::HasDefaultScene() const
{
    return !defaultSceneId.empty();
}

const Scene& Document::GetDefaultScene() const
{
    // According to the 2.0 spec: 
    //      "The glTF asset contains ZERO or more scenes"
    //      "When scene is undefined, runtime is not required to render anything at load time."
    // If there's no default defined, return the first scene, if there is one.
    if (HasDefaultScene())
    {
        return scenes[defaultSceneId];
    }

    if (!scenes.Elements().empty())
    {
        return scenes.Elements().front();
    }

    throw DocumentException("Default scene " + defaultSceneId + " was not found.");
}

const Scene& Document::SetDefaultScene(Scene&& scene, AppendIdPolicy policy)
{
    const auto& defaultScene = scenes.Append(std::move(scene), policy);
    defaultSceneId = defaultScene.id;
    return defaultScene;
}

void Document::serialize(nlohmann::json &json) const {
    json["asset"] = asset;
    if (accessors.Size()>0) json["accessors"] = accessors;
    if (animations.Size()>0) json["animations"] = animations;
    if (bufferViews.Size()>0) json["bufferViews"] = bufferViews;
    if (buffers.Size()>0) json["buffers"] = buffers;
    if (cameras.Size()>0) json["cameras"] = cameras;
    if (images.Size()>0) json["images"] = images;
    if (materials.Size()>0) json["materials"] = materials;
    if (meshes.Size()>0) json["meshes"] = meshes;
    if (nodes.Size()>0) json["nodes"] = nodes;
    if (samplers.Size()>0) json["samplers"] = samplers;
    if (scenes.Size()>0) json["scenes"] = scenes;
    if (skins.Size()>0) json["skins"] = skins;
    if (textures.Size()>0) json["textures"] = textures;
    if (HasDefaultScene()) json["scene"] = scenes.GetIndex(defaultSceneId);
    if (!gltfDocument->extensionsUsed.empty()) json["extensionsUsed"] = gltfDocument->extensionsUsed;
    for (auto& extensionName : gltfDocument->extensionsRequired) {
        if (!gltfDocument->extensionsUsed.contains(extensionName))
            throw GLTFException("required extension '" + extensionName + "' not present in extensionsUsed.");
    }
    if (!gltfDocument->extensionsRequired.empty()) json["extensionsRequired"] = gltfDocument->extensionsRequired;
}

bool Document::operator==(const Document& rhs) const
{
    return this->asset == rhs.asset
        && this->accessors == rhs.accessors
        && this->animations == rhs.animations
        && this->buffers == rhs.buffers
        && this->bufferViews == rhs.bufferViews
        && this->cameras == rhs.cameras
        && this->images == rhs.images
        && this->materials == rhs.materials
        && this->meshes == rhs.meshes
        && this->nodes == rhs.nodes
        && this->samplers == rhs.samplers
        && this->scenes == rhs.scenes
        && this->skins == rhs.skins
        && this->textures == rhs.textures
        && this->extensionsUsed == rhs.extensionsUsed
        && this->extensionsRequired == rhs.extensionsRequired
        && this->defaultSceneId == rhs.defaultSceneId
        && glTFProperty::Equals(*this, rhs);
}
