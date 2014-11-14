#pragma once

#include "TextureFormat.h"

#include <GL/gl3w.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace GM {
namespace Core {

class Texture; typedef std::shared_ptr<Texture> TexturePtr;

class TextureFactory
{
public:
	typedef std::function<std::shared_ptr<const std::vector<unsigned char>> (const std::string &file_name, int &width, int &height, GLenum &texture_format, GLenum &type)> FileFetcherFunction;

	/** Creates and loads a texture as the format requires.
	 * You _must_ specify a file fetcher. The function do not care how the image is produced, as long as it is produced.
	 *
	 * The file fetcher can be whatever, eg.
	 * [](const std::string &name, int &w, int &h, GLenum &gl_texture_format, GLenum &gl_type) -> std::shared_ptr<const vector<unsigned char>> {
	 * 	SOILTextureIO io;
	 * 	auto img = io.load(name);
	 * 	w = img.get_width();
	 * 	h = img.get_height();
	 * 	int channels = img.get_num_channels();
	 *      if (channels = 1) gl_texture_format = GL_RED;
	 *      if (channels = 2) gl_texture_format = GL_RG;
	 *      if (channels = 3) gl_texture_format = GL_RGB;
	 *      if (channels = 4) gl_texture_format = GL_RGBA;
	 *      gl_type = GL_UNSIGNED_BYTE;
	 * 	return img.get_dataptr();
	 * };
	 *
	 * @return A shared pointer of a Core::Texture instance
	 */
	static TexturePtr create(const TextureFormat &format, const std::vector<std::string> &image_paths, FileFetcherFunction &file_fetcher);

	struct TextureData {
		int width;
		int height;
		GLenum internal_format;
		GLenum texture_format;
		GLenum data_type;
		std::shared_ptr<const std::vector<unsigned char>> data_ptr;
	};
	static TexturePtr create(const TextureFormat &format, TextureData data);

	static TexturePtr create(const TextureFormat &format, const std::vector<TextureData> &data);

	static TexturePtr create(const TextureFormat &format);

private:
	static void set_parameters(const TexturePtr &texture, const TextureFormat &format);

	static void upload(const TexturePtr &texture, const std::vector<std::string> &image_paths, FileFetcherFunction &file_fetcher);

	static void upload_single_image(const GLenum target, const std::string &file_name, FileFetcherFunction &file_fetcher);

	// Convenience
	static void upload_single_image(const GLenum target, const TextureData &data);

	// The one actually uploading the image
	static void upload_single_image(const GLenum target, int width, int height, GLenum internal_format, GLenum texture_format, GLenum data_type, std::shared_ptr<const std::vector<unsigned char>> data_ptr);

};

} // namespace Core
} // namespace GM
