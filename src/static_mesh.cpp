#include "static_mesh.h"

bool StaticMesh::loadFromFile(const std::string& file_name) {
    homo_meshes_.clear();

    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(file_name.c_str(),
                                    aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                    aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | 
                                    aiProcess_ValidateDataStructure);

    if (!scene) {
        fprintf(stderr, "Error parsing '%s': '%s'\n", file_name.c_str(), Importer.GetErrorString());
        return false;
    }

    homo_meshes_.resize(scene->mNumMeshes);
    textures_.resize(scene->mNumMaterials);

    printf("num materials = %d\n", scene->mNumMaterials);

    for (int i = 0; i < static_cast<int>(scene->mNumMaterials); ++i) {
		printf("diffuse textures num = %d\n", scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE));

		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;

            if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            	printf("path = %s\n", path.C_Str());

            	if (!textures_[i].loadFromFile(std::string("models/island/") + std::string(path.C_Str()))) {
            		fprintf(stderr, "Failed loading texture '%s' for matirial #%d\n", path.C_Str(), i);
	            	exit(1);
            	}
            }
		} else {
//			textures_[i].loadFromFile("");
		}
    }

    for (int mesh_index = 0; mesh_index < static_cast<int>(homo_meshes_.size()); ++mesh_index) {
        const aiMesh* ai_mesh = scene->mMeshes[mesh_index];
        homo_meshes_[mesh_index].material_index_ = ai_mesh->mMaterialIndex;

        printf("has texture coords? %d\n", ai_mesh->HasTextureCoords(0));

        const aiVector3D zero(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < static_cast<int>(ai_mesh->mNumVertices); ++i){
            const aiVector3D* pos    = &(ai_mesh->mVertices[i]);
            const aiVector3D* normal = &(ai_mesh->mNormals[i]);
            const aiVector3D* uv_pos = ai_mesh->HasTextureCoords(0) ? &(ai_mesh->mTextureCoords[0][i]) : &zero;

            homo_meshes_[mesh_index].vertices.push_back(StaticVertex(
                glm::vec3(pos->x, pos->y, pos->z),
                glm::vec2(uv_pos->x, uv_pos->y),
                glm::vec3(normal->x, normal->y, normal->z)
            ));
        }

        for (int i = 0; i < static_cast<int>(ai_mesh->mNumFaces); ++i){
            const aiFace& face = ai_mesh->mFaces[i];

            homo_meshes_[mesh_index].indices.push_back(glm::uvec3(
                face.mIndices[0], 
                face.mIndices[1], 
                face.mIndices[2]
            ));
        }

        glGenBuffers(1, &(homo_meshes_[mesh_index].VBO));
        glBindBuffer(GL_ARRAY_BUFFER, homo_meshes_[mesh_index].VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(StaticVertex) * homo_meshes_[mesh_index].vertices.size(),
                    &(homo_meshes_[mesh_index].vertices[0].position[0]), GL_STATIC_DRAW);

        glGenBuffers(1, &(homo_meshes_[mesh_index].IBO));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, homo_meshes_[mesh_index].IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * homo_meshes_[mesh_index].indices.size(),
                    &(homo_meshes_[mesh_index].indices[0][0]), GL_STATIC_DRAW);
    }

    findEye();

    return true;
}

void StaticMesh::findEye() {
    eye_.homo_index = 0;
    eye_.vertex_index = 0;
    int cnt = 0;
    for (int i = 0; i < static_cast<int>(homo_meshes_.size()); ++i) {
        for (int j = 0; j < static_cast<int>(homo_meshes_[i].vertices.size()); ++j) {
            if (glm::length(homo_meshes_[i].vertices[j].position) < 
                glm::length(homo_meshes_[eye_.homo_index].vertices[eye_.vertex_index].position)) {

                eye_.homo_index = i;
                eye_.vertex_index = j;
            }
            ++cnt;
        }
    }
    printf("model size = %d\n", cnt);
}

void StaticMesh::render() const {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    for (int i = 0; i < static_cast<int>(homo_meshes_.size()); ++i) {
        glBindBuffer(GL_ARRAY_BUFFER, homo_meshes_[i].VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (const GLvoid*) 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (const GLvoid*) 12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (const GLvoid*) 20);

        textures_[homo_meshes_[i].material_index_].bindToUnit(GL_TEXTURE0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, homo_meshes_[i].IBO);
        glDrawElements(GL_PATCHES, homo_meshes_[i].indices.size() * 3, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}
