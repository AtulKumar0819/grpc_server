#include <iostream>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"
#include <boost/process.hpp>
using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::UploadRequest;
using helloworld::UploadResponse;

namespace bp = boost::process;
int FileCopying(string inputFile)
{
  std::ifstream infile(inputFile);
  std::ofstream outfile("/home/innobit/Documents/output.txt");

  if (!infile.is_open())
  {
    std::cerr << "Error opening input file.\n";
    return 1;
  }
  if (!outfile.is_open())
  {
    std::cerr << "Error opening output file.\n";
    return 1;
  }
  char ch;
  while (infile.get(ch))
  {
    outfile.put(ch);
  }
  infile.close();
  outfile.close();
  std::cout << "File copied successfully.\n";
  return 0;
}

void close(){

}
class GreeterServiceImpl final : public Greeter::Service
{
  Status UploadFile(ServerContext *context, const UploadRequest *request,
                    UploadResponse *reply) override
  {

    bp::ipstream pipe_stream;
    bp::child c("echo atul", bp::std_out > pipe_stream);

    // Read the output
    std::string line;
    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
    {
      std::cout << line << std::endl;
    }
    
    c.wait(); // Wait for the process to finish

    c.exit_code();
    close(c);

    // int returnCode = system(request->filename().c_str());
    //         std::string debugLog = ("/home/innobit/debug.log");
    //     reply->set_debug_log(debugLog);
    // if (returnCode == 0)
    // {
    //   std::cout << "Script executed successfully." << std::endl;
    // }
    // else
    // {
    //   std::cerr << "Failed to execute script." << std::endl;
    // }
    // FileCopying(request->filename());
    return Status::OK;
  }
};



void RunServer()
{
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main()
{
  RunServer();
  return 0;
}