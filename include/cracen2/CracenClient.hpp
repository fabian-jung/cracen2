#pragma once

#include "cracen2/network/Communicator.hpp"
#include "cracen2/backend/Messages.hpp"

namespace cracen2 {

template <class SocketImplementation>
class CracenClient {
private:

	using TagList = typename backend::ServerTagList<typename SocketImplementation::Endpoint>;
	using Communicator = network::Communicator<SocketImplementation, TagList>;
	using Endpoint = typename Communicator::Endpoint;
	using Port = typename Communicator::Port;
	using Visitor = typename Communicator::Visitor;

	Communicator communicator;

public:

	CracenClient(Endpoint serverEndpoint) {
		communicator.connect(serverEndpoint);
		communicator.send(backend::Register());
	}


}; // End of class CracenClient

} // End of namespace cracen2
