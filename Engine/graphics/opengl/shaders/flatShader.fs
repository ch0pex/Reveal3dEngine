# version 450 core

in vec2 passTexCoords;

o
    u32 solid_shader, water_shader, texture;

    solid_shader = ShaderLoader::CreateProgram((kGameConfig.solid_vertex_shader_path).c_str(), (kGameConfig.solid_fragment_shader_path).c_str());
    water_shader = ShaderLoader::CreateProgram((kGameConfig.water_vertex_shader_path).c_str(), (kGameConfig.water_fragment_shader_path).c_str());
    texture = TextureLoader::LoadTexture(kGameConfig.texture_atlas_path);

    solid_renderer_.SetTexture(texture);
    solid_renderer_.SetShader(solid_shader);
    water_renderer_.SetTexture(texture);
    water_renderer_.SetShader(water_shader);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

t vec4 color;

uniform sampler2D Texture;

void main()
{
	color = texture(Texture, passTexCoords);
}
