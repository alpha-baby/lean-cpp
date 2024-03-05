load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_proto_grpc//cpp:defs.bzl", "cpp_grpc_library")


cc_proto_library(
    name = "person_cc_proto",
    deps = [":address_proto"],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "person_proto",
    srcs = ["address.proto"],
    deps = [":zip_code_proto"],
    visibility = ["//visibility:public"],
)


proto_library(
    name = "address_proto",
    srcs = ["address.proto"],
    deps = [":zip_code_proto"],
)

cpp_grpc_library(
    name = "zip_code_grpc_cpp",
    # protos = [":zip_code_proto"],
    protos = [
        ":zip_code_proto",
    ],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "zip_code_proto",
    srcs = ["zip_code.proto"],
    visibility = ["//visibility:public"],
)