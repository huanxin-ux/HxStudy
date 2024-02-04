/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include "Boilerplate.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>



namespace Boilerplate {

DDS::DomainParticipant_var
createParticipant(DDS::DomainParticipantFactory_var dpf, int domain_id)
{
  // Create DomainParticipant
  DDS::DomainParticipant_var participant =
    dpf->create_participant(domain_id, // made-up domain ID
                            PARTICIPANT_QOS_DEFAULT,
                            0,  // no listener
                            OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  return participant;
}

DDS::Topic_var createTopic(DDS::DomainParticipant_var participant, std::string topic_name, std::string type_name)
{
    // Register TypeSupport
    DDS_DLL::DDS_BaseDataTypeSupport_var ts = new DDS_DLL::DDS_BaseDataTypeSupportImpl;

    if (ts->register_type(participant, type_name.c_str()) != DDS::RETCODE_OK)
    {
        return nullptr;
    }


    DDS::Topic_var topic = participant->create_topic(topic_name.c_str(),
        type_name.c_str(), TOPIC_QOS_DEFAULT, 0, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    return topic;
}

DDS::Publisher_var
createPublisher(DDS::DomainParticipant_var participant)
{
  // Create Publisher
  DDS::Publisher_var publisher =
    participant->create_publisher(
        PUBLISHER_QOS_DEFAULT,
        0,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  return publisher;
}

DDS::Subscriber_var
createSubscriber(DDS::DomainParticipant_var participant)
{
  // Create Subscriber
  DDS::Subscriber_var subscriber =
      participant->create_subscriber(
          SUBSCRIBER_QOS_DEFAULT,
          0,
          OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  return subscriber;
}

DDS::DataWriter_var createDataWriter(DDS::Publisher_var publisher, DDS::Topic_var topic)
{
  // Create DataWriter
  DDS::DataWriter_var writer = publisher->create_datawriter(
      topic, DATAWRITER_QOS_DEFAULT, 0, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  return writer;
}

DDS::DataReader_var createDataReader(DDS::Subscriber_var subscriber, DDS::Topic_var topic, DDS::DataReaderListener_var listener)
{
    DDS::DataReaderQos reader_qos;
    subscriber->get_default_datareader_qos(reader_qos);
    reader_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    // Create DataReader
    DDS::DataReader_var reader = subscriber->create_datareader(
        topic, reader_qos, listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);


    return reader;
}

DDS_DLL::DDS_BaseDataDataWriter_var narrow(DDS::DataWriter_var writer)
{
    // Safe cast to type-specific data writer
    DDS_DLL::DDS_BaseDataDataWriter_var message_writer =
    DDS_DLL::DDS_BaseDataDataWriter::_narrow(writer);

    // Check for failure
    if (!message_writer) {
    throw std::string("failed to narrow data writer");
    }

    return message_writer;
}

DDS_DLL::DDS_BaseDataDataReader_var narrow(DDS::DataReader_var reader)
{
    // Safe cast to type-specific data reader
    DDS_DLL::DDS_BaseDataDataReader_var message_reader =
    DDS_DLL::DDS_BaseDataDataReader::_narrow(reader);

    return message_reader;
}




bool SendTopicDDS(DDS_DLL::DDS_BaseDataDataWriter_var& message_writer, const char* content, int lsize)
{
	// Write samples
    DDS_DLL::DDS_BaseData BaseData;
    BaseData.data.length(lsize);
    BaseData.lSize = lsize;
    memcpy(BaseData.data.get_buffer(), content, lsize);
    DDS::ReturnCode_t error = message_writer->write(BaseData, DDS::HANDLE_NIL);
    if (error != DDS::RETCODE_OK)
    {
        std::cout << std::string("failed to send topic");
        return false;
    }

    BaseData.data.release();
    return true;
}




void cleanup(DDS::DomainParticipant_var participant, DDS::DomainParticipantFactory_var dpf)
{
    // Delete any topics, publishers and subscribers owned by participant
    participant->delete_contained_entities();
    // Delete participant itself
    dpf->delete_participant(participant);
    // Shut down info repo connection
    TheServiceParticipant->shutdown();
}

}  // End namespaces
