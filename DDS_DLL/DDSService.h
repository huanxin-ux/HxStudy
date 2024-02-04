#ifndef DDSSERVICE_H
#define DDSSERVICE_H

typedef  void (*CallBack_Sub)(const char* topic_name, const char* type_name, char* buffer, long lize);


extern "C"
{
    const char* GetInput(char* input);

    bool InitDDSServiceNoParam();

    const char* InitDDSServiceString(const char* ip_str, const char* port_str, int domain_id);

    bool InitDDSService(const char* ip_str, const char* port_str, int domain_id);

    bool DeInitDDSService();

    bool PublishTopic(const char* topic_name, const char* topic_type, const char* buffer, int length);

    bool SubscribTopic(const char* topic_name, const char* topic_type, CallBack_Sub call_back);

    bool UnSubscribTopic(const char* topic_name, const char* topic_type);
}



#endif // DDSSERVICE_H
