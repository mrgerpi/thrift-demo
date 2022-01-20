#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "Calculator.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace tutorial;

class CalculatorHandler : public CalculatorIf {
public:
  CalculatorHandler() {}

  void ping() { cout << "ping()" << endl; }

  int32_t add(const int32_t n1, const int32_t n2) {
    cout << "add(" << n1 << ", " << n2 << ")" << endl;
    return n1 + n2;
  }

  int32_t calculate(const int32_t logid, const Work& work) {
    cout << "calculate(" << logid << ", " << work << ")" << endl;
    int32_t val;

    switch (work.op) {
    case Operation::ADD:
      val = work.num1 + work.num2;
      break;
    case Operation::SUBTRACT:
      val = work.num1 - work.num2;
      break;
    case Operation::MULTIPLY:
      val = work.num1 * work.num2;
      break;
    case Operation::DIVIDE:
      break;
    }

    return val;
  }

};

int main() {
  boost::shared_ptr<CalculatorHandler> handler(new CalculatorHandler());
  boost::shared_ptr<TProcessor> processor(new CalculatorProcessor(handler));
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  int thread_num = 5;
  int task_num_max = 10;
  int THREAD_STACK_SIZE = 64;
  boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(thread_num, task_num_max);
  boost::shared_ptr<PosixThreadFactory> threadFactory = ::boost::shared_ptr<PosixThreadFactory>(
          new PosixThreadFactory(apache::thrift::concurrency::PosixThreadFactory::ROUND_ROBIN,
              apache::thrift::concurrency::PosixThreadFactory::NORMAL,
              THREAD_STACK_SIZE)
          );
  threadManager->threadFactory(threadFactory);
  threadManager->start();

  int port = 9090;
  int task_expire = 50;
  int io_thread_num = 10;
  apache::thrift::server::TNonblockingServer* server_ = new TNonblockingServer(processor, protocolFactory, port, threadManager);
  try {
      server_->setTaskExpireTime(task_expire);
      server_->setNumIOThreads(io_thread_num);
      server_->serve();
  } catch(TException& tx) {
      threadManager->stop();
      _exit(1);
  }
  return 0;
}
