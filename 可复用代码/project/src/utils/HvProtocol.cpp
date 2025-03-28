/**
  ******************************************************************************
  * @file           : HvProtocol.cpp
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/20
  ******************************************************************************
  */

#include "HvProtocol.h"

/**
 * @brief 封包
 * @param message
 * @return 打包后的数据：header(4字节) + message
 */
std::string HvProtocol::packMessageAsString(const std::string &message) {
	// 自定义协议格式：头部长度（4字节）+数据
	std::string packedMessage;
	packedMessage.resize(SERVER_HEAD_LENGTH + message.length());
	auto length = htonl(message.size());
	std::memcpy(packedMessage.data(), &length, SERVER_HEAD_LENGTH);
	std::memcpy(packedMessage.data() + SERVER_HEAD_LENGTH, message.data(), message.length());
	return packedMessage;
}

/**
 * @brief 拆包
 * @param receivedData
 * @return 用户本意发送的 message
 */
u_int32_t HvProtocol::unpackMessage(const std::string &receivedData, std::string &returnData) {
	// 自定义协议格式：头部长度（4字节）+数据
	uint32_t len = 0;
	std::memcpy(&len, receivedData.data(), sizeof(uint32_t));

	std::string unpackedMessage;
	auto length = receivedData.size() - SERVER_HEAD_LENGTH;
	unpackedMessage.resize(length);
	std::memcpy(unpackedMessage.data(), receivedData.data() + SERVER_HEAD_LENGTH, length);

	returnData = unpackedMessage;
	return ntohl(len);
}