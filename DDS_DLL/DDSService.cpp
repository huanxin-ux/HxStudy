#include "DDSService.h"
#include "ace/ace_wchar.h"
#include "Boilerplate.h"
#include "DataReaderListenerImpl.h"

#include <stdio.h>
#include <thread>
#include <chrono>

bool isinit = false;
const int length = 100;

DDS::DomainParticipantFactory_var dpf = nullptr;

DDS::DomainParticipant_var participant = nullptr;

DDS::Publisher_var publisher = nullptr;

DDS::Subscriber_var subscriber = nullptr;

std::map<std::string, DDS::Topic_var> pub_topic_map;

std::map<std::string, DDS::Topic_var> sub_topic_map;

std::map<std::string, DDS::DataWriter_var> DataWriter_map;

std::map<std::string, DDS::DataReader_var> DataReader_map;

std::map<std::string, DDS::DataReaderListener_var> Listener_map;

std::map<std::string, DDS_DLL::DDS_BaseDataDataWriter_var> BaseDataDataWriter_map;

std::string input_str;

std::string Key(const char *topic_name, const char *topic_type)
{
    return std::string(topic_name) + std::string(topic_type);
}

bool InitDDSService(const char* ip_str, const char* port_str, int domain_id)
{

    if(isinit)
    {
        return true;
    }

    int argc = 3;
    ACE_TCHAR** argv = new ACE_TCHAR*[argc];
    for(int i = 0; i < argc; i++)
    {
        argv[i] = new ACE_TCHAR[length];
        memset((char*)argv[i], 0, length);
    }

    sprintf(argv[1], "-DCPSInfoRepo");
    sprintf(argv[2], "corbaloc::%s:%s/DCPSInfoRepo", ip_str, port_str);

    // Initialize DomainParticipantFactory
    dpf = TheParticipantFactoryWithArgs(argc, argv);
    if(!dpf)
    {
        std::cout << "TheParticipantFactoryWithArgs func exec faild" << std::endl;
        return false;
    }

    // Create DomainParticipant
    participant = Boilerplate::createParticipant(dpf, domain_id);
    if(!participant)
    {
        std::cout << "Boilerplate::createParticipant func exec faild" << std::endl;
        return false;
    }

    //Create subscriber
    if(!subscriber)
    {
        subscriber = Boilerplate::createSubscriber(participant);
    }

    //Create publisher
    if(!publisher)
    {
        publisher = Boilerplate::createPublisher(participant);
    }

    //Create DomainParticipant success
    for(int i = 0; i < argc; i++)
    {
        delete argv[i];
        argv[i] = nullptr;
    }
    delete[] argv;
    argv = nullptr;

    isinit = true;

    return true;
}

bool DeInitDDSService()
{
    // Clean-up!
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();
    return true;
}

bool PublishTopic(const char *topic_name, const char *topic_type, const char *buffer, int length)
{

    if(!publisher)
    {
        std::cout << "Boilerplate::createPublisher func exec faild" << std::endl;
        return false;
    }

    std::string key = Key(topic_name, topic_type);

    if(pub_topic_map.count(key) <= 0)
    {
        DDS::Topic_var topic_dds = Boilerplate::createTopic(participant, topic_name, topic_type);
        if(!topic_dds)
        {
            std::cout << "Boilerplate::createTopic func exec faild" << std::endl;
            return false;
        }
        pub_topic_map.insert(std::make_pair(key, topic_dds));

        DDS::DataWriter_var writer_dds = Boilerplate::createDataWriter(publisher, topic_dds);
        if(!writer_dds)
        {
            std::cout << "Boilerplate::createDataWriter func exec faild" << std::endl;
            return false;
        }
        DataWriter_map.insert(std::make_pair(key, writer_dds));

        DDS_DLL::DDS_BaseDataDataWriter_var message_writer = Boilerplate::narrow(writer_dds);
        if(!message_writer)
        {
            std::cout << "Boilerplate::narrow func exec faild" << std::endl;
            return false;
        }
        BaseDataDataWriter_map.insert(std::make_pair(key, message_writer));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    if(BaseDataDataWriter_map.count(key) > 0)
    {
        Boilerplate::SendTopicDDS(BaseDataDataWriter_map[key], buffer, length);
        return true;
    }

    return false;

}

bool SubscribTopic(const char *topic_name, const char *topic_type, CallBack_Sub call_back)
{      
    if(!subscriber)
    {
        std::cout << "Boilerplate::createSubscriber func exec faild" << std::endl;
        return false;
    }


    std::string key = Key(topic_name, topic_type);


    if(sub_topic_map.count(key) <= 0)
    {
        DDS::Topic_var topic_dds = Boilerplate::createTopic(participant, topic_name, topic_type);
        if(!topic_dds)
        {
            std::cout << "Boilerplate::createTopic func exec faild" << std::endl;
            return false;
        }
        sub_topic_map.insert(std::make_pair(key, topic_dds));

        DDS::DataReaderListener_var listener = new DataReaderListenerImpl(call_back, topic_name, topic_type);      
        Listener_map.insert(std::make_pair(key, listener));
    }

    if(sub_topic_map.count(key) > 0 && Listener_map.count(key) > 0
            && DataReader_map.count(key) <= 0)
    {
        DDS::DataReader_var writer_dds = Boilerplate::createDataReader(subscriber, sub_topic_map[key], Listener_map[key]);
        if(!writer_dds)
        {
            std::cout << "Boilerplate::createDataReader func exec faild" << std::endl;
            return false;
        }
        DataReader_map.insert(std::make_pair(key, writer_dds));
    }
    return true;
}

bool UnSubscribTopic(const char *topic_name, const char *topic_type)
{
    std::string key = Key(topic_name, topic_type);

    if(subscriber && DataReader_map.count(key) > 0)
    {
        subscriber->delete_datareader(DataReader_map[key]);
        DataReader_map.erase(key);
        return true;
    }
    return false;
}

const char *GetInput(char *input)
{
    input_str.clear();
    input_str = std::string(input);
    input_str += "_input";
    return input_str.c_str();
}

bool InitDDSServiceNoParam()
{
    char* ip_str = "127.0.0.1";
    char* port_str = "12345";
    int domain_id = 1;
    if(isinit)
    {
        return true;
    }

    int argc = 3;
    ACE_TCHAR** argv = new ACE_TCHAR*[argc];
    for(int i = 0; i < argc; i++)
    {
        argv[i] = new ACE_TCHAR[length];
        memset((char*)argv[i], 0, length);
    }

    sprintf(argv[1], "-DCPSInfoRepo");
    sprintf(argv[2], "corbaloc::%s:%s/DCPSInfoRepo", ip_str, port_str);

    // Initialize DomainParticipantFactory
    dpf = TheParticipantFactoryWithArgs(argc, argv);
    if(!dpf)
    {
        std::cout << "TheParticipantFactoryWithArgs func exec faild" << std::endl;
        return false;
    }

    // Create DomainParticipant
    participant = Boilerplate::createParticipant(dpf, domain_id);
    if(!participant)
    {
        std::cout << "Boilerplate::createParticipant func exec faild" << std::endl;
        return false;
    }

    //Create subscriber
    if(!subscriber)
    {
        subscriber = Boilerplate::createSubscriber(participant);
    }

    //Create publisher
    if(!publisher)
    {
        publisher = Boilerplate::createPublisher(participant);
    }

    //Create DomainParticipant success
    for(int i = 0; i < argc; i++)
    {
        delete argv[i];
        argv[i] = nullptr;
    }
    delete[] argv;
    argv = nullptr;

    isinit = true;

    return true;
}

const char* InitDDSServiceString(const char *ip_str, const char *port_str, int domain_id)
{
    if(isinit)
    {
        return "success";
    }

    int argc = 3;
    ACE_TCHAR** argv = new ACE_TCHAR*[argc];
    for(int i = 0; i < argc; i++)
    {
        argv[i] = new ACE_TCHAR[length];
        memset((char*)argv[i], 0, length);
    }

    sprintf(argv[1], "-DCPSInfoRepo");
    sprintf(argv[2], "corbaloc::%s:%s/DCPSInfoRepo", ip_str, port_str);

    // Initialize DomainParticipantFactory
    dpf = TheParticipantFactoryWithArgs(argc, argv);
    if(!dpf)
    {
        std::cout << "TheParticipantFactoryWithArgs func exec faild" << std::endl;
        return "TheParticipantFactoryWithArgs func exec faild";
    }

    // Create DomainParticipant
    participant = Boilerplate::createParticipant(dpf, domain_id);
    if(!participant)
    {
        std::cout << "Boilerplate::createParticipant func exec faild" << std::endl;
        return "Boilerplate::createParticipant func exec faild";
    }

    //Create subscriber
    if(!subscriber)
    {
        subscriber = Boilerplate::createSubscriber(participant);
    }

    //Create publisher
    if(!publisher)
    {
        publisher = Boilerplate::createPublisher(participant);
    }

    //Create DomainParticipant success
    for(int i = 0; i < argc; i++)
    {
        delete argv[i];
        argv[i] = nullptr;
    }
    delete[] argv;
    argv = nullptr;

    isinit = true;

    return "success";
}
