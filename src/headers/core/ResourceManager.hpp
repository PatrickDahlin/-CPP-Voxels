#ifndef _RESOURCEMANAGER_HPP_
#define _RESOURCEMANAGER_HPP_

#include "core/Disposable.hpp"

#include "core/Common.hpp"
#include <unordered_map>


template<class U, class T>
class ResourceManager {
	static_assert(std::is_base_of<Disposable, T>::value, "Template parameter of ResourceManager must derive from Disposable");
public:
	ResourceManager() : resources() {}
	virtual ~ResourceManager() {}

	void dispose() {
		for(auto it : resources)
			it.second->dispose();
	
		resources.clear();
	}

protected:

	T* get_resource(U key) const {
		auto found = resources.find(key);
		if(found != resources.end())
			return found->second;
		else
			return nullptr;
	}

	bool has_resource(U key) {
		if(resources.find(key) != resources.end()) return true;
		return false;
	}

	void add_resource(U key, T* value) {
		resources[key] = value;
	}



//dfasf s 


private:

	std::unordered_map<U,T*> resources;

};

#endif