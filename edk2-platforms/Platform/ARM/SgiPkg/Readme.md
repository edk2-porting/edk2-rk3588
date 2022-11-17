Arm Neoverse Reference Design Platforms
=======================================

# Introduction

This document lists information related to the usage of EDKII firmware for Arm's
Neoverse Reference Design platforms. Details about Arm's Neoverse Reference
Design platforms is listed on this [page](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra)

The platform code is located under Platform/Arm/SgiPkg. Following platforms are
supported in this platform code.

- [RD-N2](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra/rdn2)
- [RD-V1](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra/rdv1)
- [RD-V1-MC](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra/rdv1mc)
- [RD-N1-Edge](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra/rdn1edge)
- [RD-N1-Edge-X2](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra/rdn1edgex2)
- [RD-E1-Edge](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra/rde1edge)
- [SGI-575](https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/tree/master/docs/infra/sgi575)

# Build Environment

- x86 Linux host machines running Ubuntu 18.04 or later
- [Toolchain](https://developer.arm.com/-/media/Files/downloads/gnu-a/8.3-2019.03/binrel/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu.tar.xz)

# How to build and execute

Please follow top-level Readme.md for build instructions. For details on building
the rest of the software components and executing it using the platform FVP, use
the links to the respective platforms listed above.

# Obtaining the platform fvp

The fixed virtual platform for the supported platforms can be obtained
[here](https://developer.arm.com/tools-and-software/open-source-software/arm-platforms-software/arm-ecosystem-fvps)
