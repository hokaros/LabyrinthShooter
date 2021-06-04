#include "connectionHandler.h"
using namespace connection;

ConnectionHandler::ConnectionHandler(asio::io_context& context, asio::ip::tcp::socket socket, std::queue<Message>& messagesIn)
	:context(context), socket(std::move(socket)), messagesIn(messagesIn)
{

}


void ConnectionHandler::connectToServer(asio::ip::tcp::resolver::results_type endpoint) {

	asio::async_connect(socket, endpoint,
		[this](std::error_code error, asio::ip::tcp::endpoint endpoint)
		{
			if (!error) {
				std::cout << "[CLIENT] connected to server\n";
				readHeader();
			}
			else {
				std::cout << "[ERROR] failed to connect server\n";
			}
		});
}


bool ConnectionHandler::isConnected() {
	return socket.is_open();
}


void ConnectionHandler::sendMessage(Message& message) {

	//zmieniamy priorytet contextu na wysylanie wiadomoœci
	asio::post(context, [this, message]() {


		//musimy sprawdzic czy context nie pisze wlasnie wiadomosci
		//jesli pisze to musimy poczekac, mozna to zrobic rekurencyjnie
		//jesli kolejka z wiadomosciami do wyslania do serwera zawiera jeakies wiadomosci
		//nie wywyolujemy funkcji writeHeader, a jedynie dodajemy wiadomosc do kolejki
		//funckja write header bedzie wywolywana dopóki kolejka nie zostanie opró¿niona

		bool isWriting = !messagesOut.empty();
		messagesOut.push(message);
		//jesli kolejka jest pusta mozna 
		if (!isWriting) {

			writeHeader();
		}
		});

}


void ConnectionHandler::writeHeader() {

	asio::async_write(socket, asio::buffer(&messagesOut.front().messageHeader, sizeof(messagesOut.front().messageHeader)),
		[this](std::error_code error, std::size_t size) {

			if (!error) {

				if (messagesOut.front().messageBody.size() > 0) {
					writeBody();
				}
				else {
					messagesOut.pop();

					if (!messagesOut.empty()) {
						writeHeader();
					}
				}
			}
			else {
				std::cout << "[ERROR] failed to write header";
			}
		});
}


void ConnectionHandler::writeBody() {

	asio::async_write(socket, asio::buffer(messagesOut.front().messageBody.data(), sizeof(messagesOut.front().messageBody.size())),
		[this](std::error_code error, std::size_t size) {

			if (!error) {


				messagesOut.pop();
				if (!messagesOut.empty()) {
					writeHeader();
				}

			}
			else {
				std::cout << "[ERROR] failed to write body";
			}
		});


}


void ConnectionHandler::readHeader() {
	asio::async_read(socket, asio::buffer(&tmpMessage.messageHeader, sizeof(tmpMessage.messageHeader)),
		[this](std::error_code error, std::size_t size) {

			if (!error) {
				//teraz wczytalismy header wiadomosci, musimy sprawdzic czy wiadomosc zawiera 
				//czêœæ body, jêœli zawiera alokujemy pamiêæ i czytamy dalej

				int bodySize = tmpMessage.messageHeader.size;
				if (bodySize > 0) {
					tmpMessage.messageBody.resize(bodySize);
					readBody();
				}
				else {
					saveMessage();
				}
			}
			else {
				std::cout << "[ERROR] failed to read header";
			}
		});
}


void ConnectionHandler::readBody() {
	asio::async_read(socket, asio::buffer(tmpMessage.messageBody.data(), tmpMessage.messageBody.size()),
		[this](std::error_code error, std::size_t size) {

			if (!error) {
				saveMessage();
			}
			else {
				std::cout << "[ERROR] failed to read body";
			}

		});
}


void ConnectionHandler::saveMessage() {
	// incomplete
	messagesIn.push(tmpMessage);
	readBody();
}


void ConnectionHandler::disconnect() {
	//To do
}