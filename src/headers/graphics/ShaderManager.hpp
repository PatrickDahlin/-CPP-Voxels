#ifndef _SHADERMANAGER_HPP_
#define _SHADERMANAGER_HPP_

#include "graphics/ShaderProgram.hpp"
#include "core/ResourceManager.hpp"
#include <string>

class ShaderProgram;

struct ShaderParams {
	ShaderParams(std::string v, std::string f) : vert(v), frag(f) {}
	std::string vert;
	std::string frag;
};

inline bool operator==(const ShaderParams& left, const ShaderParams& right)
{
	return (left.vert == right.vert && left.frag == right.frag);
}

namespace std {
	template<> struct hash<ShaderParams>
	{
		typedef ShaderParams arg_type;
		typedef std::size_t result;
		result operator()(arg_type const& arg) const noexcept
		{
			std::size_t h1 = std::hash<std::string>{}(arg.vert);
			std::size_t h2 = std::hash<std::string>{}(arg.frag);
			return h1 ^ (h2 << 1);
		}
	};
}


class ShaderManager : public ResourceManager<ShaderParams, ShaderProgram> {
public:
	ShaderManager();
	~ShaderManager();
	
	ShaderProgram* get_shader(std::string vert, std::string frag);

};

#endif