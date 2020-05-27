#pragma once

#include <iostream>
#include "../../../Extern/Json/json.h"
#include "Utils\CogPath.h"
#include <fstream>
#include "GLM\glm.hpp"

namespace serialization
{

	class Serializable
	{
	public:
		inline virtual ~Serializable() {}

		inline virtual void to_json(Json::Value & val) const = 0;
		inline virtual void from_json(const Json::Value & val) = 0;

		inline void to_stream(std::ostream& s) const
		{
			Json::Value val;
			to_json(val);
			Json::StyledStreamWriter writer;
			writer.write(s, val);
		}
		inline void from_stream(std::istream& s)
		{
			Json::Value val;
			Json::Reader reader;
			reader.parse(s, val, false);
			from_json(val);
		}

		inline explicit operator Json::Value() const
		{
			Json::Value val;
			to_json(val);
			return val;
		}

		inline friend std::ostream& operator << (std::ostream& output, const Serializable& s)
		{
			s.to_stream(output);
			return output;
		}
		inline friend Serializable& operator >> (std::istream& input, Serializable& s)
		{
			s.from_stream(input);
			return s;
		}

		inline void operator=(Json::Value& val)
		{
			to_json(val);
		}

	};

#pragma region From/to_json
	template <class T>
	void to_json(const std::vector<T> i, Json::Value& val)
	{
		Json::Value temp;
		temp.resize(static_cast<Json::ArrayIndex>(i.size()));

		for (unsigned counter = 0; counter < i.size(); ++counter)
			to_json(i[counter], temp[static_cast<int>(counter)]);

		val = temp;
	}

	inline void to_json(const int i, Json::Value& val)
	{
		val = i;
	}
	inline void to_json(const unsigned i, Json::Value& val)
	{
		val = i;
	}
	inline void to_json(const float i, Json::Value& val)
	{
		val = i;
	}
	inline void to_json(const bool i, Json::Value& val)
	{
		val = i;
	}
	inline void to_json(const std::string& i, Json::Value& val)
	{
		val = i;
	}
	inline void to_json(const glm::vec2& i, Json::Value& val)
	{
		Json::Value temp;
		temp.resize(4);

		to_json(i.x, temp[0]);
		to_json(i.y, temp[1]);

		val = temp;
	}
	inline void to_json(const glm::vec4& i, Json::Value& val)
	{
		Json::Value temp;
		temp.resize(4);

		to_json(i.x, temp[0]);
		to_json(i.y, temp[1]);
		to_json(i.z, temp[2]);
		to_json(i.w, temp[3]);

		val = temp;
	}
	inline void to_json(const CogPath & i, Json::Value& val)
	{
		i.to_json(val);
	}

	inline void to_json(const Serializable& i, Json::Value& val)
	{
		i.to_json(val);
	}

	/*struct ObjectPath
	{
		void * object;
	};

	inline void to_json(const ObjectPath& i, Json::Value& val)
	{
		to_json((unsigned)(i.object->GUID), val["ptr"]);
	}

	typedef std::map<unsigned, unsigned> MemAddressTable;
	typedef std::map<unsigned, std::list<void**>> LinkReqTable;

	static MemAddressTable sMemTable;
	static LinkReqTable sLinkTable;

	inline void from_json(ObjectPath &i, Json::Value& val)
	{
		unsigned tmp;
		from_json(tmp, val["ptr"]);
		sLinkTable[tmp].push_back(&i.object);
	}*/

	inline void from_json(int& i, const Json::Value& val)
	{
		i = val.asInt();
	}
	inline void from_json(unsigned& i, const Json::Value& val)
	{
		i = static_cast<unsigned>(val.asUInt64());
	}
	inline void from_json(float& i, const Json::Value& val)
	{
		i = val.asFloat();
	}
	inline void from_json(bool& i, const Json::Value& val)
	{
		i = val.asBool();
	}
	inline void from_json(std::string& i, const Json::Value& val)
	{
		i = val.asString();
	}
	inline void from_json(glm::vec2& i, const Json::Value& val)
	{
		i.x = val[0].asFloat();
		i.y = val[1].asFloat();
	}
	inline void from_json(glm::vec4& i, const Json::Value& val)
	{
		i.x = val[0].asFloat();
		i.y = val[1].asFloat();
		i.z = val[2].asFloat();
		i.w = val[3].asFloat();
	}
	inline void from_json(CogPath & i, const Json::Value& val)
	{
		i.from_json(val);
	}
	inline void from_json(Serializable& s, const Json::Value& val)
	{
		s.from_json(val);
	}
	template <class T>
	void from_json(std::vector<T>& s, const Json::Value& val)
	{
		s.resize(val.size());
		for (unsigned counter = 0; counter < s.size(); ++counter)
			from_json(s[counter], val[static_cast<int>(counter)]);
	}

	template<class ... Args>
	void to_json_variadic(Json::Value& val, const std::vector<std::string>& names, const Args&... args)
	{
		unsigned i = 0;
		to_json(val, names, i, args...);
	}

	inline void to_json(Json::Value& /*val*/, const std::vector<std::string>& /*names*/, unsigned /*i*/) {}

	template<class T, class ...Args>
	void to_json(Json::Value& val, const std::vector<std::string>& names, unsigned i, const T& first, const Args&... args)
	{
		to_json(first, val[names[i++]]);
		to_json(val, names, i, args...);
	}

	template<class ... Args>
	void from_json_variadic(const Json::Value& val, const std::vector<std::string>& names, Args&... args)
	{
		unsigned i = 0;
		from_json(val, names, i, args...);
	}
	inline Json::Value FileToJson(const std::string & filename, const std::string & path)
	{
		std::ifstream in(path + filename);
		Json::Value JsonValue;
		Json::Reader reader;
		reader.parse(in, JsonValue, false);
		return JsonValue;
	}
	inline void JsonToFile(const Json::Value& val, const std::string & filename, const std::string & path)
	{
		std::ofstream out(path + filename);
		if (out.is_open())
		{
			Json::StyledStreamWriter jsonWriter;
			jsonWriter.write(out, val);
		}
		else
		{
			std::cout << "Serializer::WriteToFile - Could not save to file" << std::endl;
		}
	}

	inline void from_json(const Json::Value& /*val*/, const std::vector<std::string>& /*names*/, unsigned /*i*/) {}

	template<class T, class ...Args>
	void from_json(const Json::Value& val, const std::vector<std::string>& names, unsigned i, T& first, Args&... args)
	{
		from_json(first, val[names[i++]]);
		from_json(val, names, i, args...);
	}
#pragma endregion

#pragma region Macros

#define SERIALIZABLE									\
		virtual void from_json(const Json::Value & val);		\
		virtual void to_json(Json::Value & val) const;			\

#define SERIALIZABLE_NO_PROPERTIES									\
		virtual void from_json(const Json::Value & /*val*/) {}		\
		virtual void to_json(Json::Value & /*val*/) const {}			\

#define get_names_(type) get_names_##type
#define IMPLEMENT_GET_NAMES(CLASS, ...)													\
		static const std::vector<std::string> & get_names_(CLASS)()										\
		{																						\
			enum 																				\
			{																					\
				__VA_ARGS__,																	\
				token_count																		\
			};																					\
			const char str[] = #__VA_ARGS__;													\
			static detail::tokenizer<token_count, sizeof(str) - (token_count - 1)> tokens(str);	\
			return tokens.names;																\
		}		


#define IMPLEMENT_TO_JSON(CLASS, ...)							\
		void CLASS::to_json(Json::Value & val) const					\
		{																\
			const std::vector<std::string>& names = get_names_(CLASS)();		\
			serialization::to_json_variadic(val, names, __VA_ARGS__);	\
		}																\

#define IMPLEMENT_FROM_JSON(CLASS, ...)							\
		void CLASS::from_json(const Json::Value & val)					\
		{																\
			const std::vector<std::string>& names = get_names_(CLASS)();		\
			serialization::from_json_variadic(val, names, __VA_ARGS__);	\
		}																\

#define REFLECTION(CLASS, ...)									\
		IMPLEMENT_GET_NAMES(CLASS, __VA_ARGS__);						\
		IMPLEMENT_TO_JSON(CLASS, __VA_ARGS__);							\
		IMPLEMENT_FROM_JSON(CLASS, __VA_ARGS__);						\

#pragma endregion

}

namespace detail
{
	template <size_t token_count, size_t size>
	class tokenizer
	{
	public:
		tokenizer(const char* str)
		{
			const char* c = ",";
			const char* space = " ";
			const char* null = "\0";

			do
			{
				if (*str == *space) ++str;

				const char *begin = str;

				while (*str != *c && *str)
				{
					buffer.push_back(*str);
					++str;
				}
				buffer.push_back(*null);
				names.push_back(std::string(begin, str));
			} while (0 != *str++);

		}
		std::vector<std::string> names;
		std::vector<char> buffer;
	};
}


