load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "test",
    deps = [
        "//src:person_cc_proto",
        "//src:zip_code_grpc_cpp",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings:str_format",
    ],
    srcs = ["test.cc"],
)

cc_binary(
    name = "grpc_demo",
    deps = [
        "//src:person_cc_proto",
        "//src:zip_code_grpc_cpp",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings:str_format",
    ],
    srcs = ["grpc_demo.cc"],
)
