load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def utf8_range_deps():
    maybe(
        http_archive,
        name = "com_google_absl",
        url = "https://github.com/abseil/abseil-cpp/archive/8c0b94e793a66495e0b1f34a5eb26bd7dc672db0.zip",
        strip_prefix = "abseil-cpp-8c0b94e793a66495e0b1f34a5eb26bd7dc672db0",
        sha256 = "b9f490fae1c0d89a19073a081c3c588452461e5586e4ae31bc50a8f36339135e",
    )
    maybe(
        http_archive,
        name = "rules_fuzzing",
        sha256 = "d9002dd3cd6437017f08593124fdd1b13b3473c7b929ceb0e60d317cb9346118",
        strip_prefix = "rules_fuzzing-0.3.2",
        urls = ["https://github.com/bazelbuild/rules_fuzzing/archive/v0.3.2.zip"],
    )
