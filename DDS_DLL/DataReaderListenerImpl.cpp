/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include "DataReaderListenerImpl.h"
#include "DDS_IDLTypeSupportC.h"
#include "DDS_IDLTypeSupportImpl.h"
#include "dds/DdsDcpsCoreC.h"
#include "DDSService.h"

#include <iostream>



DataReaderListenerImpl::DataReaderListenerImpl(CallBack_Sub call_back, std::string topic_name, std::string topic_type)
{
    m_rc_callback = call_back;
    m_topic_name = topic_name;
    m_topic_type = topic_type;
}

DataReaderListenerImpl::~DataReaderListenerImpl()
{
	std::cout << "delete DataReaderListenerImpl" << std::endl;
}

void
DataReaderListenerImpl::on_requested_deadline_missed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedDeadlineMissedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_requested_incompatible_qos(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedIncompatibleQosStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_sample_rejected(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleRejectedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_liveliness_changed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::LivelinessChangedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
    std::cout << "recive msg: " << std::endl;
  DDS_DLL::DDS_BaseDataDataReader_var reader_i = DDS_DLL::DDS_BaseDataDataReader::_narrow(reader);
  if (reader_i)
  {
      DDS_DLL::DDS_BaseData message;
	  DDS::SampleInfo info;

	  const DDS::ReturnCode_t error = reader_i->take_next_sample(message, info);

      if (error == DDS::RETCODE_OK
              && info.instance_state != DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE
              && info.instance_state != DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE
              && info.instance_state != DDS::NOT_ALIVE_INSTANCE_STATE) {
		  std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
		  std::cout << "SampleInfo.instance_state = " << OpenDDS::DCPS::InstanceState::instance_state_mask_string(info.instance_state) << std::endl;

		  if (info.valid_data) {
                m_rc_callback(m_topic_name.c_str(), m_topic_type.c_str(), message.data.get_buffer(), message.data.length());
		  }
	  }
  }



  

  
}

void
DataReaderListenerImpl::on_subscription_matched(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SubscriptionMatchedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_sample_lost(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleLostStatus& /*status*/)
{
}
