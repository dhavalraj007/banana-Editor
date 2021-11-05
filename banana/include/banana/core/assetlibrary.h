#pragma once
#include<unordered_map>
#include<string>
#include<memory>
#include"banana/log.h"

namespace banana::core
{
	template<typename T>
	class AssetLibrary
	{
	public:
		AssetLibrary() = default;
		~AssetLibrary() = default;

		bool exists(const std::string& name)
		{
			return m_Assets.find(name) != m_Assets.end();
		}

		void load(const std::string& name, std::shared_ptr<T> asset)
		{
			if (exists(name))
			{
				BANANA_WARN("Asset with the same name exists already. Overwriting the asset : {}", name.c_str());
			}
			m_Assets[name] = asset;
		}

		std::shared_ptr<T> get(const std::string& name)
		{
			if (exists(name))
			{
				return m_Assets[name];
			}
			else
			{
				BANANA_ERROR("Asset {} not found.", name.c_str());
				return nullptr;
			}
		}

		void deleteAsset(const std::string name)
		{
			if (exists(name))
			{
				auto it = m_Assets[name];
				auto useCount = it->second.use_count();
				if (useCount > 1)
				{
					BANANA_INFO("number of handles left for Asset {} : {}", name.c_str(), useCount-1);
				}
				m_Assets.erase(it);
			}
		}

		void clear()
		{
			m_Assets.clear();
		}

		const std::unordered_map<std::string, std::shared_ptr<T>>& getMap() { return m_Assets; }

	private:
		std::unordered_map<std::string, std::shared_ptr<T>> m_Assets;
	};
}