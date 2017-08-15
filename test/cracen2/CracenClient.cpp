#include <memory>
#include <typeinfo>
#include <mutex>
#include <set>

#include "cracen2/util/Test.hpp"
#include "cracen2/util/Demangle.hpp"
#include "cracen2/util/Thread.hpp"
#include "cracen2/CracenServer.hpp"
#include "cracen2/CracenClient.hpp"
#include "cracen2/sockets/Asio.hpp"
#include "cracen2/util/AtomicQueue.hpp"

using namespace cracen2;
using namespace cracen2::util;
using namespace cracen2::sockets;
using namespace cracen2::backend;

constexpr std::array<unsigned int, 3> participantsPerRole = {{2, 2, 2}};

constexpr std::uint16_t serverPort = 39391;

template <class SocketImplementation>
struct CracenClientTest {

	using Endpoint = typename SocketImplementation::Endpoint;

	TestSuite testSuite;

	cracen2::CracenServer<SocketImplementation> server;
	std::vector<cracen2::CracenClient<SocketImplementation>> clients;

	CracenClientTest(TestSuite& parent)  :
		testSuite(
			std::string("Implementation test for ")+demangle(typeid(SocketImplementation).name()),
			std::cout,
			&parent
		),
		server(
			serverPort
		)
	{
		for(unsigned int role = 0; role < participantsPerRole.size(); role++) {
			for(unsigned int id = 0; id < participantsPerRole[role]; id++) {
				clients.emplace_back(
					Endpoint(
						boost::asio::ip::address::from_string("127.0.0.1"),
						serverPort
					),
					role
				);
			}
		}

	}

	~CracenClientTest() {
		server.stop();
	}

}; // End of class CracenServerTest


int main() {
	TestSuite testSuite("Cracen Server Test");

	//CracenClientTest<AsioStreamingSocket> tcpServerTest(testSuite);
	CracenClientTest<AsioDatagramSocket> udpServerTest(testSuite);

}
