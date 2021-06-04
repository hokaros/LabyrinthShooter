#pragma once

#include <vector>
#include "msgId.h"

namespace connection {

	
	struct MessageHeader {

		Id messageId{};
		uint32_t size = 0;
	};


	
	struct Message {

		MessageHeader messageHeader{};
		std::vector<uint8_t> messageBody;

		

		template<typename dataType>
		friend Message& operator += (Message& msg, const dataType& data) {

			size_t currentSize = msg.messageBody.size();
			msg.messageBody.resize(currentSize + sizeof(dataType));
			std::memcpy(msg.messageBody.data() + currentSize, &data, sizeof(dataType));
			msg.messageHeader.size = msg.messageBody.size();

			return msg;
		}

		template<typename dataType>
		friend Message& operator >> (Message& msg, dataType& data) {

			//proba czytania pustej wiadomosci
			if (msg.messageBody.size() == 0) {
				std::cout << "[WARNING] An attempt to read an empty message\n";
				return msg;
			}

			size_t newSize = msg.messageBody.size() - sizeof(dataType);
			std::memcpy(&data, msg.messageBody.data() + newSize, sizeof(dataType));
			msg.messageBody.resize(newSize);
			msg.messageHeader.size = msg.messageBody.size();

			return msg;
		}


	};

}
