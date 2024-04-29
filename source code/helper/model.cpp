#include "model.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../helper/stb/stb_image.h"

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// position,normal，textoord
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
		{
			glm::vec2 vec;
			//texcoords
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		//// tangent
		//if (mesh->mTangents)
		//{
		//	vector.x = mesh->mTangents[i].x;
		//	vector.y = mesh->mTangents[i].y;
		//	vector.z = mesh->mTangents[i].z;
		//	vertex.Tangent = vector;
		//}
		//// bitangent
		//if (mesh->mBitangents) {
		//	vector.x = mesh->mBitangents[i].x;
		//	vector.y = mesh->mBitangents[i].y;
		//	vector.z = mesh->mBitangents[i].z;
		//	vertex.Bitangent = vector;
		//}
		vertices.push_back(vertex);

	}

	// process index
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// 处理材质
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
		}
	}
	return textures;
}


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Model::Draw(DirLight& light, Camera& camera,GLSLProgram& shader, glm::vec3 target)
{
	shader.use();
	transform = glm::scale(glm::mat4(1.0), Scale);
	transform[3] = glm::vec4(Position, 1.0f);
	shader.setUniform("model", transform);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800/(float)600, 0.1f, 10000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.setUniform("projection", projection);
	shader.setUniform("view", view);
	shader.setUniform("viewPos", camera.Position);

	GLfloat near_plane = 1.0f, far_plane = 70.f;
	glm::mat4 Lprojection = glm::ortho(-36.0f, 36.0f, -36.0f, 36.0f, near_plane, far_plane);
	glm::mat4 Lview = glm::lookAt(target - 3.f*light.Direction, target, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setUniform("lightSpaceMatrix", Lprojection* Lview);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
		
}

void Model::DrawShadow(DirLight& light, GLSLProgram& shader, bool isPhysicalObject, glm::vec3 target)
{
	shader.use();
	if (!isPhysicalObject) {
		transform = glm::scale(glm::mat4(1.0), Scale);
		transform[3] = glm::vec4(Position, 1.0f);
	}
	
	shader.setUniform("model", transform);

	GLfloat near_plane = 1.0f, far_plane = 70.f;
	glm::mat4 projection = glm::ortho(-36.0f, 36.0f, -36.0f, 36.0f, near_plane, far_plane);
	glm::mat4 Lview = glm::lookAt(target - 3.f * light.Direction, target, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setUniform("view", Lview);
	shader.setUniform("projection", projection);
	for (unsigned int i = 0; i < meshes.size(); i++)
		//meshes[i].Draw(shader);
		meshes[i].Draw_depth(shader);
}

// This function is called when you want to draw the physical object's shadow,
// we don't compute phtsics here
void PhysicalModel::DrawShadow(DirLight& light, GLSLProgram& renderShader, bool isPhysicalObject, glm::vec3 target) {
	Model::DrawShadow(light, renderShader, true, target);
}

void PhysicalModel::Draw(DirLight& light, Camera& camera, GLSLProgram& physicalShader, GLSLProgram& renderShader)
{
	physicalShader.use();
	transform = glm::mat4(rotation);
	transform[3] = glm::vec4(Position, 1.0f);
	physicalShader.setUniform("gravity", gravity);
	physicalShader.setUniform("model", transform);
	force += glm::vec3(-damping_stiffness_byair*velocity[0], -damping_stiffness_byair * velocity[1], -damping_stiffness_byair * velocity[2]);
	physicalShader.setUniform("totalforce", force);
	physicalShader.setUniform("collision_stiffness", collision_stiffness);
	physicalShader.setUniform("damping_stiffness_byground", damping_stiffness_byground);

	for (unsigned int i = 0; i < meshes.size(); i++) {

		glEnable(GL_RASTERIZER_DISCARD);
		for (int iter = 0; iter < 10; iter++) {
			// there are things to do before you can draw a physical mesh
			// 1. make sure before you call this draw function, the meshes's vao, vbo, ebo are already set up (lucky you have done so)
			// 2. use a vertex shader to compute the force and torque for each particle using transfrom feedback and get the data
			// 3. compute the whole attributes for the body, like velocity, angular velocity...
			// 4. use the computed attributes to update the vertex position, remember we store the original vertex position in meshsh[i].vertices[j].Position
			// 5. for steps 2-4, we compute many times with small time step
			// 6. finally we can draw the mesh
			// ...
			physicalShader.setUniform("cm_position", cm_postion);
			physicalShader.setUniform("velocity", velocity);

			glBindVertexArray(temp_vaos[i]);
			glBeginTransformFeedback(GL_TRIANGLES); 
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshes[i].indices.size()), GL_UNSIGNED_INT, 0); // 执行顶点着色器计算
			glEndTransformFeedback();

			glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, feedbackBuffers[i]);
			float* feedbackData = (float*)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_ONLY);
			
			force = { 0,0,0 };
			torque = { 0,0,0 };
			for (int j = 0; j < meshes[i].vertices.size(); j+=6) {
				force += glm::vec3(feedbackData[j], feedbackData[j + 1], feedbackData[j + 2]);
				
				torque += glm::vec3(feedbackData[j + 3], feedbackData[j + 4], feedbackData[j + 5]);
			}
			glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
			glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

			velocity += (force / body_mass * (1/dt_inv));
			cm_postion += velocity * (1/dt_inv);

			glm::quat d_p = quat_multi(glm::quat(0, angular_velocity[0], angular_velocity[1], angular_velocity[2]), rotation_quat) * 0.5f;
			rotation_quat += d_p * (1/dt_inv);

			rotation_quat = glm::normalize(rotation_quat);
			rotation = quat_to_mat(rotation_quat);
			angular_momentum = torque * (1/dt_inv);
			inverse_inertia = rotation * origin_inverse_inertia * glm::transpose(rotation);

			angular_velocity = inverse_inertia * angular_momentum;

			transform = glm::mat4(rotation);
			transform[3] = glm::vec4(cm_postion - origin_cm_position + Position, 1.0f);
			physicalShader.setUniform("model", transform);
		}
		glDisable(GL_RASTERIZER_DISCARD);
		force = { 0,0,0 };
		current_position = cm_postion - origin_cm_position + Position;
		current_velocity = velocity;
		// now we draw the mesh
		renderShader.use();
		renderShader.setUniform("model", transform);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		renderShader.setUniform("projection", projection);
		renderShader.setUniform("view", view);
		renderShader.setUniform("viewPos", camera.Position);
		GLfloat near_plane = 1.0f, far_plane = 70.f;
		glm::mat4 Lprojection = glm::ortho(-24.0f, 24.0f, -24.0f, 24.0f, near_plane, far_plane);
		glm::mat4 Lview = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(light.Direction[0], light.Direction[1], light.Direction[2]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		renderShader.setUniform("lightSpaceMatrix", Lprojection * Lview);

		meshes[i].Draw(renderShader);
	}
		
}

void PhysicalModel::addForce(glm::vec3 force) {
	this->force += force;
}

glm::mat3x3 PhysicalModel::getRotation() {
	return rotation;
}

void PhysicalModel::changeRotation(glm::quat rot) {
	this->rotation_quat = rot * this->rotation_quat;
	this->rotation = quat_to_mat(this->rotation_quat);
}