// Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// -- Godot Engine <https://godotengine.org>

// Major portions from the main source file of fbx2glTF.
// See licenses.

// clang-format off
#include <Reference.hpp>
#include <Godot.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ComChibifireAssImpImporter.h"
#include <Animation.hpp>
#include <Directory.hpp>
#include <EditorSceneImporter.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Ref.hpp>
#include <EditorFileSystem.hpp>
#include <Vector3.hpp>
#include <Object.hpp>
#include <Node.hpp>
#include <Spatial.hpp>
#include <AnimationPlayer.hpp>
// clang-format on

bool verboseOutput = false;

using namespace godot;
using godot::Directory;
using godot::Ref;

Array ComChibifireAssImpImporter::get_extensions() const {
	PoolStringArray arr;
	arr.push_back("fbx");
	arr.push_back("blend");
	return Array(arr);
}

int64_t ComChibifireAssImpImporter::get_import_flags() const {
	return IMPORT_SCENE | IMPORT_ANIMATION;
}

Node *ComChibifireAssImpImporter::import_scene(const String path, const int64_t flags, const int64_t bake_fps) {
	Assimp::Importer importer;
	const String ass_file = ProjectSettings::globalize_path(path);
	const aiScene *scene = importer.ReadFile(ass_file.alloc_c_string(),
			0);
	ERR_FAIL_COND_V(!scene, nullptr);
	Assimp::Exporter exporter;
	const String file = path.get_basename().get_file() + "-" + path.md5_text() + String(".glb");
	const String gltf_path = String("res://.import/").plus_file(file);
	const String gltf_global = ProjectSettings::globalize_path(gltf_path);
	Assimp::ExportProperties properties;
	if (path.get_extension() == "fbx") {
		properties.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 0.01f);
	}
	exporter.Export(scene, "glb2", gltf_global.alloc_c_string(), aiProcess_GlobalScale, &properties);
	Spatial *custom = (Spatial *)owner->import_scene_from_other_importer(gltf_path, flags, bake_fps);
	if (path.get_extension() == "fbx" || path.get_extension() == "blend") {
		custom->set_rotation_degrees(custom->get_rotation_degrees() + Vector3(-90.0f, 0.0f, 0.0f));
	}
	return custom;
}

Ref<Animation> ComChibifireAssImpImporter::import_animation(const String path, const int64_t flags, const int64_t bake_fps) {
	return Ref<Animation>();
}

void ComChibifireAssImpImporter::_register_methods() {
	register_method("_get_extensions", &ComChibifireAssImpImporter::get_extensions);
	register_method("_get_import_flags", &ComChibifireAssImpImporter::get_import_flags);
	register_method("_import_scene", &ComChibifireAssImpImporter::import_scene);
	register_method("_import_animation", &ComChibifireAssImpImporter::import_animation);
}
