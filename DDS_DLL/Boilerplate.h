/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */
#include "DDS_IDLTypeSupportImpl.h"
#include <string>
// Boilerplate code pulled out of mains of publisher and subscriber
// in order to simplify this example.

#define TOPICHELLO "Hello_Topic"
#define TOPICWORLD "Heartbeat"

namespace Boilerplate {

DDS::DomainParticipant_var
createParticipant(DDS::DomainParticipantFactory_var dpf, int domain_id);

DDS::Publisher_var  createPublisher(DDS::DomainParticipant_var participant);
DDS::Subscriber_var createSubscriber(DDS::DomainParticipant_var participant);

DDS::Topic_var createTopic(DDS::DomainParticipant_var participant, std::string topic_name, std::string type_name);

DDS::DataWriter_var createDataWriter(DDS::Publisher_var publisher,
                                     DDS::Topic_var topic);
DDS::DataReader_var createDataReader(DDS::Subscriber_var subscriber,
                                     DDS::Topic_var topic,
                                     DDS::DataReaderListener_var listener);

DDS_DLL::DDS_BaseDataDataWriter_var narrow(DDS::DataWriter_var writer);
DDS_DLL::DDS_BaseDataDataReader_var narrow(DDS::DataReader_var reader);
bool SendTopicDDS(DDS_DLL::DDS_BaseDataDataWriter_var& message_writer, const char* buffer, int length);



void cleanup(DDS::DomainParticipant_var participant,
             DDS::DomainParticipantFactory_var dpf);

}  // End namespaces
