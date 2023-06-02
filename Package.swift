// swift-tools-version: 5.8
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "CP3_DSP",
    products: [
        // Products define the executables and libraries a package produces, and make them visible to other packages.
        .library(
            name: "CP3_DSP",
            targets: ["CP3_DSP", "CP3_DSP_ObjC"]),
    ],
    dependencies: [
        // Dependencies declare other packages that this package depends on.
        // .package(url: /* package url */, from: "1.0.0"),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .target(
            name: "CP3_DSP",
            dependencies: ["CP3_DSP_ObjC"]),
        .target(
            name: "CP3_DSP_ObjC",
            dependencies: [],
            path: "Sources/CP3_DSP_ObjC"),
        .testTarget(
            name: "CP3_DSPTests",
            dependencies: ["CP3_DSP"],
            path: "Tests/CP3_DSPTests"),
        .testTarget(
            name: "CP3_DSPTests_ObjC",
            dependencies: ["CP3_DSP_ObjC"],
            path: "Tests/CP3_DSPTests_ObjC"),
    ],
    cxxLanguageStandard: .cxx14
)
