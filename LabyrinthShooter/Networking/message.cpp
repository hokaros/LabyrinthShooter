#include "message.h"

template<typename dataType>
connection::Message& operator += (connection::Message& msg, const dataType& data) {

	size_t currentSize = msg.messageBody.size();
	msg.messageBody.resize(currentSize + sizeof(dataType));
	std::memcpy(msg.messageBody.data() + currentSize, &data, sizeof(dataType));
	msg.messageHeader.size = msg.size();

	return msg;
}