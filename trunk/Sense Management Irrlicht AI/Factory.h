#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>

template <class T>
class Factory
{
public:
	T* Create(const std::string& typeName)
	{
		Function f = m_map[typeName];
		if (f)
		{
			return f();
		}
		return 0;
	}
	typedef T* (*Function)();
	bool AddType(const std::string& typeName, Function f)
	{
		m_map[typeName] = f;
		return true;
	}

private:
	typedef std::map<std::string, Function> FactoryMap;

	FactoryMap m_map;
};

#endif