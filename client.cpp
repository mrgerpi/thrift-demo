#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "Calculator.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace tutorial;

int main() {

  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
  socket->setConnTimeout(100);
  socket->setRecvTimeout(500);
  socket->setSendTimeout(500);
  boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  transport->open();

  CalculatorClient client(protocol);
  try {
    transport->open();

    client.ping();
    cout << "ping()" << endl;

    cout << "1 + 1 = " << client.add(1, 1) << endl;

    Work work;
    work.op = Operation::DIVIDE;
    work.num1 = 1;
    work.num2 = 0;

    client.calculate(1, work);
    cout << "Whoa? We can divide by zero!" << endl;

    work.op = Operation::SUBTRACT;
    work.num1 = 15;
    work.num2 = 10;
    int32_t diff = client.calculate(1, work);
    cout << "15 - 10 = " << diff << endl;

    transport->close();
  } catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }
}
