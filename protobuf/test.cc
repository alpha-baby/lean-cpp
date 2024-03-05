#include <fstream>
#include <iostream>

#include "src/zip_code.pb.h"
#include "src/zip_code.grpc.pb.h"

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

