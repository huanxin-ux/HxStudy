#include <iostream>
#include <vector>
#include <document.h>
#include <fstream>

struct JsonPairInterface
{
	virtual std::string key() = 0;
	virtual void save(rapidjson::Value& json_value) = 0;
};

struct JsonParse
{
	bool fromJson(rapidjson::Document& json_doc)
	{
		if (json_doc.HasParseError())
		{
			return false;
		}
		return fromJson(std::move(json_doc.GetObject()));
	}

	bool fromJson(rapidjson::GenericObject<false, rapidjson::Value> json_obj)
	{
		
		for (auto element : elements)
		{
			if (json_obj.HasMember(element->key().c_str()))
			{
				element->save(json_obj[element->key().c_str()]);
			}			
		}
		return true;
	}

	void registerElement(JsonPairInterface* element)
	{
		elements.push_back(element);
	}
	std::vector<JsonPairInterface*> elements;
};

template<typename T>
struct JsonPair : public JsonPairInterface
{
	JsonPair(std::string json_key, JsonParse* json_parse)
		: m_key(std::move(json_key))
		, m_value(T())
	{
		json_parse->registerElement(this);
	}

	T& operator()()
	{
		return m_value;
	}

	virtual std::string key()override
	{
		return m_key;
	}



	template <typename K>
	using ValueType = typename std::conditional<std::is_base_of<JsonParse, K>::value, JsonParse, K>::type;

	virtual void save(rapidjson::Value& json_value)override
	{
		fromJsonValue(m_value, json_value, ValueType<T>());
	}

	//������ͨ����
	template<typename I, typename K>
	void fromJsonValue(I& dst_value, rapidjson::Value& json_value, K)
	{
		dst_value = json_value.Get<K>();
	}

	//����std::string
	template<typename I>
	void fromJsonValue(I& dst_value, rapidjson::Value& json_value, std::string)
	{
		dst_value = json_value.GetString();
	}

	//����object
	template<typename I>
	void fromJsonValue(I& dst_value, rapidjson::Value& json_value, JsonParse)
	{
		dst_value.fromJson(json_value.GetObject());
	}

	//����array
	template<typename I, typename K>
	void fromJsonValue(I& dst_value, rapidjson::Value& json_value, std::vector<K>)
	{
		int i = 0;
		auto json_array = json_value.GetArray();
		m_value.resize(json_array.Size());
		for (auto& it : json_array)
		{
			rapidjson::Value& json_array_value = it;
			fromJsonValue(m_value.at(i++), json_array_value, ValueType<K>());
		}
	}

	std::string m_key;
	T m_value;
};

#define JSONPAIR(Type, Key) JsonPair<Type> Key{#Key, this}

struct Stage : public JsonParse
{
	JSONPAIR(std::string, stageStr);
	JSONPAIR(int, stageValue);
};

struct Info : public JsonParse
{
	JSONPAIR(std::string, project);
	JSONPAIR(Stage, stage);
	JSONPAIR(int, stars);
	JSONPAIR(int, stage1);
	JSONPAIR(int, stage2);
	JSONPAIR(std::vector<std::string>, namelist);
	JSONPAIR(std::vector<Stage>, array_Object);
	JSONPAIR(int, ttt);
	JSONPAIR(std::string, ddd);
};





std::string ReadFile(std::string file_path)
{
	std::string content;

	std::ifstream readFile;
	readFile.open(file_path);  //����Ҫ�������ļ�
	if (readFile.fail())
	{
		std::cout << "file open faild" << std::endl;
		return "";   //˵���ļ���ȡʧ�ܣ�
	}
	readFile.seekg(0, std::ios::end);  //��ָ��ָ���ļ��Ľ�β
	int nLen = readFile.tellg();   //��ȡ�ļ��ĳ���
	readFile.seekg(0, std::ios::beg);  //�ٽ�ָ��ָ���ļ��Ŀ�ʼ����Ҫ��Ϊ�˶�ȡ����

	//char* buffer = new char[nLen];
	content.resize(nLen, 0);

	readFile.read((char*)content.c_str(), nLen);  //��ȡ�ļ��е�����

	readFile.close();  //�ڲ����ļ����в���ʱ��һ��Ҫ�ر��ļ�

	return content;
}



int main()
{
	std::string json_str = ReadFile("D:/test.json");

	Info info;
	rapidjson::Document json_doc;
	json_doc.Parse(json_str.c_str());

	bool issucces = info.fromJson(json_doc);

	if (!issucces)
	{
		std::cout << "����ʧ��" << std::endl;
		return 0;
	}
	std::cout << "�����ɹ�" << std::endl;
	std::cout << "project: " << info.project() << std::endl;
	std::cout << "Stage.stageStr: " << info.stage().stageStr() << std::endl;
	std::cout << "Stage.stageValue: " << info.stage().stageValue() << std::endl;
	std::cout << "stars: " << info.stars() << std::endl;
	std::cout << "stage1: " << info.stage1() << std::endl;
	std::cout << "stage2: " << info.stage2() << std::endl;

	std::cout << "array list: " << std::endl;
	for (auto& value : info.namelist())
	{	
		std::cout << value.c_str() << std::endl;
	}

	std::cout << "arrayObject: " << std::endl;
	for (auto& value1 : info.array_Object())
	{
		std::cout << value1.stageStr() << std::endl;
		std::cout << value1.stageValue() << std::endl;
	}
	std::cout << "ttt: " << info.ttt() << std::endl;
	std::cout << "ddd: " << info.ddd() << std::endl;
}