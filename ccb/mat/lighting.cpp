#include "lighting.h"

/*
	load_irradiance_maps(Cubemap) -> void
	purpose: get all irradiance map components from the given skybox
	\param imap: cubemap holding the desired irradiance map components
*/
void Lighting::load_irradiance_maps(Cubemap imap)
{
	irradiance_map.diff_component = imap.get_diffusion_approximation();
	irradiance_map.spec_component = imap.get_specular_approximation();
	irradiance_map.brdf_component = imap.get_specular_brdf();
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
	upload_<irradiance_component>() -> void
	purpose: uploads the desired irradiance component the the lighting shader
	NOTE: deferred lighting shader has to be enabled before running the upload
	NOTE: the appropriate texture index has to be activated before running the upload
*/
void Lighting::upload_diffusion_map()
{ glBindTexture(GL_TEXTURE_CUBE_MAP,irradiance_map.diff_component); }
void Lighting::upload_specular_map()
{ glBindTexture(GL_TEXTURE_CUBE_MAP,irradiance_map.spec_component); }
void Lighting::upload_specular_brdf()
{ glBindTexture(GL_TEXTURE_2D,irradiance_map.brdf_component); }