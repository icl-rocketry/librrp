#pragma once

#include <stdint.h>
#include <librnp/rnp_packet.h>
#include <libriccore/platform/millis.h>

#include "Config/forward_decl.h" 

namespace DummyCommands{
    void getTimeCommand(ForwardDecl_SystemClass& simNode, const RnpPacketSerialized& packet){

		SimpleCommandPacket commandpacket(packet);

		uint32_t time = millis();

		BasicDataPacket<uint32_t,0,105> responsePacket(time);
		responsePacket.header.source_service = simNode.commandhandler.getServiceID(); 
		responsePacket.header.destination_service = packet.header.source_service;
		responsePacket.header.source = packet.header.destination;
		responsePacket.header.destination = packet.header.source;
		responsePacket.header.uid = packet.header.uid;
		system.networkmanager.sendPacket(responsePacket);	
	}
}