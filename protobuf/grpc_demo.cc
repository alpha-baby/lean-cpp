#include <fstream>
#include <iostream>

#include "src/zip_code.pb.h"
#include "src/zip_code.grpc.pb.h"

#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

using ZCodePtr = std::unique_ptr<demo::ZipCode>;

int main2(int argc, char* argv[]) {
    ZCodePtr code_p = std::make_unique<demo::ZipCode>();
    code_p->set_code("code");
    std::cout << "hello world" << std::endl;

    std::cout << code_p->code() << std::endl;

    {
        // Read the existing address book.
        std::fstream input(argv[1], std::ios::in | std::ios::binary);
        if (!input) {
            std::cout << argv[1] << ": File not found.  Creating a new file."
                      << std::endl;
        } else if (!code_p->ParseFromIstream(&input)) {
            std::cerr << "Failed to parse address book." << std::endl;
            return -1;
        }
    }

    // Add an address.
    code_p->set_code("hello world protobuf");
    code_p->set_number(23);

    {
        // Write the new address book back to disk.
        std::fstream output(argv[1],
                            std::ios::out | std::ios::trunc | std::ios::binary);
        if (!code_p->SerializeToOstream(&output)) {
            std::cerr << "Failed to write address book." << std::endl;
            return -1;
        }
    }
    
    // 释放资源
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

// Logic and data behind the server's behavior.
class ZipServiceImpl final : public demo::ZipService::Service {
  grpc::Status Search(grpc::ServerContext* context, const demo::ZipCode* request, demo::ZipCode* response) override {
    std::string prefix("Hello ");
    response->set_code(prefix + request->code());
    return grpc::Status::OK;
  }
};

void RunServer(uint16_t port) {
  std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
  ZipServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  RunServer(absl::GetFlag(FLAGS_port));
  return 0;
}