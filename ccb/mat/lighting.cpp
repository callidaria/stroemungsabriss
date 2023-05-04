#include "lighting.h"

/*
	add_<lighttype>(LightSun|LightPoint|LightSpot) -> void
	args[0]: new light to add to scene
	purpose: add a new light to the scene
*/
void Lighting::add_sunlight(LightSun sunlight)
{ sunlights.push_back(sunlight); }
void Lighting::add_pointlight(LightPoint pointlight)
{ pointlights.push_back(pointlight); }
void Lighting::add_spotlight(LightSpot spotlight)
{ spotlights.push_back(spotlight); }

/*
	TODO
*/
void Lighting::load_irradiance_maps(Cubemap imap)
{
	irradiance_map.diff_component = imap.get_diffusion_approximation();
	irradiance_map.spec_component = imap.get_specular_approximation();
}

/*
	upload(Shader*) -> void
	shader: deferred lighting shader
	purpose: upload complex lighting system to the simplified deferred lighting shader
	NOTE: parameterized shader has to be enabled before running this method
*/
void Lighting::upload(Shader* shader)
{
	// upload sunlights
	size_t sunlight_count = sunlights.size();
	for (uint16_t i=0;i<sunlight_count;i++) {
		std::string arr_location = "sunlight["+std::to_string(i)+"].";
		shader->upload_vec3((arr_location+"position").c_str(),sunlights[i].position);
		shader->upload_vec3((arr_location+"colour").c_str(),sunlights[i].colour);
		shader->upload_float((arr_location+"intensity").c_str(),sunlights[i].intensity);
	} shader->upload_int("sunlight_count",sunlight_count);

	// upload pointlights
	size_t pointlight_count = pointlights.size();
	for (uint16_t i=0;i<pointlight_count;i++) {
		std::string arr_location = "pointlight["+std::to_string(i)+"].";
		shader->upload_vec3((arr_location+"position").c_str(),pointlights[i].position);
		shader->upload_vec3((arr_location+"colour").c_str(),pointlights[i].colour);
		shader->upload_float((arr_location+"constant").c_str(),pointlights[i].constant);
		shader->upload_float((arr_location+"linear").c_str(),pointlights[i].linear);
		shader->upload_float((arr_location+"quadratic").c_str(),pointlights[i].quadratic);
		shader->upload_float((arr_location+"intensity").c_str(),pointlights[i].intensity);
	} shader->upload_int("pointlight_count",pointlight_count);

	// upload spotlights
	size_t spotlight_count = spotlights.size();
	for(uint16_t i=0;i<spotlight_count;i++) {
		std::string arr_location = "spotlight["+std::to_string(i)+"].";
		shader->upload_vec3((arr_location+"position").c_str(),spotlights[i].position);
		shader->upload_vec3((arr_location+"colour").c_str(),spotlights[i].colour);
		shader->upload_vec3((arr_location+"direction").c_str(),spotlights[i].direction);
		shader->upload_float((arr_location+"cut_in").c_str(),spotlights[i].cut_in);
		shader->upload_float((arr_location+"cut_out").c_str(),spotlights[i].cut_out);
	} shader->upload_int("spotlight_count",spotlight_count);
}

/*
	TODO
*/
void Lighting::upload_diffusion_map()
{ glBindTexture(GL_TEXTURE_CUBE_MAP,irradiance_map.diff_component); }
void Lighting::upload_specular_map()
{ glBindTexture(GL_TEXTURE_CUBE_MAP,irradiance_map.spec_component); }